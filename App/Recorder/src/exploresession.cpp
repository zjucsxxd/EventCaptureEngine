#include "exploresession.h"

#include "dlcrmain.h"
#include "locationutil.h"
#include "roomutil.h"
#include "eventtypeutil.h"
#include "roomeventtypeutil.h"
#include "userroomeventtypeutil.h"
#include "userutil.h"
#include "sessionutil.h"
#include "appsettingsutil.h"
#include "eventutil.h"
#include "pcutil.h"
#include "systemfetch.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <stdio.h>

ExploreSession::ExploreSession(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	sharedObj=new DLCRShared();
	logSession=LoginUserSession::session();

	icoOpen=QIcon(QPixmap(":/Recorder/Resources/webcamera.png"));
	icoClosed=QIcon(QPixmap(":/Recorder/Resources/closed.png"));

	connect(ui.cmbLocation,			SIGNAL(currentIndexChanged(int)),	this,SLOT(cmbLocationName_indexChanged(int)));
	connect(ui.cmbRoom,				SIGNAL(currentIndexChanged(int)),	this,SLOT(cmbRoomName_indexChanged(int)));
	connect(ui.tblResumeSession,	SIGNAL(cellClicked(int,int)),		this,SLOT(eventDetails_cellClicked(int,int)));
	connect(ui.tblResumeSession,	SIGNAL(currentCellChanged(int,int,int,int)),this, SLOT(tblEventDetails_cellChanged(int,int)));
	connect(ui.cmbSearchBox,		SIGNAL(currentIndexChanged(int)),	this,SLOT(cmbSearchCategory_indexChanged(int)));
	connect(ui.txtSearch,			SIGNAL(textChanged(QString)),		this,SLOT(txtSearch_textChanged(QString)));
	connect(ui.btnStart,			SIGNAL(clicked()),					this,SLOT(btnStartEvent_clicked()));
	connect(ui.btnClose,			SIGNAL(clicked()),					this,SLOT(btnCloseEvent_clicked()));
	connect(ui.btnOpen,				SIGNAL(clicked()),					this,SLOT(btnReopenEvent_clicked()));
	connect(ui.btnResume,			SIGNAL(clicked()),					this,SLOT(btnResumeEvent_clicked()));	
	connect(ui.btnViewNotes,		SIGNAL(clicked()),					this,SLOT(btnViewNotes_clicked()));
	connect(ui.btnAutoFill,			SIGNAL(clicked()),					this,SLOT(btnAutoFill_clicked()));
	connect(ui.txtEventDescription, SIGNAL(textChanged()),				this,SLOT(validateDescription()));

	if(NOT logSession->canStartEvent())
	{
		ui.tabWidget->setTabEnabled(0,false);
	}
}
void ExploreSession::btnAutoFill_clicked()
{
	PCUtil *putil=new PCUtil(this);
	SystemFetch *fetch=new SystemFetch();
	EventUtil *util=new EventUtil(this);
	QList<EventInfo> allEventsofPC;
	QString eventdes;
	try
	{
		PCInfo info=putil->loadPCDetails(fetch->getSystemID(),logSession->getDriverName());
		allEventsofPC=util->loadEventsOFPC(info.getPCID(),logSession->getDriverName());
		foreach(EventInfo einfo,allEventsofPC)
		{
			eventdes=einfo.getEventDescription();
		}
		ui.txtEventDescription->setText(eventdes);
	}
	catch(DBExeption e)//catching db exeptions
	{
		if(e==ExecutionFailed)
		{
			PRINT_ERROR("insertion failed");	
		}
		if(e==UnableToConnect)
		{
			PRINT_ERROR("Unable to connect to db");	
		}	
		if(e==EmptyResultSet)
		{
			PRINT_WARNING("Empty result set");
		}
	}
	catch(...)//catching unknown wxwptions
	{
		PRINT_ERROR("UNKnown ERROR");
	}

}
void ExploreSession::btnStartEvent_clicked()
{
	if(ui.txtEventNo->text().trimmed()=="" || ui.cmbEventType->currentText()=="") return;

	if(ui.txtEventName->text().trimmed()=="") ui.txtEventName->setText("--");
	if(ui.txtEventDescription->toPlainText().trimmed()=="") ui.txtEventDescription->setText("--");

	EventUtil *util=new EventUtil(this);
	SessionUtil *session=new SessionUtil(this);
	AppSettingsUtil *settings=new AppSettingsUtil(this);

	try
	{
		if (util->existence(ui.txtEventNo->text().trimmed(),logSession->getCurrentRoom(),logSession->getDriverName())!=true)
		{
			int eventTypeID=ui.cmbEventType->itemData(ui.cmbEventType->currentIndex(),Qt::UserRole).toInt();

			EventInfo eInfo;
			eInfo.setEventNo(ui.txtEventNo->text().trimmed());
			eInfo.setEventName(ui.txtEventName->text().trimmed());
			eInfo.setEventDescription(ui.txtEventDescription->toPlainText().trimmed());
			eInfo.setEventTypeID(eventTypeID);
			eInfo.setEventEEL("");
			eInfo.setUserID(logSession->getUserID());

			int eventID=util->createEvent(eInfo,logSession->getDriverName());

			AppSettingsInfo sInfo=settings->loadAppSettings(logSession->getPCID(),logSession->getDriverName());
			SessionInfo info;

			if (sInfo.getSplittingDuration()!=1000000)
			{
				info.setMergeStatus(0);
			}
			else
			{
				info.setMergeStatus(1);
			}

			int sessionID=session->createSession(info,logSession->getDriverName());

			logSession->setEventID(eventID);
			logSession->setEventNo(ui.txtEventNo->text());
			logSession->setEventName(ui.txtEventName->text());
			logSession->setEventDescription(ui.txtEventDescription->toPlainText());
			logSession->setSessionID(sessionID);
			logSession->setCurrentEventType(eventTypeID);

			DLCRMain *dlcrs=static_cast<DLCRMain*>(this->parent());

			QTableWidgetItem *itemtxtUser=new QTableWidgetItem();
			QTableWidgetItem *itemtxtDateTime=new QTableWidgetItem();
			QTableWidgetItem *itemtxtLocation=new QTableWidgetItem();
			QTableWidgetItem *itemtxtRoom=new QTableWidgetItem();
			itemtxtUser=ui.tblNewSession->item(0,0);
			itemtxtDateTime=ui.tblNewSession->item(0,3);
			itemtxtLocation=ui.tblNewSession->item(0,1);
			itemtxtRoom=ui.tblNewSession->item(0,2);
			QString locationName,roomName;
			locationName=itemtxtLocation->text();
			roomName=itemtxtRoom->text();

			caseDetails details;
			details.no=logSession->getEventNo();
			details.title=logSession->getEventName();
			details.description=logSession->getEventDescription();
			details.dt=sharedObj->getCurrentDateTime();
			details.location=locationName;
			details.room=roomName;
			details.eventType=ui.cmbEventType->currentText();

			dlcrs->ui.activeEditor->setCaseDetails(details);

			QString eventHomePath=sharedObj->getEventHomePath(locationName,roomName,ui.cmbEventType->currentText(),logSession->getEventNo());
			logSession->setEventHomePath(eventHomePath);
			int videoCount=dlcrs->getVideoCount();
			QStringList filePath=sharedObj->getEventFileName(videoCount,logSession->getEventNo());
			QStringList filepaths;
			for (int i=0;i<videoCount;i++)
			{
				filepaths<<eventHomePath+"/"+filePath[i];
			}
			/*QString filePath="/"+sharedObj->getEventFileName(logSession->getEventNo());
			filePath.insert(0,eventHomePath);*/

			logSession->setCurrentFilePath(filepaths);

			dlcrs->startEvent();

			ui.txtEventNo->clear();
			ui.txtEventName->clear();
			ui.txtEventDescription->clear();

			this->close();
		}
		else
		{
			loadUserDetails();
			ui.txtSearch->setText(ui.txtEventNo->text());
			ui.tabWidget->setCurrentIndex(1);	
		}
	}
	catch(DBExeption e)//catching db exeptions
	{
		if(e==ExecutionFailed)
		{
			PRINT_ERROR("insertion failed");	
		}
		if(e==UnableToConnect)
		{
			PRINT_ERROR("Unable to connect to db");	
		}	
		if(e==EmptyResultSet)
		{
			PRINT_WARNING("Empty result set");
		}
	}
	catch(...)//catching unknown wxwptions
	{
		PRINT_ERROR("UNKnown ERROR");
	}

	delete settings;
	delete session;
	delete util;
}

