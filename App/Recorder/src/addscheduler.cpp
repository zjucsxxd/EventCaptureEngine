#include "addscheduler.h"
#include "roomeventtypeutil.h"
#include "eventtypeutil.h"
#include "userroomeventtypeutil.h"
#include "schedulerutil.h"
#include "eventutil.h"
#include "locationutil.h"
#include "roomutil.h"

AddScheduler::AddScheduler(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	logSession=LoginUserSession::session();

	ui.weeklyFrame->setEnabled(false);

	connect(ui.cmbAction,	SIGNAL(currentIndexChanged(int)), this,SLOT(cmbAction_indexChanged(int)));
	connect(ui.cmbRepeat,	SIGNAL(currentIndexChanged(int)), this,SLOT(cmbRepeat_indexChanged(int)));
	connect(ui.cmbEventNo,	SIGNAL(currentIndexChanged(int)), this,SLOT(cmbEventNo_indexChanged(int)));
	connect(ui.cmbLocation, SIGNAL(currentIndexChanged(int)), this,SLOT(cmbLocationName_indexChanged(int)));
	connect(ui.cmbRoom,		SIGNAL(currentIndexChanged(int)), this,SLOT(cmbRoomName_indexChanged(int)));
	connect(ui.btnOk,		SIGNAL(clicked()),				  this,SLOT(btnOk_Clicked()));
	loadUserDetails();
}

