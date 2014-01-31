#include "explorecase.h"

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

#include <stdio.h>

ExploreCase::ExploreCase(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	sharedObj=new DLCRShared();
	logSession=LoginUserSession::session();

	icoDown=QIcon(QPixmap(":/Recorder/Resources/down.png"));
	icoTop=QIcon(QPixmap(":/Recorder/Resources/top.png"));
	icoOpen=QIcon(QPixmap(":/Recorder/Resources/webcamera.png"));
	icoClosed=QIcon(QPixmap(":/Recorder/Resources/closed.png"));
	
	ui.frameMoreDetails->setHidden(true);
	ui.btnMore->setIcon(icoTop);

	connect(ui.cmbLocationName, SIGNAL(currentIndexChanged(int)), this,SLOT(cmbLocationName_indexChanged(int)));
	connect(ui.cmbRoomName, SIGNAL(currentIndexChanged(int)), this,SLOT(cmbRoomName_indexChanged(int)));
	connect(ui.tblEventDetails, SIGNAL(cellClicked(int,int)),	this,SLOT(eventDetails_cellClicked(int,int)));
	connect(ui.tblEventDetails,SIGNAL(currentCellChanged(int,int,int,int)),this, SLOT(tblEventDetails_cellChanged(int,int)));
	connect(ui.cmbSearchCategory,SIGNAL(currentIndexChanged(int)),	this,SLOT(cmbSearchCategory_indexChanged(int)));
	connect(ui.txtSearch,	SIGNAL(textChanged(QString)),	this,SLOT(txtSearch_textChanged(QString)));
	connect(ui.btnMore,		SIGNAL(clicked()),	this,SLOT(btnMore_clicked()));
	connect(ui.btnStartEvent,	SIGNAL(clicked()), this,SLOT(btnStartEvent_clicked()));
	connect(ui.btnCloseEvent,	SIGNAL(clicked()), this,SLOT(btnCloseEvent_clicked()));
	connect(ui.btnReopenEvent,	SIGNAL(clicked()), this,SLOT(btnReopenEvent_clicked()));
	connect(ui.btnResumeEvent,	SIGNAL(clicked()), this,SLOT(btnResumeEvent_clicked()));	
	connect(ui.btnViewNotes,	SIGNAL(clicked()), this,SLOT(btnViewNotes_clicked()));
	connect(ui.txtEventDescription, SIGNAL(textChanged()), this,SLOT(validateDescription()));

 	if(NOT logSession->canStartEvent())
 	{
 		ui.tabWidget->setTabEnabled(0,false);
 	}
}
void ExploreCase::btnStartEvent_clicked()
{
	if(ui.txtEventNo->text().trimmed()=="" || ui.cmbEventType->currentText()=="") return;

	if(ui.txtEventTitle->text().trimmed()=="") ui.txtEventTitle->setText("--");
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
			eInfo.setEventName(ui.txtEventTitle->text().trimmed());
			eInfo.setEventDescription(ui.txtEventDescription->toPlainText().trimmed());
			eInfo.setEventTypeID(eventTypeID);
			eInfo.setEventEEL("");
			eInfo.setUserID(1);

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
			logSession->setEventName(ui.txtEventTitle->text());
			logSession->setEventDescription(ui.txtEventDescription->toPlainText());
			logSession->setSessionID(sessionID);
			logSession->setCurrentEventType(eventTypeID);
			
			DLCRMain *dlcrs=static_cast<DLCRMain*>(this->parent());

			caseDetails details;
			details.no=logSession->getEventNo();
			details.title=logSession->getEventName();
			details.description=logSession->getEventDescription();
			details.dt=sharedObj->getCurrentDateTime();
			details.location=ui.txtLocation->text();
			details.room=ui.txtRoom->text();
			details.eventType=ui.cmbEventType->currentText();

			dlcrs->ui.activeEditor->setCaseDetails(details);

			QString eventHomePath=sharedObj->getEventHomePath(ui.txtLocation->text(),ui.txtRoom->text(),ui.cmbEventType->currentText(),logSession->getEventNo());
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
			ui.txtEventTitle->clear();
			ui.txtEventDescription->clear();

			this->close();
		}
		else
		{
			loadUserDetails();
			ui.txtSearch->setText(ui.txtEventNo->text());
			ui.tabWidget->setCurrentIndex(1);
// 			ui.tblEventDetails->selectRow(0);
// 			eventDetails_cellClicked(0,1);
// 			if(ui.btnResumeEvent->isEnabled()) btnResumeEvent_clicked();
// 			else SHOW_MESSAGE(tr("Starting Failed"),tr("Closed case!!! Please reopen to start."),tr("Ok"))
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
void ExploreCase::validateDescription()
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
void ExploreCase::cmbRoomName_indexChanged(int index_)
{
	cmbSearchCategory_indexChanged(ui.cmbSearchCategory->currentIndex());
	
	if (ui.cmbRoomName->itemData(index_,Qt::UserRole).toInt()==logSession->getCurrentRoom())
	{
		ui.btnResumeEvent->setVisible(true);
	}
	else
	{
		ui.btnResumeEvent->setVisible(false);
	}
}
void ExploreCase::btnMore_clicked()
{
	if (ui.btnMore->isChecked()!=false)	ui.btnMore->setIcon(icoDown);
	else ui.btnMore->setIcon(icoTop);
}
void ExploreCase::loadUserDetails()
{
	ui.txtSearch->clear();

	loadEventTypes();
	loadLocations();
	
	txtSearch_textChanged(ui.txtSearch->text());

	LocationUtil *lUtil=new LocationUtil(this);
	RoomUtil *rUtil=new RoomUtil(this);

	try
	{
		LocationInfo lInfo=lUtil->loadLocation(logSession->getCurrentLocation(),logSession->getDriverName());
		ui.txtLocation->setText(lInfo.getLocationName());

		RoomInfo rInfo=rUtil->loadRoom(logSession->getCurrentRoom(),logSession->getDriverName());
		ui.txtRoom->setText(rInfo.getRoomName());

		ui.txtUser->setText(logSession->getUserName());
		ui.txtStartDate->setText(sharedObj->getCurrentDateTime().toString("dd/MM/yyyy"));

		int locationIndex=ui.cmbLocationName->findText(lInfo.getLocationName(),Qt::MatchExactly | Qt::MatchCaseSensitive);
		ui.cmbLocationName->setCurrentIndex(locationIndex);

		int roomIndex=ui.cmbRoomName->findText(rInfo.getRoomName(),Qt::MatchExactly | Qt::MatchCaseSensitive);
		ui.cmbRoomName->setCurrentIndex(roomIndex);
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
void ExploreCase::loadEventTypes()
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
bool ExploreCase::isAssigned(int userID_,int roomID_,int eventTypeID_)
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
void ExploreCase::loadLocations()
{
	LocationUtil *util=new LocationUtil(this);

	try
	{
		ui.cmbLocationName->blockSignals(true);
		ui.cmbLocationName->clear();
		ui.cmbLocationName->blockSignals(false);

		QList<int> userLocationList=logSession->getUserLocationIDs();
		for(int i=0;i<userLocationList.size();i++)
		{
			LocationInfo info=util->loadLocation(userLocationList[i],logSession->getDriverName());
			ui.cmbLocationName->insertItem(i,info.getLocationName(),info.getLocationID());
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

	cmbLocationName_indexChanged(ui.cmbLocationName->currentIndex());
}
void ExploreCase::cmbLocationName_indexChanged(int index_)
{
	RoomUtil *util =new RoomUtil(this);

	try
	{
		ui.cmbRoomName->blockSignals(true);
		ui.cmbRoomName->clear();
		ui.cmbRoomName->blockSignals(false);

		int locationID=ui.cmbLocationName->itemData(index_,Qt::UserRole).toInt();
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
					ui.cmbRoomName->insertItem(i,info.getRoomName(),info.getRoomID());
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
void ExploreCase::cmbSearchCategory_indexChanged(int index_)
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
void ExploreCase::txtSearch_textChanged(QString str_)
{
	//for removing rows from table widget
	for (int i=ui.tblEventDetails->rowCount(); i >= 0; --i)
	{
		ui.tblEventDetails->removeRow(i);	
	}
	
	listEvents(str_);
	
	ui.btnCloseEvent->setEnabled(false);
	ui.btnResumeEvent->setEnabled(false);
	ui.btnReopenEvent->setEnabled(false);
	ui.btnViewNotes->setEnabled(false);
}
void ExploreCase::loadAllEvents()
{
	EventUtil *util=new EventUtil(this);

	try
	{
		int roomID=ui.cmbRoomName->itemData(ui.cmbRoomName->currentIndex(),Qt::UserRole).toInt();
		
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
void ExploreCase::loadRescentEvents()
{
	EventUtil *util=new EventUtil(this);

	try
	{
		int roomID=ui.cmbRoomName->itemData(ui.cmbRoomName->currentIndex(),Qt::UserRole).toInt();
		
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

void ExploreCase::listEvents(QString str_)
{
	try
	{
		int k=0;

		foreach(EventInfo info,allEvents)
		{
			QTableWidgetItem *itemEventNo,*itemEventTitle,*itemEventDescription,*itemCreatedDate,*itemFinishedDate,*itemEventType,*itemStaus,*vHItem;

			itemEventNo=new QTableWidgetItem();
			itemEventTitle=new QTableWidgetItem();
			itemEventDescription=new QTableWidgetItem();
			itemCreatedDate=new QTableWidgetItem();
			itemFinishedDate=new QTableWidgetItem();
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
				itemFinishedDate->setText("--");
				itemEventNo->setIcon(icoOpen);
				//vHItem->setIcon(icoOpen);
			}
			else 
			{
				if(logSession->canViewClosedEvent()) itemFinishedDate->setText(info.getFinishedDate().toString("d-M-yyyy"));
				else continue;
				itemEventNo->setIcon(icoClosed);
				//vHItem->setIcon(icoClosed);
			}

			EventTypeUtil *etUtil=new EventTypeUtil(this);
			itemEventType->setText(etUtil->loadEventType(info.getEventTypeID(),logSession->getDriverName()).getEventTypeName());
			itemEventType->setData(Qt::UserRole,info.getEventTypeID());
			delete etUtil;

			itemStaus->setText(info.getEventStatus());

			if(!info.getEventNo().contains(str_)) continue;

			ui.tblEventDetails->insertRow(k);

			//ui.tblEventDetails->setVerticalHeaderItem(k,vHItem);

			ui.tblEventDetails->setItem(k,0,itemEventNo);
			ui.tblEventDetails->setItem(k,1,itemEventTitle);
			ui.tblEventDetails->setItem(k,2,itemEventDescription);
			ui.tblEventDetails->setItem(k,3,itemCreatedDate);
			ui.tblEventDetails->setItem(k,4,itemFinishedDate);
			ui.tblEventDetails->setItem(k,5,itemEventType);
			ui.tblEventDetails->setItem(k,6,itemStaus);

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
void ExploreCase::btnCloseEvent_clicked()
{
	EventUtil *util=new EventUtil(this);

	try
	{
		int eventID=ui.tblEventDetails->item(ui.tblEventDetails->currentRow(),0)->data(Qt::UserRole).toInt();
		
		EventInfo info;
		info.setEventID(eventID);
		info.setFinishedDate(sharedObj->getCurrentDateTime());
		info.setEventStatus("Closed");

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

	cmbRoomName_indexChanged(ui.cmbRoomName->currentIndex());
}
void ExploreCase::btnReopenEvent_clicked()
{
	EventUtil *util=new EventUtil(this);

	try
	{
		int eventID=ui.tblEventDetails->item(ui.tblEventDetails->currentRow(),0)->data(Qt::UserRole).toInt();

		EventInfo info;
		info.setEventID(eventID);
		info.setFinishedDate(sharedObj->getCurrentDateTime());
		info.setEventStatus("Open");

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

	cmbRoomName_indexChanged(ui.cmbRoomName->currentIndex());
}
void ExploreCase::btnResumeEvent_clicked()
{
	SLOT_ENTER

	EventUtil *util=new EventUtil(this);
	SessionUtil *session=new SessionUtil(this);
	AppSettingsUtil *settings=new AppSettingsUtil(this);

	try
	{
			int eventID=ui.tblEventDetails->item(ui.tblEventDetails->currentRow(),0)->data(Qt::UserRole).toInt();
			int eventTypeID=ui.tblEventDetails->item(ui.tblEventDetails->currentRow(),5)->data(Qt::UserRole).toInt();

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
			logSession->setEventNo(ui.tblEventDetails->item(ui.tblEventDetails->currentRow(),0)->text());
			logSession->setEventName(ui.tblEventDetails->item(ui.tblEventDetails->currentRow(),1)->text());
			logSession->setEventDescription(ui.tblEventDetails->item(ui.tblEventDetails->currentRow(),2)->text());
			logSession->setSessionID(sessionID);
			logSession->setCurrentEventType(eventTypeID);

			DLCRMain *dlcrs=static_cast<DLCRMain*>(this->parent());

			caseDetails details;
			details.no=logSession->getEventNo();
			details.title=logSession->getEventName();
			details.description=logSession->getEventDescription();
			details.dt=sharedObj->getCurrentDateTime();
			details.location=ui.cmbLocationName->currentText();
			details.room=ui.cmbRoomName->currentText();
			details.eventType=ui.tblEventDetails->item(ui.tblEventDetails->currentRow(),5)->text();

			dlcrs->ui.activeEditor->setCaseDetails(details);

			QString eventHomePath=sharedObj->getEventHomePath(ui.cmbLocationName->currentText(),ui.cmbRoomName->currentText(),ui.tblEventDetails->item(ui.tblEventDetails->currentRow(),5)->text(),logSession->getEventNo());
			logSession->setEventHomePath(eventHomePath);

//			QString filePath=eventHomePath+"/"+sharedObj->getEventFileName(1,logSession->getEventNo());

//			logSession->setCurrentFilePath(filePath);
			int videoCount=dlcrs->getVideoCount();
			QStringList filePath=sharedObj->getEventFileName(videoCount,logSession->getEventNo());
			QStringList filepaths;
			for (int i=0;i<videoCount;i++)
			{
				filepaths<<eventHomePath+"/"+filePath[i];
			}
			logSession->setCurrentFilePath(filepaths);
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
void ExploreCase::btnViewNotes_clicked()
{
	SLOT_ENTER

	int eventID=ui.tblEventDetails->item(ui.tblEventDetails->currentRow(),0)->data(Qt::UserRole).toInt();
	int roomID=ui.cmbRoomName->itemData(ui.cmbRoomName->currentIndex(),Qt::UserRole).toInt();

	DLCRMain *dlcrs=static_cast<DLCRMain*>(this->parent());

	retrieveDetails details;
	details.no=ui.tblEventDetails->item(ui.tblEventDetails->currentRow(),0)->text();
	details.title=ui.tblEventDetails->item(ui.tblEventDetails->currentRow(),1)->text();
	details.description=ui.tblEventDetails->item(ui.tblEventDetails->currentRow(),2)->text();
	QString strDateTime=ui.tblEventDetails->item(ui.tblEventDetails->currentRow(),3)->text();
	details.dt=QDateTime::fromString(strDateTime,"d-M-yyyy");
	details.eventType=ui.tblEventDetails->item(ui.tblEventDetails->currentRow(),5)->text();
	
	dlcrs->ui.activeRetriever->setEventDetails(details);

	logSession->setRetrievedID(eventID);
	logSession->setRetrievedLocation(ui.cmbLocationName->currentText());
	logSession->setRetrievedRoom(ui.cmbRoomName->currentText());

	dlcrs->retrieveEvent(eventID,roomID);

	this->close();

	SLOT_EXIT
}
void ExploreCase::eventDetails_cellClicked(int row_,int column_)
{
	int roomID=ui.cmbRoomName->itemData(ui.cmbRoomName->currentIndex(),Qt::UserRole).toInt();
	int eventTypeID=ui.tblEventDetails->item(ui.tblEventDetails->currentRow(),5)->data(Qt::UserRole).toInt();

	if(ui.tblEventDetails->item(row_,6)->text()=="Open")
	{
		if(isAssigned(logSession->getUserID(),roomID,eventTypeID))
		{
			if(logSession->canCloseEvent()) ui.btnCloseEvent->setEnabled(true);
			else ui.btnCloseEvent->setEnabled(false);
			if(logSession->canResumeEvent()) ui.btnResumeEvent->setEnabled(true);
			else ui.btnResumeEvent->setEnabled(false);
			ui.btnReopenEvent->setEnabled(false);
			if(logSession->canRetrieveEvent()) ui.btnViewNotes->setEnabled(true);
			else ui.btnViewNotes->setEnabled(false);
		}
		else
		{
			ui.btnCloseEvent->setEnabled(false);
			ui.btnResumeEvent->setEnabled(false);
			ui.btnReopenEvent->setEnabled(false);
			ui.btnViewNotes->setEnabled(false);
		}
	}
	else
	{			
		if(isAssigned(logSession->getUserID(),roomID,eventTypeID))
		{	
			ui.btnCloseEvent->setEnabled(false);
			ui.btnResumeEvent->setEnabled(false);
			if(logSession->canReopenEvent()) ui.btnReopenEvent->setEnabled(true);	
			else ui.btnReopenEvent->setEnabled(false);	
			if(logSession->canRetrieveEvent()) ui.btnViewNotes->setEnabled(true);
			else ui.btnViewNotes->setEnabled(false);
		}
		else
		{
			ui.btnCloseEvent->setEnabled(false);
			ui.btnResumeEvent->setEnabled(false);
			ui.btnReopenEvent->setEnabled(false);
			ui.btnViewNotes->setEnabled(false);
		}
	}
}
void ExploreCase::tblEventDetails_cellChanged(int row_,int column_)
{
	if(this->layoutDirection()==Qt::RightToLeft)
	{
		ui.tblEventDetails->resize(ui.tblEventDetails->width()+1,ui.tblEventDetails->height());
		ui.tblEventDetails->resize(ui.tblEventDetails->width()-1,ui.tblEventDetails->height());
	}

	int roomID=ui.cmbRoomName->itemData(ui.cmbRoomName->currentIndex(),Qt::UserRole).toInt();
	if(ui.tblEventDetails->item(ui.tblEventDetails->currentRow(),5)==NULL) return;
	int eventTypeID=ui.tblEventDetails->item(ui.tblEventDetails->currentRow(),5)->data(Qt::UserRole).toInt();

	if(ui.tblEventDetails->item(row_,6)->text()=="Open")
	{
		if(isAssigned(logSession->getUserID(),roomID,eventTypeID))
		{
			if(logSession->canCloseEvent()) ui.btnCloseEvent->setEnabled(true);
			else ui.btnCloseEvent->setEnabled(false);
			if(logSession->canResumeEvent()) ui.btnResumeEvent->setEnabled(true);
			else ui.btnResumeEvent->setEnabled(false);
			ui.btnReopenEvent->setEnabled(false);
			if(logSession->canRetrieveEvent()) ui.btnViewNotes->setEnabled(true);
			else ui.btnViewNotes->setEnabled(false);
		}
		else
		{
			ui.btnCloseEvent->setEnabled(false);
			ui.btnResumeEvent->setEnabled(false);
			ui.btnReopenEvent->setEnabled(false);
			ui.btnViewNotes->setEnabled(false);
		}
	}
	else
	{			
		if(isAssigned(logSession->getUserID(),roomID,eventTypeID))
		{	
			ui.btnCloseEvent->setEnabled(false);
			ui.btnResumeEvent->setEnabled(false);
			if(logSession->canReopenEvent()) ui.btnReopenEvent->setEnabled(true);	
			else ui.btnReopenEvent->setEnabled(false);	
			if(logSession->canRetrieveEvent()) ui.btnViewNotes->setEnabled(true);
			else ui.btnViewNotes->setEnabled(false);
		}
		else
		{
			ui.btnCloseEvent->setEnabled(false);
			ui.btnResumeEvent->setEnabled(false);
			ui.btnReopenEvent->setEnabled(false);
			ui.btnViewNotes->setEnabled(false);
		}
	}
}
ExploreCase::~ExploreCase()
{

}