void ExploreSession::validateDescription()
{
	QString description;
	int leftChar = 0;
	leftChar = 950 - ui.txtEventDescription -> toPlainText().size(); 
	if(leftChar < 0){
		description = (ui.txtEventDescription -> toPlainText()).left(950); //more than maxChar so keep only the first maxChar characters
		ui.txtEventDescription -> setText(description); //set these characters as the visble
		ui.txtEventDescription -> moveCursor(QTextCursor::End,QTextCursor::MoveAnchor); //set cursor to the end
	}
}

void ExploreSession::cmbRoomName_indexChanged(int index_)
{
	cmbSearchCategory_indexChanged(ui.cmbSearchBox->currentIndex());

	if (ui.cmbRoom->itemData(index_,Qt::UserRole).toInt()==logSession->getCurrentRoom())
	{
		ui.btnResume->setVisible(true);
	}
	else
	{
		ui.btnResume->setVisible(false);
	}
}

void ExploreSession::loadUserDetails()
{
	ui.txtSearch->clear();

	for (int i=ui.tblNewSession->rowCount(); i >= 0; --i)
	{
		ui.tblNewSession->removeRow(i);	
	}

	loadEventTypes();
	loadLocations();

	txtSearch_textChanged(ui.txtSearch->text());

	LocationUtil *lUtil=new LocationUtil(this);
	RoomUtil *rUtil=new RoomUtil(this);

	try
	{
		QTableWidgetItem *itemtxtUser=new QTableWidgetItem();
		QTableWidgetItem *itemtxtDateTime=new QTableWidgetItem();
		QTableWidgetItem *itemtxtLocation=new QTableWidgetItem();
		QTableWidgetItem *itemtxtRoom=new QTableWidgetItem();
	
		LocationInfo lInfo=lUtil->loadLocation(logSession->getCurrentLocation(),logSession->getDriverName());
		itemtxtLocation->setText(lInfo.getLocationName());
		
		RoomInfo rInfo=rUtil->loadRoom(logSession->getCurrentRoom(),logSession->getDriverName());
		itemtxtRoom->setText(rInfo.getRoomName());
		
		itemtxtUser->setText(logSession->getUserName());
		itemtxtDateTime->setText(sharedObj->getCurrentDateTime().toString("dd/MM/yyyy hh-mm-ss AP"));
		
		int locationIndex=ui.cmbLocation->findText(lInfo.getLocationName(),Qt::MatchExactly | Qt::MatchCaseSensitive);
		ui.cmbLocation->setCurrentIndex(locationIndex);

		int roomIndex=ui.cmbRoom->findText(rInfo.getRoomName(),Qt::MatchExactly | Qt::MatchCaseSensitive);
		ui.cmbRoom->setCurrentIndex(roomIndex);

		ui.tblNewSession->insertRow(0);
		ui.tblNewSession->setItem(0,0,itemtxtUser);
		ui.tblNewSession->setItem(0,1,itemtxtLocation);
		ui.tblNewSession->setItem(0,2,itemtxtRoom);
		ui.tblNewSession->setItem(0,3,itemtxtDateTime);

	}
	catch(DBExeption e)//catching db exeptions
	{
		if(e==ExecutionFailed)
		{
			PRINT_ERROR("insertion failed");	
		}
		if(e==UnableToConnect)
		{
			PRINT_ERROR("Unable to connect to db");	
		}	
		if(e==EmptyResultSet)
		{
			PRINT_WARNING("Empty result set");
		}
	}
	catch(...)//catching unknown wxwptions
	{
		PRINT_ERROR("UNKnown ERROR");
	}

	delete lUtil;
	delete rUtil;
}

