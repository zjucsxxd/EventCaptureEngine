/* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "advancedsearch.h"

#include <QString >
#include <QSqlQuery>
#include <QSqlRecord>

#include "dlcrshared.h"
#include "eventutil.h"
#include "eventtypeutil.h"
#include "eventnoteutil.h"
#include "locationutil.h"
#include "roomutil.h"
#include "roomeventtypeutil.h"
#include "userroomeventtypeutil.h"
#include "appsettingsutil.h"
#include "dlcrmain.h"
#include "sessionutil.h"
#include "appsettingsutil.h"

AdvancedSearch::AdvancedSearch(QWidget *parent)
: QDialog(parent)
{
	ui.setupUi(this);

	icoOpen=QIcon(QPixmap(":/Recorder/Resources/webcamera.png"));
	icoClosed=QIcon(QPixmap(":/Recorder/Resources/closed.png"));

	logSession=LoginUserSession::session();
	sharedObj=new DLCRShared();

	ui.tblCases->setColumnWidth(1,175);

	connect(ui.btnNewSearch,SIGNAL(clicked()),this, SLOT(btnNewsearch_click()));
	connect(ui.btnSearch,SIGNAL(clicked()),this, SLOT(btnSearch_click()));
	connect(ui.cmbDate,SIGNAL(currentIndexChanged (int)),this, SLOT(cmbo_click(int)));
	connect(ui.tblCases,SIGNAL(cellClicked(int,int)),this, SLOT(tblCellClicked(int,int)));
	connect(ui.tblCases,SIGNAL(currentCellChanged(int,int,int,int)),this, SLOT(tblCellChanged(int,int)));
	connect(ui.btnCloseEvent,	SIGNAL(clicked()), this,SLOT(btnCloseEvent_clicked()));
	connect(ui.btnReopenEvent,	SIGNAL(clicked()), this,SLOT(btnReopenEvent_clicked()));
	connect(ui.btnResumeEvent,	SIGNAL(clicked()), this,SLOT(btnResumeEvent_clicked()));	
	connect(ui.btnViewNotes,	SIGNAL(clicked()), this,SLOT(btnViewNotes_clicked()));

	ui.dateEditOne->setDate(QDate::currentDate());
	ui.dateEditSecond->setDate(QDate::currentDate());
}
void AdvancedSearch::cmbo_click(int index)
{
	if(index==0)
	{
		ui.dateEditOne->setEnabled(true);
		ui.dateEditSecond->setEnabled(false);
	}
	else if(index==1)
	{
		ui.dateEditOne->setEnabled(true);
		ui.dateEditSecond->setEnabled(false);
	}
	else if(index==2)
	{
		ui.dateEditOne->setEnabled(true);
		ui.dateEditSecond->setEnabled(true);
	}
	else
	{
		ui.dateEditOne->setEnabled(false);
		ui.dateEditSecond->setEnabled(false);
	}	
}
void AdvancedSearch::tblCellChanged(int row,int col)
{
	if(this->layoutDirection()==Qt::RightToLeft)
	{
		ui.tblCases->resize(ui.tblCases->width()+1,ui.tblCases->height());
		ui.tblCases->resize(ui.tblCases->width()-1,ui.tblCases->height());
	}

	if (ui.tblCases->item(row,0)==NULL)
	{
		return;
	}

	EventUtil *util=new EventUtil(this);
	EventTypeUtil *t=new EventTypeUtil(this);
	EventNoteUtil *e=new EventNoteUtil(this);
	RoomUtil *r=new RoomUtil(this);
	LocationUtil *l=new LocationUtil(this);

	try
	{
		int EventId = ui.tblCases->item(row,0)->data(Qt::UserRole).toInt();
		
		EventTypeInfo infoevent;
		EventInfo info;
		EventNoteInfo infonote;
		RoomInfo inforoom;
		LocationInfo infoloc;
		info=util->loadEvent(EventId,logSession->getDriverName());
		infoevent=t->loadEventType(info.getEventTypeID(),logSession->getDriverName());
		infonote=e->getEventDetailsByNotes(EventId);
		inforoom=r->loadRoom(infonote.getRoomID(),logSession->getDriverName());
		infoloc=l->loadLocation(inforoom.getLocationID(),logSession->getDriverName());
		QTableWidgetItem *itemEventTitle;
		QTableWidgetItem *itemEventStatus;
		QTableWidgetItem *itemEventDate;
		QTableWidgetItem *itemEventType;
		QTableWidgetItem *itemRoomName;
		QTableWidgetItem *itemLocationName;
		itemEventTitle=new QTableWidgetItem(info.getEventName());
		itemEventStatus=new QTableWidgetItem(info.getEventStatus());
		itemEventDate=new QTableWidgetItem(info.getCreatedDate().toString("dd/MM/yyyy hh:mm:ss AP"));
		itemEventType=new QTableWidgetItem(infoevent.getEventTypeName());
		itemEventType->setData(Qt::UserRole,infoevent.getEventTypeID());
		itemRoomName=new QTableWidgetItem(inforoom.getRoomName());
		itemRoomName->setData(Qt::UserRole,inforoom.getRoomID());
		itemLocationName=new QTableWidgetItem(infoloc.getLocationName());
		itemLocationName->setData(Qt::UserRole,infoloc.getLocationID());
		ui.tblDetails->setItem(0,0,itemEventTitle);
		ui.tblDetails->setItem(0,5,itemEventStatus);
		ui.tblDetails->setItem(0,4,itemEventDate);
		ui.tblDetails->setItem(0,3,itemEventType);
		ui.tblDetails->setItem(0,2,itemRoomName);
		ui.tblDetails->setItem(0,1,itemLocationName);

		if(ui.tblDetails->item(0,5)->text()=="Open")
		{
			if(isAssigned(logSession->getUserID(),infonote.getRoomID(),info.getEventTypeID()))
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
			if(isAssigned(logSession->getUserID(),infonote.getRoomID(),info.getEventTypeID()))
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
	delete e;
	delete t;
	delete r;
	delete l;
}
void AdvancedSearch::tblCellClicked(int row,int col)
{
	if (ui.tblCases->item(row,0)==NULL)
	{
		return;
	}

	EventUtil *util=new EventUtil(this);
	EventTypeUtil *t=new EventTypeUtil(this);
	EventNoteUtil *e=new EventNoteUtil(this);
	RoomUtil *r=new RoomUtil(this);
	LocationUtil *l=new LocationUtil(this);

	try
	{
		int EventId = ui.tblCases->item(row,0)->data(Qt::UserRole).toInt();
		
		EventTypeInfo infoevent;
		EventInfo info;
		EventNoteInfo infonote;
		RoomInfo inforoom;
		LocationInfo infoloc;
		info=util->loadEvent(EventId,logSession->getDriverName());
		infoevent=t->loadEventType(info.getEventTypeID(),logSession->getDriverName());
		infonote=e->getEventDetailsByNotes(EventId);
		inforoom=r->loadRoom(infonote.getRoomID(),logSession->getDriverName());
		infoloc=l->loadLocation(inforoom.getLocationID(),logSession->getDriverName());
		QTableWidgetItem *itemEventTitle;
		QTableWidgetItem *itemEventStatus;
		QTableWidgetItem *itemEventDate;
		QTableWidgetItem *itemEventType;
		QTableWidgetItem *itemRoomName;
		QTableWidgetItem *itemLocationName;
		itemEventTitle=new QTableWidgetItem(info.getEventName());
		itemEventStatus=new QTableWidgetItem(info.getEventStatus());
		itemEventDate=new QTableWidgetItem(info.getCreatedDate().toString("dd/MM/yyyy hh:mm:ss AP"));
		itemEventType=new QTableWidgetItem(infoevent.getEventTypeName());
		itemEventType->setData(Qt::UserRole,infoevent.getEventTypeID());
		itemRoomName=new QTableWidgetItem(inforoom.getRoomName());
		itemRoomName->setData(Qt::UserRole,inforoom.getRoomID());
		itemLocationName=new QTableWidgetItem(infoloc.getLocationName());
		itemLocationName->setData(Qt::UserRole,infoloc.getLocationID());
		ui.tblDetails->setItem(0,0,itemEventTitle);
		ui.tblDetails->setItem(0,5,itemEventStatus);
		ui.tblDetails->setItem(0,4,itemEventDate);
		ui.tblDetails->setItem(0,3,itemEventType);
		ui.tblDetails->setItem(0,2,itemRoomName);
		ui.tblDetails->setItem(0,1,itemLocationName);

		if(ui.tblDetails->item(0,5)->text()=="Open")
		{
			if(isAssigned(logSession->getUserID(),infonote.getRoomID(),info.getEventTypeID()))
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
			if(isAssigned(logSession->getUserID(),infonote.getRoomID(),info.getEventTypeID()))
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
	delete e;
	delete t;
	delete r;
	delete l;
}
bool AdvancedSearch::isAssigned(int userID_,int roomID_,int eventTypeID_)
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
void AdvancedSearch::btnNewsearch_click()
{
	ui.txtEventNo->clear();
	ui.txtTitle->clear();       
	ui.txtDescription->clear();
	ui.txtLocation->clear();    
	ui.txtRoom->clear();
	ui.txtEventType->clear();   
	ui.txtSpeaker->clear();
	ui.txtLognote->clear();
	ui.tblCases->clearContents();
	ui.tblDetails->clearContents();

	//for removing rows from table widget
	for (int i=ui.tblCases->rowCount(); i >= 0; --i)
	{
		ui.tblCases->removeRow(i);	
	}

	ui.btnCloseEvent->setEnabled(false);
	ui.btnResumeEvent->setEnabled(false);
	ui.btnReopenEvent->setEnabled(false);
	ui.btnViewNotes->setEnabled(false);
}

void AdvancedSearch::btnSearch_click()
{
	try
	{
		searchByCatagory(ui.txtEventNo->text(),     
			ui.cmbDate->currentIndex(),
			ui.dateEditOne->dateTime(), 
			ui.dateEditSecond->dateTime(),
			ui.txtTitle->text(),       
			ui.txtDescription->text(),
			ui.txtLocation->text(),    
			ui.txtRoom->text(),
			ui.txtEventType->text(),   
			ui.txtSpeaker->text(),
			ui.cmbNote->currentIndex(),
			ui.txtLognote->text());

	}
	catch(DBExeption e)
	{
		if(e==ExecutionFailed)
		{
			PRINT_ERROR("insertion failed"); 
		}
		if(e==UnableToConnect)
		{
			PRINT_ERROR("Unable to connect to db"); 
		}  
	}
	catch(...)
	{
		PRINT_ERROR("UNKnown ERROR");
	}

/*	ui.tblDetails->clearContents();*/

	ui.btnCloseEvent->setEnabled(false);
	ui.btnResumeEvent->setEnabled(false);
	ui.btnReopenEvent->setEnabled(false);
	ui.btnViewNotes->setEnabled(false);
}


