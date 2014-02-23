/* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "easynotes.h"
#include <QMessageBox>
#include <QTableWidgetItem>

#include "eventutil.h"
#include "easynoteutil.h"

EasyNotes::EasyNotes(QWidget *parent)
	: QDialog(parent)
{

	ui.setupUi(this);

	logSession=LoginUserSession::session();

	connect(ui.btnSave,		SIGNAL(clicked()),					this, SLOT(btnSave_click()));
	connect(ui.btnApply,	SIGNAL(clicked()),					this, SLOT(btnApply_click()));
	connect(ui.cmbBasedOn,	SIGNAL(currentIndexChanged(int)),	this, SLOT(cmbBasedOn_indexChange(int)));
	connect(ui.cmbCaseList,	SIGNAL(currentIndexChanged(int)),	this, SLOT(cmbCaseList_indexChange(int)));
	connect(ui.tblEasyNotes,SIGNAL(currentCellChanged(int,int,int,int)),this, SLOT(tblCellChanged()));

	refreshDialog();

}

void EasyNotes::refreshDialog()
{	
	ui.tblEasyNotes->setColumnWidth(1,250);

	ui.lblCaseList->setHidden(true);
	ui.cmbCaseList->setHidden(true);

	for(int i=0;i<ui.tblEasyNotes->rowCount();i++)
	{
		QTableWidgetItem *itemText,*itemLabel;
		itemText=new QTableWidgetItem();
		itemLabel=new QTableWidgetItem();

		itemText->setText(" ");
		itemLabel->setText(" ");

		ui.tblEasyNotes->setItem(i,1,itemText);	
		ui.tblEasyNotes->setItem(i,2,itemLabel);		
	}

	ui.cmbBasedOn->setCurrentIndex(0);

	cmbBasedOn_indexChange(ui.cmbBasedOn->currentIndex());	
}

void EasyNotes::btnSave_click()
{
	EasyNoteUtil *util=new EasyNoteUtil(this);

	for (int i=0;i<ui.tblEasyNotes->rowCount();i++)
	{
		QString key=(ui.tblEasyNotes->item(i,0)==NULL)? " ":ui.tblEasyNotes->item(i,0)->text();
		QString text=(ui.tblEasyNotes->item(i,1)==NULL)? " ":ui.tblEasyNotes->item(i,1)->text();
		QString label=(ui.tblEasyNotes->item(i,2)==NULL)? " ":ui.tblEasyNotes->item(i,2)->text();
		
		EasyNoteInfo info;
		info.setShortcutKey(key);
		info.setShortcutText(text);
		info.setShortcutLabel(label);
		
		if(ui.cmbBasedOn->currentIndex()==0)
		{
			info.setPCID(0);
			info.setEventID(0);
			info.setUserID(logSession->getUserID());
		}
		else if(ui.cmbBasedOn->currentIndex()==1)
		{
			info.setPCID(0);
			int eventID=ui.cmbCaseList->itemData(ui.cmbCaseList->currentIndex(),Qt::UserRole).toInt();
			info.setEventID(eventID);
			info.setUserID(0);
		}
		else
		{
			info.setPCID(logSession->getPCID());
			info.setEventID(0);
			info.setUserID(0);
		}
		
		try
		{
			util->insertOrUpdateEasyNote(info,logSession->getDriverName());
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

	delete util;
}
void EasyNotes::cmbBasedOn_indexChange(int index_)
{
	if (index_==0)
	{
		ui.lblCaseList->setHidden(true);
		ui.cmbCaseList->setHidden(true);
		loadEasyNotes(0,logSession->getUserID());
	}
	else if(index_==1)
	{
		ui.lblCaseList->setHidden(false);
		ui.cmbCaseList->setHidden(false);
		loadUserEvents();
	}
	else
	{
		ui.lblCaseList->setHidden(true);
		ui.cmbCaseList->setHidden(true);		
		loadEasyNotes(logSession->getPCID(),0);
	}
}
void EasyNotes::loadEasyNotes(int pcID_,int userID_)
{
	EasyNoteUtil *util=new EasyNoteUtil(this);
	QList<EasyNoteInfo> allNotes;

	try
	{
		allNotes=util->selectAllEasyNotes(pcID_,userID_,0,logSession->getDriverName());
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

 	for(int i=0;i<ui.tblEasyNotes->rowCount();i++)
 	{
 		QTableWidgetItem *itemText,*itemLabel;
 		itemText=new QTableWidgetItem();
 		itemLabel=new QTableWidgetItem();
 
 		itemText->setText(" ");
 		itemLabel->setText(" ");
 
 		delete ui.tblEasyNotes->item(i,1);
 		delete ui.tblEasyNotes->item(i,2);
 
 		ui.tblEasyNotes->setItem(i,1,itemText);
 		ui.tblEasyNotes->setItem(i,2,itemLabel);
 	}

	int k=0;

	foreach(EasyNoteInfo info,allNotes)
	{
		QTableWidgetItem *itemText,*itemLabel;
		itemText=new QTableWidgetItem();
		itemLabel=new QTableWidgetItem();

		itemText->setText(info.getShortcutText());
		itemText->setData(Qt::UserRole,info.getEasyNoteID());
		itemLabel->setText(info.getShortcutLabel());

		delete ui.tblEasyNotes->item(k,1);
		delete ui.tblEasyNotes->item(k,2);

		for (int j=0;j<ui.tblEasyNotes->rowCount();j++)
		{
			if (ui.tblEasyNotes->item(j,0)->text()==info.getShortcutKey())
			{
				ui.tblEasyNotes->setItem(j,1,itemText);	
				ui.tblEasyNotes->setItem(j,2,itemLabel);
			}
		}

		k++;
	}

	delete util;
}
void EasyNotes::cmbCaseList_indexChange(int index_)
{
	EasyNoteUtil *util=new EasyNoteUtil(this);

	int eventID=ui.cmbCaseList->itemData(index_,Qt::UserRole).toInt();
	QList<EasyNoteInfo> allNotes;

	try
	{
		allNotes=util->selectAllEasyNotes(0,0,eventID,logSession->getDriverName());
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

	for(int i=0;i<ui.tblEasyNotes->rowCount();i++)
	{
		QTableWidgetItem *itemText,*itemLabel;
		itemText=new QTableWidgetItem();
		itemLabel=new QTableWidgetItem();

		itemText->setText(" ");
		itemLabel->setText(" ");

		delete ui.tblEasyNotes->item(i,1);
		delete ui.tblEasyNotes->item(i,2);

		ui.tblEasyNotes->setItem(i,1,itemText);
		ui.tblEasyNotes->setItem(i,2,itemLabel);
	}

	int k=0;

	foreach(EasyNoteInfo info,allNotes)
	{
		QTableWidgetItem *itemText,*itemLabel;
		itemText=new QTableWidgetItem();
		itemLabel=new QTableWidgetItem();

		itemText->setText(info.getShortcutText());
		itemText->setData(Qt::UserRole,info.getEasyNoteID());
		itemLabel->setText(info.getShortcutLabel());

		delete ui.tblEasyNotes->item(k,1);
		delete ui.tblEasyNotes->item(k,2);

		for (int j=0;j<ui.tblEasyNotes->rowCount();j++)
		{
			if (ui.tblEasyNotes->item(j,0)->text()==info.getShortcutKey())
			{
				ui.tblEasyNotes->setItem(j,1,itemText);	
				ui.tblEasyNotes->setItem(j,2,itemLabel);
			}
		}

		k++;
	}

	delete util;
}
void EasyNotes::loadUserEvents()
{
	EventUtil *util=new EventUtil(this);

	try
	{
		//getting all location details
		QList<EventInfo> allEvents=util->loadEventsOFUser(logSession->getUserID(),logSession->getDriverName());

		int k=0;

		/*************clearing all location combo box*************************/
		//block signals is to prevent unusual index change signal emits

		ui.cmbCaseList->blockSignals(true);
		ui.cmbCaseList->clear();
		ui.cmbCaseList->blockSignals(false);

		foreach(EventInfo info,allEvents)
		{
			ui.cmbCaseList->insertItem(k,info.getEventNo(),info.getEventID());

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
void EasyNotes::applyShortcuts()
{
	btnApply_click();
}
void EasyNotes::btnApply_click()
{
	QStringList wordList;

	for (int i=0;i<ui.tblEasyNotes->rowCount();i++)
	{	
		QString str=(ui.tblEasyNotes->item(i,1)==NULL)? " ":ui.tblEasyNotes->item(i,1)->text();
		wordList<<str;
	}

	logSession->setShortcutList(wordList);

	this->close();
}
void EasyNotes::tblCellChanged()
{
	if(this->layoutDirection()==Qt::RightToLeft)
	{
		ui.tblEasyNotes->resize(ui.tblEasyNotes->width()+1,ui.tblEasyNotes->height());
		ui.tblEasyNotes->resize(ui.tblEasyNotes->width()-1,ui.tblEasyNotes->height());
	}
}
EasyNotes::~EasyNotes()
{

}