void ExploreSession::loadEventTypes()
{
	EventTypeUtil *util =new EventTypeUtil(this);

	try
	{	
		ui.cmbEventType->blockSignals(true);
		ui.cmbEventType->clear();
		ui.cmbEventType->blockSignals(false);

		//getting all event types
		QList<EventTypeInfo> allEventTypes=util->selectAllEventTypes(logSession->getDriverName());
		int k=0;
		foreach(EventTypeInfo info,allEventTypes)
		{
			if (isAssigned(logSession->getUserID(),logSession->getCurrentRoom(),info.getEventTypeID()))
			{
				ui.cmbEventType->insertItem(k,info.getEventTypeName(),info.getEventTypeID());
				k++;
			}
		}		
	}	
	catch(DBExeption e)//catching db exceptions
	{
		if(e==ExecutionFailed)
		{
			PRINT_ERROR("insertion failed");	
		}
		if(e==UnableToConnect)
		{
			PRINT_ERROR("Unable to connect to db");	
		}	
		if(e==EmptyResultSet)
		{
			PRINT_WARNING("Empty result set");
		}
	}
	catch(...)//catching unknown exceptions
	{
		PRINT_ERROR("UNKnown ERROR");
	}

	delete util;		
}

bool ExploreSession::isAssigned(int userID_,int roomID_,int eventTypeID_)
{			

	UserRoomEventTypeUtil *ureUtil=new UserRoomEventTypeUtil(this);
	bool assignedStatus;

	try
	{
		RoomEventTypeUtil *util=new RoomEventTypeUtil(this);

		//selecting room event id by room and event id's
		int roomEventTypeID=util->selectRoomEventTypeID(roomID_,eventTypeID_,logSession->getDriverName());

		//loading info by user id and room event type id
		UserRoomEventTypeInfo info=ureUtil->loadUserEventType(userID_,roomEventTypeID,logSession->getDriverName());

		//checking visibility of event type
		if (info.getVisibility()!=0)
		{
			assignedStatus=true;
		}
		else
		{
			assignedStatus=false;
		}

		delete util;
	}
	catch(DBExeption e)//catching db exeptions
	{
		if(e==ExecutionFailed)
		{
			PRINT_ERROR("insertion failed");	
		}
		if(e==UnableToConnect)
		{
			PRINT_ERROR("Unable to connect to db");	
		}	
		if(e==EmptyResultSet)
		{
			PRINT_WARNING("Empty result set");
		}
	}
	catch(...)//catching unknown wxwptions
	{
		PRINT_ERROR("UNKnown ERROR");
	}

	delete ureUtil;

	return assignedStatus;

}