void AddScheduler::cmbAction_indexChanged(int index_)
{
	if (index_==0)
	{
		ui.cmbEventNo->clear();
		ui.cmbEventType->setEnabled(true);
		ui.txtName->clear();
		ui.txtDescription->clear();
		loadEventTypes();
		ui.txtName->setReadOnly(false);
		ui.txtDescription->setReadOnly(false);

	}
	if (index_==1)
	{
		loadLocations();
		ui.cmbEventType->setEnabled(false);
		ui.txtName->setReadOnly(true);
		ui.txtDescription->setReadOnly(true);
	}
	if (index_==2)
	{
		loadLocations();
		ui.cmbEventType->setEnabled(false);
		ui.txtName->setReadOnly(true);
		ui.txtDescription->setReadOnly(true);
	}
}
void AddScheduler::cmbRepeat_indexChanged(int index_)
{
	if (index_==0)
	{
		ui.dateEdit->setEnabled(true);
		ui.timeEdit->setEnabled(true);
		ui.weeklyFrame->setEnabled(false);
	}
	if (index_==1)
	{
		ui.weeklyFrame->setEnabled(false);
		ui.dateEdit->setEnabled(false);
		ui.timeEdit->setEnabled(true);
		
	}
	if (index_==2)
	{
		ui.dateEdit->setEnabled(false);
		ui.timeEdit->setEnabled(true);
		ui.weeklyFrame->setEnabled(true);
	}
}
void AddScheduler::cmbEventNo_indexChanged(int index_)
{

	EventUtil *util=new EventUtil(this);
	EventTypeUtil *etUtil=new EventTypeUtil(this);
	int eventID=ui.cmbEventNo->itemData(index_,Qt::UserRole).toInt();

	try
	{
		EventInfo einfo =util->loadEvent(eventID,logSession->getDriverName());
		int eventTypeID=einfo.getEventTypeID();
		QString eventName=einfo.getEventName();
		QString description=einfo.getEventDescription();
		EventTypeInfo etInfo=etUtil->loadEventType(eventTypeID,logSession->getDriverName());
		QString eventTypeName=etInfo.getEventTypeName();

		ui.cmbEventType->addItem(eventTypeName,etInfo.getEventTypeID());
		ui.txtName->setText(eventName);
		ui.txtDescription->setText(description);
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


}

void AddScheduler::loadEventTypes()
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
bool AddScheduler::isAssigned(int userID_,int roomID_,int eventTypeID_)
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

	delete ureUtil;

	return assignedStatus;

}
void AddScheduler::btnOk_Clicked()
{
	Scheduler *e=static_cast<Scheduler*>(this->parent());

	EventUtil *util=new EventUtil(this);
	int RoomID;
	QString eventName;
	eventName==ui.txtName->text().trimmed();
	RoomID=ui.cmbRoom->itemData(ui.cmbRoom->currentIndex(),Qt::UserRole).toInt();
	
	if(ui.cmbAction->currentIndex()==0)
	{
		int eventID;
		//e->loadschedulersinfo(ui.cmbAction->currentText(),ui.cmbRepeat->currentText(),ui.dateEdit->text(),ui.timeEdit->text(),ui.cmbEventNo->currentText(),ui.txtName->text(),ui.cmbEventType->currentText());
		SchedulerUtil *schduler=new SchedulerUtil(this);
		try
		{
			int eventTypeID=ui.cmbEventType->itemData(ui.cmbEventType->currentIndex(),Qt::UserRole).toInt();

			if (util->existenceStatus(ui.cmbEventNo->currentText().trimmed(),eventTypeID,logSession->getUserID(),logSession->getDriverName())!=true)
			{
				
				EventInfo eInfo;

				eInfo.setEventNo(ui.cmbEventNo->currentText().trimmed());

				if (eventName=="")
				{
					eInfo.setEventName("--");
				}
				else
				{
					eInfo.setEventName(ui.txtName->text().trimmed());
				}
				
				eInfo.setEventDescription(ui.txtDescription->text().trimmed());
				eInfo.setEventTypeID(eventTypeID);
				eInfo.setEventEEL("");
				eInfo.setUserID(logSession->getUserID());

				eventID=util->createEvent(eInfo,logSession->getDriverName());
			}
			else
			{
				return;
			}

			SchedulerInfo sinfo;

			if (ui.cmbRepeat->currentIndex()==0)
			{
				sinfo.setScheduleDate(ui.dateEdit->text());
			}
			if (ui.cmbRepeat->currentIndex()==1)
			{
				sinfo.setScheduleDate("EveryDay");
			}
			if (ui.cmbRepeat->currentIndex()==2)
			{
				sinfo.setScheduleDate("Weekly");
			}
			sinfo.setScheduleTime(ui.timeEdit->text());
			sinfo.setDuration((ui.cmbDuration->currentText()).toInt());
			sinfo.setSchedulerAction(ui.cmbAction->currentText());
			sinfo.setSchedulerRepeat(ui.cmbRepeat->currentText());
			sinfo.setEventNo(ui.cmbEventNo->currentText());
			if (eventName=="")
			{
				sinfo.setEventName("--");
			}
			else
			{
				sinfo.setEventName(ui.txtName->text().trimmed());
			}

			sinfo.setEventDescription(ui.txtDescription->text());
			sinfo.setEventType(ui.cmbEventType->currentText());
//			sinfo.setRoomID(RoomID);
			sinfo.setEventID(eventID);
			sinfo.setSchedulerStatus(1);

			if (ui.cmbRepeat->currentIndex()==2)
			{
				if (ui.chkSunday->checkState()==Qt::Checked)
				{
					sinfo.setSunday(1);
				}
				else
				{
					sinfo.setSunday(0);

				}
				if (ui.chkMonday->checkState()==Qt::Checked)
				{

					sinfo.setMonday(1);
				}
				else
				{
					sinfo.setMonday(0);

				}
				if (ui.chkTuesday->checkState()==Qt::Checked)
				{

					sinfo.setTuesday(1);

				}
				else
				{
					sinfo.setTuesday(0);

				}
				if (ui.chkWenesday->checkState()==Qt::Checked)
				{
					sinfo.setWenesday(1);
				}
				else
				{
					sinfo.setWenesday(0);

				}
				if (ui.chkThursday->checkState()==Qt::Checked)
				{
					sinfo.setThursday(1);
				}
				else
				{
					sinfo.setThursday(0);

				}
				if (ui.chkFriday->checkState()==Qt::Checked)
				{

					sinfo.setFriday(1);
				}
				else
				{
					sinfo.setFriday(0);

				}
				if (ui.chkSaturday->checkState()==Qt::Checked)
				{
					sinfo.setSaturday(1);
				}
				else
				{
					sinfo.setSaturday(0);

				}
			}
			else
			{
				sinfo.setSunday(0);
				sinfo.setMonday(0);
				sinfo.setTuesday(0);
				sinfo.setWenesday(0);
				sinfo.setThursday(0);
				sinfo.setFriday(0);
				sinfo.setSaturday(0);

			}
			schduler->insertOrUpdateScheduler(sinfo,logSession->getDriverName());
			e->loadschedulersinfo(ui.cmbAction->currentText(),ui.cmbRepeat->currentText(),ui.dateEdit->text(),ui.timeEdit->text(),ui.cmbEventNo->currentText(),ui.txtName->text(),ui.cmbEventType->currentText());

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
	}
	else
	{
		//e->loadschedulersinfo(ui.cmbAction->currentText(),ui.cmbRepeat->currentText(),ui.dateEdit->text(),ui.timeEdit->text(),ui.cmbEventNo->currentText(),ui.txtName->text(),ui.cmbEventType->currentText());
		EventUtil *eutil=new EventUtil(this);
		SchedulerUtil *schduler=new SchedulerUtil(this);
		
		try
		{
			QList<EventInfo> EventIDList;
			int EventID;
			EventID=ui.cmbEventNo->itemData(ui.cmbEventNo->currentIndex(),Qt::UserRole).toInt();

			SchedulerInfo sinfo;

			if (ui.cmbRepeat->currentIndex()==0)
			{
				sinfo.setScheduleDate(ui.dateEdit->text());
			}
			if (ui.cmbRepeat->currentIndex()==1)
			{
				sinfo.setScheduleDate("EveryDay");
			}
			if (ui.cmbRepeat->currentIndex()==2)
			{
				sinfo.setScheduleDate("Weekly");
			}

			sinfo.setScheduleTime(ui.timeEdit->text());
			sinfo.setDuration((ui.cmbDuration->currentText()).toInt());
			sinfo.setSchedulerAction(ui.cmbAction->currentText());
			sinfo.setSchedulerRepeat(ui.cmbRepeat->currentText());
			sinfo.setEventNo(ui.cmbEventNo->currentText());
			if (eventName=="")
			{
				sinfo.setEventName("--");
			}
			else
			{
				sinfo.setEventName(ui.txtName->text().trimmed());
			}
			sinfo.setEventDescription(ui.txtDescription->text());
			sinfo.setEventType(ui.cmbEventType->currentText());
			sinfo.setRoomID(RoomID);
			sinfo.setEventID(EventID);
			sinfo.setSchedulerStatus(1);
			
			if (ui.cmbRepeat->currentIndex()==2)
			{
				if (ui.chkSunday->checkState()==Qt::Checked)
				{
					sinfo.setSunday(1);
				}
				else
				{
					sinfo.setSunday(0);
				}
				if (ui.chkMonday->checkState()==Qt::Checked)
				{
					sinfo.setMonday(1);
				}
				else
				{
					sinfo.setMonday(0);
				}
				if (ui.chkTuesday->checkState()==Qt::Checked)
				{
					sinfo.setTuesday(1);
				}
				else
				{
					sinfo.setTuesday(0);
				}
				if (ui.chkWenesday->checkState()==Qt::Checked)
				{
					sinfo.setWenesday(1);
				}
				else
				{
					sinfo.setWenesday(0);
				}
				if (ui.chkThursday->checkState()==Qt::Checked)
				{
					sinfo.setThursday(1);
				}
				else
				{
					sinfo.setThursday(0);
				}
				if (ui.chkFriday->checkState()==Qt::Checked)
				{	
					sinfo.setFriday(1);
				}
				else
				{
					sinfo.setFriday(0);
				}
				if (ui.chkSaturday->checkState()==Qt::Checked)
				{
					sinfo.setSaturday(1);
				}
				else
				{
					sinfo.setSaturday(0);
				}
				
			}
			else
			{
				sinfo.setSunday(0);
				sinfo.setMonday(0);
				sinfo.setTuesday(0);
				sinfo.setWenesday(0);
				sinfo.setThursday(0);
				sinfo.setFriday(0);
				sinfo.setSaturday(0);
			}

			schduler->insertOrUpdateScheduler(sinfo,logSession->getDriverName());
			e->loadschedulersinfo(ui.cmbAction->currentText(),ui.cmbRepeat->currentText(),ui.dateEdit->text(),ui.timeEdit->text(),ui.cmbEventNo->currentText(),ui.txtName->text(),ui.cmbEventType->currentText());
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
	}
}
void AddScheduler::loadLocations()
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

	cmbLocationName_indexChanged(ui.cmbLocation->currentIndex());
}
void AddScheduler::cmbLocationName_indexChanged(int index_)
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
	if (ui.cmbAction->currentIndex()==0)
	{
		ui.txtName->clear();
		ui.txtDescription->clear();
		ui.cmbEventNo->clear();
	}
	delete util;
}
void AddScheduler::cmbRoomName_indexChanged(int index_)
{
	loadAllEvents();
	listEvents();

	if (ui.cmbAction->currentIndex()==0)
	{
		ui.txtName->clear();
		ui.txtDescription->clear();
		ui.cmbEventNo->clear();
	}
}
void AddScheduler::loadAllEvents()
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
void AddScheduler::listEvents()
{
	try
	{
		ui.cmbEventNo->blockSignals(true);
		ui.cmbEventNo->clear();
		ui.cmbEventNo->blockSignals(false);

		int k=0;

		foreach(EventInfo info,allEvents)
		{
			ui.cmbEventNo->insertItem(k,info.getEventNo(),info.getEventID());
			
			k++;
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
}

void AddScheduler::loadUserDetails()
{

	loadLocations();
	loadEventTypes();

	if (ui.cmbAction->currentIndex()==0)
	{
		ui.cmbEventType->setEnabled(true);
		ui.cmbEventNo->clear();
	}
	else
	{
		ui.cmbEventType->setEnabled(false);
	}

}
AddScheduler::~AddScheduler()
{

}
