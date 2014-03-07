/*Owner & Copyrights: Vance King Saxbe. A.*//* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "importnotes.h"
#include "dlcrmain.h"
#include "locationutil.h"
#include "roomutil.h"
#include "eventtypeutil.h"
#include "roomeventtypeutil.h"
#include "userroomeventtypeutil.h"
#include "eventutil.h"
#include "pcutil.h"
#include "systemfetch.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <stdio.h>
#include "eventnoteutil.h"

ImportNotes::ImportNotes(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	sharedObj=new DLCRShared();
	logSession=LoginUserSession::session();

	connect(ui.cmbLocation,			SIGNAL(currentIndexChanged(int)),	this,SLOT(cmbLocationName_indexChanged(int)));
	connect(ui.cmbRoom,				SIGNAL(currentIndexChanged(int)),	this,SLOT(cmbRoomName_indexChanged(int)));
	connect(ui.tblImportSession,	SIGNAL(cellClicked(int,int)),		this,SLOT(eventDetails_cellClicked(int,int)));
	connect(ui.tblImportSession,	SIGNAL(currentCellChanged(int,int,int,int)),this, SLOT(tblEventDetails_cellChanged(int,int)));
	connect(ui.cmbSearchBox,		SIGNAL(currentIndexChanged(int)),	this,SLOT(cmbSearchCategory_indexChanged(int)));
	connect(ui.txtSearch,			SIGNAL(textChanged(QString)),		this,SLOT(txtSearch_textChanged(QString)));
	connect(ui.btnImportNotes,		SIGNAL(clicked()),					this,SLOT(btnImportNotes_clicked()));
	//loadDetails();
}

void ImportNotes::cmbRoomName_indexChanged(int index_)
{
	cmbSearchCategory_indexChanged(ui.cmbSearchBox->currentIndex());

	if (ui.cmbRoom->itemData(index_,Qt::UserRole).toInt()==logSession->getCurrentRoom())
	{
		//ui.btnResume->setVisible(true);
	}
	else
	{
		//ui.btnResume->setVisible(false);
	}
}
void ImportNotes::loadLocations()
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
void ImportNotes::cmbLocationName_indexChanged(int index_)
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
void ImportNotes::cmbSearchCategory_indexChanged(int index_)
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

void ImportNotes::txtSearch_textChanged(QString str_)
{
	//for removing rows from table widget
	for (int i=ui.tblImportSession->rowCount(); i >= 0; --i)
	{
		ui.tblImportSession->removeRow(i);	
	}

	listEvents(str_);

	ui.btnImportNotes->setEnabled(false);
}
void ImportNotes::loadAllEvents()
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
void ImportNotes::loadRescentEvents()
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

void ImportNotes::listEvents(QString str_)
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
				//if(logSession->canViewClosedEvent()) itemFinishedDate->setText(info.getFinishedDate().toString("d-M-yyyy"));
				//else continue;
				//itemEventNo->setIcon(icoClosed);
				//vHItem->setIcon(icoClosed);
			}

			EventTypeUtil *etUtil=new EventTypeUtil(this);
			itemEventType->setText(etUtil->loadEventType(info.getEventTypeID(),logSession->getDriverName()).getEventTypeName());
			itemEventType->setData(Qt::UserRole,info.getEventTypeID());
			delete etUtil;

			itemStaus->setText(info.getEventStatus());

			if(!info.getEventNo().contains(str_)) continue;

			ui.tblImportSession->insertRow(k);

			//ui.tblEventDetails->setVerticalHeaderItem(k,vHItem);

			ui.tblImportSession->setItem(k,0,itemEventNo);
			ui.tblImportSession->setItem(k,1,itemEventTitle);
			ui.tblImportSession->setItem(k,2,itemEventDescription);
			ui.tblImportSession->setItem(k,3,itemCreatedDate);
			ui.tblImportSession->setItem(k,4,itemEventType);
			ui.tblImportSession->setItem(k,5,itemStaus);

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
void ImportNotes::eventDetails_cellClicked(int row_,int column_)
{
	ui.btnImportNotes->setEnabled(true);
}
void ImportNotes::btnImportNotes_clicked()
{
	int eventID=ui.tblImportSession->item(ui.tblImportSession->currentRow(),0)->data(Qt::UserRole).toInt();
	int roomID=ui.cmbRoom->itemData(ui.cmbRoom->currentIndex(),Qt::UserRole).toInt();
	bool chkState;

	DLCRMain *dlcrs=static_cast<DLCRMain*>(this->parent());
	EventNoteUtil *util=new EventNoteUtil(this);
	try
	{
		QList<EventNoteInfo> allEventNotes=util->selectAllEventNotes(eventID,roomID,logSession->getDriverName());
		if (tabIndex==0)
		{
			if (ui.chkTime->checkState()==Qt::Checked)
			{
				chkState=true;
			}
			else
			{
				chkState=false;
			}
			dlcrs->ui.activeEditor->createEditor();
			dlcrs->ui.activeEditor->insertImportRows(allEventNotes,chkState);
			ui.btnImportNotes->setEnabled(false);
		}
		else
		{
			dlcrs->ui.activeRetriever->createEditor();
			dlcrs->ui.activeRetriever->insertImportRows(allEventNotes);
			ui.btnImportNotes->setEnabled(false);
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
void ImportNotes::loadDetails()
{
	loadLocations();
	txtSearch_textChanged(ui.txtSearch->text());
}
void ImportNotes::setTabIndex(int index_)
{
	tabIndex=index_;
	loadLocations();
	exec();
}
ImportNotes::~ImportNotes()
{

}
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/