void ExploreSession::loadLocations()
{
	LocationUtil *util=new LocationUtil(this);

	try
	{
		ui.cmbLocation->blockSignals(true);
		ui.cmbLocation->clear();
		ui.cmbLocation->blockSignals(false);

		QList<int> userLocationList=logSession->getUserLocationIDs();
		for(int i=0;i<userLocationList.size();i++)
		{
			LocationInfo info=util->loadLocation(userLocationList[i],logSession->getDriverName());
			ui.cmbLocation->insertItem(i,info.getLocationName(),info.getLocationID());
		}
	}
	catch(DBExeption e)//catching db exeptions
	{
		if(e==ExecutionFailed)
		{
			PRINT_ERROR("insertion failed");	
		}
		if(e==UnableToConnect)
		{
			PRINT_ERROR("Unable to connect to db");	
		}	
		if(e==EmptyResultSet)
		{
			PRINT_WARNING("Empty result set");
		}
	}
	catch(...)//catching unknown wxwptions
	{
		PRINT_ERROR("UNKnown ERROR");
	}

	delete util;

	cmbLocationName_indexChanged(ui.cmbLocation->currentIndex());
}
void ExploreSession::cmbLocationName_indexChanged(int index_)
{
	RoomUtil *util =new RoomUtil(this);

	try
	{
		ui.cmbRoom->blockSignals(true);
		ui.cmbRoom->clear();
		ui.cmbRoom->blockSignals(false);

		int locationID=ui.cmbLocation->itemData(index_,Qt::UserRole).toInt();
		//getting all rooms by location
		QList<RoomInfo> locationRooms=util->selectRoomsOfLocation(locationID,logSession->getDriverName());

		QList<int> userRoomList=logSession->getUserRoomIDs();

		for (int i=0;i<userRoomList.size();i++)
		{
			//inserting location details as widget items to tblLocations
			foreach(RoomInfo info,locationRooms)
			{
				if (info.getRoomID()==userRoomList[i])
				{
					ui.cmbRoom->insertItem(i,info.getRoomName(),info.getRoomID());
					break;
				}
			}
		}		
	}
	catch(DBExeption e)//catching db exeptions
	{
		if(e==ExecutionFailed)
		{
			PRINT_ERROR("insertion failed");	
		}
		if(e==UnableToConnect)
		{
			PRINT_ERROR("Unable to connect to db");	
		}	
		if(e==EmptyResultSet)
		{
			PRINT_WARNING("Empty result set");
		}
	}
	catch(...)//catching unknown wxwptions
	{
		PRINT_ERROR("UNKnown ERROR");
	}

	delete util;
}
void ExploreSession::cmbSearchCategory_indexChanged(int index_)
{
	if (index_==0)
	{
		loadAllEvents();
		txtSearch_textChanged(ui.txtSearch->text());
	}
	else
	{
		loadRescentEvents();
		txtSearch_textChanged(ui.txtSearch->text());
	}	
}