void AdvancedSearch::searchByCatagory(QString eventNo,
									  int dateIndex,
									  QDateTime afterDate_,
									  QDateTime beforeDate_,
									  QString titleTxt,
									  QString descriptionTxt,
									  QString locationTxt,
									  QString roomTxt,
									  QString eventTxt,
									  QString speakerTxt ,
									  int noteIndex,
									  QString noteTxt)
{

	CONNECT_TO_DB(cn,"Inst_AdvancedSearch_SearchByCatagory");

	QSqlQuery sql(cn); 

	EventUtil *util=new EventUtil(this);
	EventInfo info;
	QString eventSpeaker,eventNote,noteQuery,dateQuery;
	int eventID;
	QString eventtime;
	if (noteIndex==0)
	{
		noteQuery="'"+noteTxt+"%'";
	}
	if (noteIndex==1)
	{
		noteQuery="'%"+noteTxt+"'";
	}
	if (noteIndex==2)
	{
		noteQuery="'%"+noteTxt+"%'";
	}
	if (dateIndex==0)
	{
		dateQuery= "> '" + afterDate_.toString("MM/dd/yyyy hh:mm:ss AP")+ "'";
	}
	if (dateIndex==1)
	{
		dateQuery="<'" + afterDate_.toString("MM/dd/yyyy hh:mm:ss AP")+ "'";
	}
	if (dateIndex==2)
	{
		
		//beforeDate_.addDays(1);
		if (afterDate_>=beforeDate_)
		{
			qSwap(afterDate_,beforeDate_);
		}

		beforeDate_= beforeDate_.addDays(1);
		dateQuery= "> '" + afterDate_.toString("MM/dd/yyyy hh:mm:ss AP") + "'AND EventTime <'" + beforeDate_.toString("MM/dd/yyyy hh:mm:ss AP")+ "'";	
	}
	if (dateIndex==3)
	{
		dateQuery=">'1/1/1800 12:00:00 AM'";

	}

	QString qry="select * from tbl_EventNotes_T where Visibility=1 and EventSpeaker like '%"+
		speakerTxt+"%' AND EventNote like "+noteQuery+" AND EventTime "+dateQuery+" AND EventID in (select EventID from"
		" tbl_EventDetails_M where EventNo like '%"+eventNo+"%' AND EventName like '%"
		+titleTxt+"%' AND EventDescription like '%"+descriptionTxt+"%') AND "
		"RoomID in(select RoomID from tbl_RoomDetails_M where RoomName Like '%"+
		roomTxt+"%' AND LocationID in (select LocationID from tbl_LocationDetails_M "
		"where LocationName like '%"+locationTxt+"%')) AND EventID in(select EventID from "
		"tbl_EventDetails_M where EventTypeID in(select EventTypeID from tbl_EventTypeDetails_M "
		"where EventTypeName like'%"+eventTxt+"%'))";

	sql.prepare(qry);
	QTableWidgetItem *itemEventNo;
	QTableWidgetItem *itemEventTime;
	QTableWidgetItem *itemSpeaker;
	QTableWidgetItem *itemNotes;

	ui.tblCases->clear();
	ui.tblCases->setRowCount(0);
	ui.tblCases->setHorizontalHeaderLabels(QStringList()<<tr("CaseNo")<<tr("Date & Time")<<tr("Speaker")<<tr("LogNotes"));

	QTableWidgetItem *openedItem=new QTableWidgetItem(info.getEventNo());
	openedItem->setIcon(icoOpen);

	QTableWidgetItem *closeItem =new QTableWidgetItem();
	closeItem->setIcon(icoClosed);

	if(sql.exec())
	{
		for(int i=0;sql.next();i++)
		{
			eventSpeaker=sql.value(sql.record().indexOf("EventSpeaker")).toString();
			eventNote=sql.value(sql.record().indexOf("EventNote")).toString();
			eventID=sql.value(sql.record().indexOf("EventID")).toInt();
			QDateTime tempTime=sql.value(sql.record().indexOf("EventTime")).toDateTime();
			eventtime=tempTime.toString("dd/MM/yyyy hh:mm:ss AP");

			info=util->loadEvent(eventID,logSession->getDriverName());
			
			if(info.getEventStatus()!="Open") itemEventNo=closeItem->clone();
			else itemEventNo=openedItem->clone();

			itemEventNo->setText(info.getEventNo());
			itemEventTime=new QTableWidgetItem(eventtime);
			itemEventTime->setData(Qt::UserRole,sql.value(sql.record().indexOf("EventID")));
			itemSpeaker=new QTableWidgetItem(eventSpeaker);
			itemNotes=new QTableWidgetItem(eventNote);
			ui.tblCases->insertRow(i);
			itemEventNo->setData(Qt::UserRole,eventID);
			int k=itemEventNo->data(Qt::UserRole).toInt();

			ui.tblCases->setItem(i,0,itemEventNo);
			ui.tblCases->setItem(i,1,itemEventTime);
			ui.tblCases->setItem(i,2,itemSpeaker);
			ui.tblCases->setItem(i,3,itemNotes);
		}
	}
	else
	{
		throw ExecutionFailed;
	}		

	CLOSE_CONNECTION(sql,cn,"Inst_AdvancedSearch_SearchByCatagory");
}
void AdvancedSearch::btnCloseEvent_clicked()
{
	EventUtil *util=new EventUtil(this);

	try
	{
		int eventID=ui.tblCases->item(ui.tblCases->currentRow(),0)->data(Qt::UserRole).toInt();

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
	
	btnSearch_click();
}
void AdvancedSearch::btnReopenEvent_clicked()
{
	EventUtil *util=new EventUtil(this);

	try
	{
		int eventID=ui.tblCases->item(ui.tblCases->currentRow(),0)->data(Qt::UserRole).toInt();

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

	btnSearch_click();
}
void AdvancedSearch::btnResumeEvent_clicked()
{
	EventUtil *util=new EventUtil(this);
	SessionUtil *session=new SessionUtil(this);
	AppSettingsUtil *settings=new AppSettingsUtil(this);

	try
	{
		int eventID=ui.tblCases->item(ui.tblCases->currentRow(),0)->data(Qt::UserRole).toInt();
		int eventTypeID=ui.tblDetails->item(0,3)->data(Qt::UserRole).toInt();

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

		EventUtil *eUtil=new EventUtil(this);
		EventInfo eInfo=eUtil->loadEvent(eventID,logSession->getDriverName());

		logSession->setEventNo(eInfo.getEventNo());
		logSession->setEventName(eInfo.getEventName());
		logSession->setEventDescription(eInfo.getEventDescription());
		logSession->setSessionID(sessionID);
		logSession->setCurrentEventType(eventTypeID);

		delete eUtil;

//		DLCRMain *dlcrs=static_cast<DLCRMain*>(this->parent());

// 		caseDetails details;
// 		details.no=logSession->getEventNo();
// 		details.title=logSession->getEventName();
// 		details.description=logSession->getEventDescription();
// 		details.dt=sharedObj->getCurrentDateTime();
// 		details.location=ui.tblDetails->item(0,1)->text();
// 		details.room=ui.tblDetails->item(0,2)->text();
// 		details.eventType=ui.tblDetails->item(0,3)->text();
// 
// 		dlcrs->ui.activeEditor->setCaseDetails(details);
// 
// 		QString eventHomePath=sharedObj->getEventHomePath(details.location,details.room,details.eventType,details.no);
// 		logSession->setEventHomePath(eventHomePath);
// 
// //		QString filePath=eventHomePath+"/"+sharedObj->getEventFileName(1,logSession->getEventNo());
// 
// //		logSession->setCurrentFilePath(filePath);
// 		int videoCount=dlcrs->getVideoCount();
// 		QStringList filePath=sharedObj->getEventFileName(videoCount,logSession->getEventNo());
// 		QStringList filepaths;
// 		for (int i=0;i<videoCount;i++)
// 		{
// 			filepaths<<eventHomePath+"/"+filePath[i];
// 		}
// 		logSession->setCurrentFilePath(filepaths);
// 		dlcrs->startEvent();
// 
// 		this->close();		

		DLCRMain *dlcrs=static_cast<DLCRMain*>(this->parent());

		caseDetails details;
		details.no=logSession->getEventNo();
		details.title=logSession->getEventName();
		details.description=logSession->getEventDescription();
		details.dt=sharedObj->getCurrentDateTime();
		details.location=ui.tblDetails->item(0,1)->text();
		details.room=ui.tblDetails->item(0,2)->text();
		details.eventType=ui.tblDetails->item(0,3)->text();

		dlcrs->ui.activeEditor->setCaseDetails(details);

		QString eventHomePath=sharedObj->getEventHomePath(details.location,details.room,details.eventType,details.no);
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
}
void AdvancedSearch::btnViewNotes_clicked()
{
	int eventID=ui.tblCases->item(ui.tblCases->currentRow(),0)->data(Qt::UserRole).toInt();
	int roomID=ui.tblDetails->item(0,2)->data(Qt::UserRole).toInt();

	DLCRMain *dlcrs=static_cast<DLCRMain*>(this->parent());

	EventUtil *eUtil=new EventUtil(this);
	EventInfo eInfo=eUtil->loadEvent(eventID,logSession->getDriverName());

	retrieveDetails details;
	details.no=eInfo.getEventNo();
	details.title=eInfo.getEventName();
	details.description=eInfo.getEventDescription();
	details.dt=eInfo.getCreatedDate();
	details.eventType=ui.tblDetails->item(0,3)->text();

	delete eUtil;

	dlcrs->ui.activeRetriever->setEventDetails(details);

	logSession->setRetrievedID(eventID);
	logSession->setRetrievedLocation(ui.tblDetails->item(0,1)->text());
	logSession->setRetrievedRoom(ui.tblDetails->item(0,2)->text());

	dlcrs->retrieveEvent(eventID,roomID);

	this->close();
}
AdvancedSearch::~AdvancedSearch()
{

}