void ExploreSession::txtSearch_textChanged(QString str_)
{
	//for removing rows from table widget
	for (int i=ui.tblResumeSession->rowCount(); i >= 0; --i)
	{
		ui.tblResumeSession->removeRow(i);	
	}

	listEvents(str_);

	ui.btnClose->setEnabled(false);
	ui.btnResume->setEnabled(false);
	ui.btnOpen->setEnabled(false);
	ui.btnViewNotes->setEnabled(false);
}
void ExploreSession::loadAllEvents()
{
	EventUtil *util=new EventUtil(this);

	try
	{
		int roomID=ui.cmbRoom->itemData(ui.cmbRoom->currentIndex(),Qt::UserRole).toInt();

		allEvents.clear();

		QList<EventInfo> totalEvents=util->loadEventsOFRoom(roomID,logSession->getDriverName());

		int k=0;

		for(int i=0;i<totalEvents.size();i++)
		{			
			if (totalEvents[i].getEventStatus()=="Closed")
			{
				if(logSession->canViewClosedEvent()) allEvents<<totalEvents[i];
				else continue;
			}
			else
			{
				allEvents<<totalEvents[i];
			}

			k++;
		}
	}	
	catch(DBExeption e)//catching db exeptions
	{
		if(e==ExecutionFailed)
		{
			PRINT_ERROR("insertion failed");	
		}
		if(e==UnableToConnect)
		{
			PRINT_ERROR("Unable to connect to db");	
		}	
		if(e==EmptyResultSet)
		{
			PRINT_WARNING("Empty result set");
		}
	}
	catch(...)//catching unknown wxwptions
	{
		PRINT_ERROR("UNKnown ERROR");
	}

	delete util;
}
void ExploreSession::loadRescentEvents()
{
	EventUtil *util=new EventUtil(this);

	try
	{
		int roomID=ui.cmbRoom->itemData(ui.cmbRoom->currentIndex(),Qt::UserRole).toInt();

		allEvents.clear();

		QList<EventInfo> totalEvents=util->loadEventsOFRoom(roomID,logSession->getDriverName());

		int k=0;

		for(int i=totalEvents.size()-1;i>=0;i--)
		{

			if (totalEvents[i].getEventStatus()=="Closed")
			{
				if(logSession->canViewClosedEvent()) allEvents<<totalEvents[i];
				else continue;
			}
			else
			{
				allEvents<<totalEvents[i];
			}

			if(k==6) break;

			k++;
		}
	}	
	catch(DBExeption e)//catching db exeptions
	{
		if(e==ExecutionFailed)
		{
			PRINT_ERROR("insertion failed");	
		}
		if(e==UnableToConnect)
		{
			PRINT_ERROR("Unable to connect to db");	
		}	
		if(e==EmptyResultSet)
		{
			PRINT_WARNING("Empty result set");
		}
	}
	catch(...)//catching unknown wxwptions
	{
		PRINT_ERROR("UNKnown ERROR");
	}

	delete util;
}

void ExploreSession::listEvents(QString str_)
{
	try
	{
		int k=0;

		foreach(EventInfo info,allEvents)
		{
			QTableWidgetItem *itemEventNo,*itemEventTitle,*itemEventDescription,*itemCreatedDate,*itemEventType,*itemStaus,*vHItem;

			itemEventNo=new QTableWidgetItem();
			itemEventTitle=new QTableWidgetItem();
			itemEventDescription=new QTableWidgetItem();
			itemCreatedDate=new QTableWidgetItem();
			itemEventType=new QTableWidgetItem();
			itemStaus=new QTableWidgetItem();
			vHItem=new QTableWidgetItem();

			itemEventNo->setText(info.getEventNo());
			itemEventNo->setData(Qt::UserRole,info.getEventID());
			itemEventTitle->setText(info.getEventName());
			itemEventDescription->setText(info.getEventDescription());
			itemCreatedDate->setText(info.getCreatedDate().toString("d-M-yyyy"));

			if (info.getEventStatus()!="Closed")
			{

				itemEventNo->setIcon(icoOpen);
				//vHItem->setIcon(icoOpen);
			}
			else 
			{
//				if(logSession->canViewClosedEvent()) itemFinishedDate->setText(info.getFinishedDate().toString("d-M-yyyy"));
//				else continue;
				itemEventNo->setIcon(icoClosed);
				//vHItem->setIcon(icoClosed);
			}

			EventTypeUtil *etUtil=new EventTypeUtil(this);
			itemEventType->setText(etUtil->loadEventType(info.getEventTypeID(),logSession->getDriverName()).getEventTypeName());
			itemEventType->setData(Qt::UserRole,info.getEventTypeID());
			delete etUtil;

			itemStaus->setText(info.getEventStatus());

			if(!info.getEventNo().contains(str_)) continue;

			ui.tblResumeSession->insertRow(k);

			//ui.tblEventDetails->setVerticalHeaderItem(k,vHItem);

			ui.tblResumeSession->setItem(k,0,itemEventNo);
			ui.tblResumeSession->setItem(k,1,itemEventTitle);
			ui.tblResumeSession->setItem(k,2,itemEventDescription);
			ui.tblResumeSession->setItem(k,3,itemCreatedDate);
			ui.tblResumeSession->setItem(k,4,itemEventType);
			ui.tblResumeSession->setItem(k,5,itemStaus);

			k++;
		}
	}
	catch(DBExeption e)//catching db exeptions
	{
		if(e==ExecutionFailed)
		{
			PRINT_ERROR("insertion failed");	
		}
		if(e==UnableToConnect)
		{
			PRINT_ERROR("Unable to connect to db");	
		}	
		if(e==EmptyResultSet)
		{
			PRINT_WARNING("Empty result set");
		}
	}
	catch(...)//catching unknown wxwptions
	{
		PRINT_ERROR("UNKnown ERROR");
	}
}

void ExploreSession::btnCloseEvent_clicked()
{
	EventUtil *util=new EventUtil(this);

	try
	{
		int eventID=ui.tblResumeSession->item(ui.tblResumeSession->currentRow(),0)->data(Qt::UserRole).toInt();

		EventInfo info;
		info.setEventID(eventID);
		info.setFinishedDate(sharedObj->getCurrentDateTime());
		info.setEventStatus("Closed");
		info.setEventEEL("");
		info.setUserID(logSession->getUserID());

		util->editEvent(info,logSession->getDriverName());
	}
	catch(DBExeption e)//catching db exeptions
	{
		if(e==ExecutionFailed)
		{
			PRINT_ERROR("insertion failed");	
		}
		if(e==UnableToConnect)
		{
			PRINT_ERROR("Unable to connect to db");	
		}	
		if(e==EmptyResultSet)
		{
			PRINT_WARNING("Empty result set");
		}
	}
	catch(...)//catching unknown wxwptions
	{
		PRINT_ERROR("UNKnown ERROR");
	}

	delete util;

	cmbRoomName_indexChanged(ui.cmbRoom->currentIndex());
}

void ExploreSession::btnReopenEvent_clicked()
{
	EventUtil *util=new EventUtil(this);

	try
	{
		int eventID=ui.tblResumeSession->item(ui.tblResumeSession->currentRow(),0)->data(Qt::UserRole).toInt();

		EventInfo info;
		info.setEventID(eventID);
		info.setFinishedDate(sharedObj->getCurrentDateTime());
		info.setEventStatus("Open");
		info.setEventEEL("");
		info.setUserID(logSession->getUserID());

		util->editEvent(info,logSession->getDriverName());
	}
	catch(DBExeption e)//catching db exeptions
	{
		if(e==ExecutionFailed)
		{
			PRINT_ERROR("insertion failed");	
		}
		if(e==UnableToConnect)
		{
			PRINT_ERROR("Unable to connect to db");	
		}	
		if(e==EmptyResultSet)
		{
			PRINT_WARNING("Empty result set");
		}
	}
	catch(...)//catching unknown wxwptions
	{
		PRINT_ERROR("UNKnown ERROR");
	}

	delete util;

	cmbRoomName_indexChanged(ui.cmbRoom->currentIndex());
}
void ExploreSession::btnResumeEvent_clicked()
{
	SLOT_ENTER

	EventUtil *util=new EventUtil(this);
	SessionUtil *session=new SessionUtil(this);
	AppSettingsUtil *settings=new AppSettingsUtil(this);

	printf("\Entered...\n");
	try
	{
		int eventID=ui.tblResumeSession->item(ui.tblResumeSession->currentRow(),0)->data(Qt::UserRole).toInt();
		int eventTypeID=ui.tblResumeSession->item(ui.tblResumeSession->currentRow(),4)->data(Qt::UserRole).toInt();

		AppSettingsInfo sInfo=settings->loadAppSettings(logSession->getPCID(),logSession->getDriverName());
		SessionInfo info;

		if (sInfo.getSplittingDuration()!=1000000)
		{
			info.setMergeStatus(0);
		}
		else
		{
			info.setMergeStatus(1);
		}

		int sessionID=session->createSession(info,logSession->getDriverName());

		logSession->setEventID(eventID);
		logSession->setEventNo(ui.tblResumeSession->item(ui.tblResumeSession->currentRow(),0)->text());
		logSession->setEventName(ui.tblResumeSession->item(ui.tblResumeSession->currentRow(),1)->text());
		logSession->setEventDescription(ui.tblResumeSession->item(ui.tblResumeSession->currentRow(),2)->text());
		logSession->setSessionID(sessionID);
		logSession->setCurrentEventType(eventTypeID);
		printf("\logSession Entered...\n");
		DLCRMain *dlcrs=static_cast<DLCRMain*>(this->parent());

		caseDetails details;
		details.no=logSession->getEventNo();
		details.title=logSession->getEventName();
		details.description=logSession->getEventDescription();
		details.dt=sharedObj->getCurrentDateTime();
		details.location=ui.cmbLocation->currentText();
		details.room=ui.cmbRoom->currentText();
		details.eventType=ui.tblResumeSession->item(ui.tblResumeSession->currentRow(),4)->text();

		dlcrs->ui.activeEditor->setCaseDetails(details);

		QString eventHomePath=sharedObj->getEventHomePath(ui.cmbLocation->currentText(),ui.cmbRoom->currentText(),ui.tblResumeSession->item(ui.tblResumeSession->currentRow(),4)->text(),logSession->getEventNo());
		logSession->setEventHomePath(eventHomePath);

		int videoCount=dlcrs->getVideoCount();
		QStringList filePath=sharedObj->getEventFileName(videoCount,logSession->getEventNo());
		QStringList filepaths;
		for (int i=0;i<videoCount;i++)
		{
			filepaths<<eventHomePath+"/"+filePath[i];
		}
		logSession->setCurrentFilePath(filepaths);
		printf("\ngoing to start...\n");
		dlcrs->startEvent();

		this->close();		
	}
	catch(DBExeption e)//catching db exeptions
	{
		if(e==ExecutionFailed)
		{
			PRINT_ERROR("insertion failed");	
		}
		if(e==UnableToConnect)
		{
			PRINT_ERROR("Unable to connect to db");	
		}	
		if(e==EmptyResultSet)
		{
			PRINT_WARNING("Empty result set");
		}
	}
	catch(...)//catching unknown wxwptions
	{
		PRINT_ERROR("UNKnown ERROR");
	}

	delete settings;
	delete session;
	delete util;

	SLOT_EXIT
}

void ExploreSession::btnViewNotes_clicked()
{
	SLOT_ENTER

	int eventID=ui.tblResumeSession->item(ui.tblResumeSession->currentRow(),0)->data(Qt::UserRole).toInt();
	int roomID=ui.cmbRoom->itemData(ui.cmbRoom->currentIndex(),Qt::UserRole).toInt();

	DLCRMain *dlcrs=static_cast<DLCRMain*>(this->parent());

	retrieveDetails details;
	details.no=ui.tblResumeSession->item(ui.tblResumeSession->currentRow(),0)->text();
	details.title=ui.tblResumeSession->item(ui.tblResumeSession->currentRow(),1)->text();
	details.description=ui.tblResumeSession->item(ui.tblResumeSession->currentRow(),2)->text();
	QString strDateTime=ui.tblResumeSession->item(ui.tblResumeSession->currentRow(),3)->text();
	details.dt=QDateTime::fromString(strDateTime,"d-M-yyyy");
	details.eventType=ui.tblResumeSession->item(ui.tblResumeSession->currentRow(),4)->text();

	dlcrs->ui.activeRetriever->setEventDetails(details);

	logSession->setEventID(eventID);
	logSession->setRetrievedID(eventID);
	logSession->setRetrievedLocation(ui.cmbLocation->currentText());
	logSession->setRetrievedRoom(ui.cmbRoom->currentText());

	dlcrs->retrieveEvent(eventID,roomID);

	this->close();

	SLOT_EXIT
}

void ExploreSession::eventDetails_cellClicked(int row_,int column_)
{
	int roomID=ui.cmbRoom->itemData(ui.cmbRoom->currentIndex(),Qt::UserRole).toInt();
	int eventTypeID=ui.tblResumeSession->item(ui.tblResumeSession->currentRow(),4)->data(Qt::UserRole).toInt();

	if(ui.tblResumeSession->item(row_,5)->text()=="Open")
	{
		if(isAssigned(logSession->getUserID(),roomID,eventTypeID))
		{
			if(logSession->canCloseEvent()) ui.btnClose->setEnabled(true);
			else ui.btnClose->setEnabled(false);
			if(logSession->canResumeEvent()) ui.btnResume->setEnabled(true);
			else ui.btnResume->setEnabled(false);
			ui.btnOpen->setEnabled(false);
			if(logSession->canRetrieveEvent()) ui.btnViewNotes->setEnabled(true);
			else ui.btnViewNotes->setEnabled(false);
		}
		else
		{
			ui.btnClose->setEnabled(false);
			ui.btnResume->setEnabled(false);
			ui.btnOpen->setEnabled(false);
			ui.btnViewNotes->setEnabled(false);
		}
	}
	else
	{			
		if(isAssigned(logSession->getUserID(),roomID,eventTypeID))
		{	
			ui.btnClose->setEnabled(false);
			ui.btnResume->setEnabled(false);
			if(logSession->canReopenEvent()) ui.btnOpen->setEnabled(true);	
			else ui.btnOpen->setEnabled(false);	
			if(logSession->canRetrieveEvent()) ui.btnViewNotes->setEnabled(true);
			else ui.btnViewNotes->setEnabled(false);
		}
		else
		{
			ui.btnClose->setEnabled(false);
			ui.btnResume->setEnabled(false);
			ui.btnOpen->setEnabled(false);
			ui.btnViewNotes->setEnabled(false);
		}
	}
}

void ExploreSession::tblEventDetails_cellChanged(int row_,int column_)
{
	if(this->layoutDirection()==Qt::RightToLeft)
	{
		ui.tblResumeSession->resize(ui.tblResumeSession->width()+1,ui.tblResumeSession->height());
		ui.tblResumeSession->resize(ui.tblResumeSession->width()-1,ui.tblResumeSession->height());
	}

	int roomID=ui.cmbRoom->itemData(ui.cmbRoom->currentIndex(),Qt::UserRole).toInt();
	if(ui.tblResumeSession->item(ui.tblResumeSession->currentRow(),5)==NULL) return;
	int eventTypeID=ui.tblResumeSession->item(ui.tblResumeSession->currentRow(),4)->data(Qt::UserRole).toInt();

	if(ui.tblResumeSession->item(row_,5)->text()=="Open")
	{
		if(isAssigned(logSession->getUserID(),roomID,eventTypeID))
		{
			if(logSession->canCloseEvent()) ui.btnClose->setEnabled(true);
			else ui.btnClose->setEnabled(false);
			if(logSession->canResumeEvent()) ui.btnResume->setEnabled(true);
			else ui.btnResume->setEnabled(false);
			ui.btnOpen->setEnabled(false);
			if(logSession->canRetrieveEvent()) ui.btnViewNotes->setEnabled(true);
			else ui.btnViewNotes->setEnabled(false);
		}
		else
		{
			ui.btnClose->setEnabled(false);
			ui.btnResume->setEnabled(false);
			ui.btnOpen->setEnabled(false);
			ui.btnViewNotes->setEnabled(false);
		}
	}
	else
	{			
		if(isAssigned(logSession->getUserID(),roomID,eventTypeID))
		{	
			ui.btnClose->setEnabled(false);
			ui.btnResume->setEnabled(false);
			if(logSession->canReopenEvent()) ui.btnOpen->setEnabled(true);	
			else ui.btnOpen->setEnabled(false);	
			if(logSession->canRetrieveEvent()) ui.btnViewNotes->setEnabled(true);
			else ui.btnViewNotes->setEnabled(false);
		}
		else
		{
			ui.btnClose->setEnabled(false);
			ui.btnResume->setEnabled(false);
			ui.btnOpen->setEnabled(false);
			ui.btnViewNotes->setEnabled(false);
		}
	}
}
ExploreSession::~ExploreSession()
{

}
