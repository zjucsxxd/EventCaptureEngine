/*Owner & Copyrights: Vance King Saxbe. A.*//* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "editor.h"

#include <QTime>
#include <QAbstractItemView>
#define TIME_COUNT 0
#define SPEAKER_COUNT 1
#define NOTE_COUNT 2
#define INTERNAL 3
#define EXTERNAL 4
Editor::Editor(QWidget *parent)
	: QTableWidget(parent)
{
	//fixedIco=QIcon(QPixmap(":/Recorder/Resources/attach.png"));
	//currentIco=QIcon(QPixmap(":/Recorder/Resources/edit-normal.png"));

	sharedObj=new DLCRShared();
	tempDB=SQLiteDataBase::dataBase();
	logSession=LoginUserSession::session();
	this->setAlternatingRowColors(true);
	
	connect(this,	SIGNAL(currentCellChanged(int,int,int,int)),this, SLOT(tblCellChanged()));
}

void Editor::insertRow(int row_)
{
	QTableWidget::insertRow(row_);

	QTableWidgetItem *time,*notes,*speaker,*iReference,*eReference,*vHItem;

	time   = new QTableWidgetItem();
	notes  = new QTableWidgetItem();
	speaker = new QTableWidgetItem();
	iReference=new QTableWidgetItem();
	eReference=new QTableWidgetItem();
	vHItem = new QTableWidgetItem();

	time->setFlags(Qt::ItemIsEditable);	
	notes->setFlags(Qt::ItemIsEditable);	
	speaker->setFlags(Qt::ItemIsEditable);
	iReference->setFlags(Qt::ItemIsEditable);
	eReference->setFlags(Qt::ItemIsEditable);

	this->setItem(row_,TIME_COUNT,time);
	this->setItem(row_,SPEAKER_COUNT,speaker);
	this->setItem(row_,NOTE_COUNT,notes);
	this->setItem(row_,INTERNAL,iReference);
	this->setItem(row_,EXTERNAL,eReference);

	//this->setVerticalHeaderItem(row_,vHItem);
}

void Editor::activate(int row_)
{	
	QTableWidgetItem *time,*notes,*speaker,*iReference,*eReference,*vHItem;

	time   = new QTableWidgetItem();
	notes  = new QTableWidgetItem();
	speaker = new QTableWidgetItem();
	iReference=new QTableWidgetItem();
	eReference=new QTableWidgetItem();
	vHItem = new QTableWidgetItem();	

	//vHItem->setIcon(currentIco);
	time->setText("#"+dayTimeNow());
	time->setFlags(Qt::ItemIsEditable);

	speaker->setText(this->item(row_,SPEAKER_COUNT)->text());
	notes->setText(this->item(row_,NOTE_COUNT)->text());
	iReference->setText(this->item(row_,INTERNAL)->text());
	eReference->setText(this->item(row_,EXTERNAL)->text());

	delete this->item(row_,TIME_COUNT);
	delete this->item(row_,SPEAKER_COUNT);
	delete this->item(row_,NOTE_COUNT);
	delete this->item(row_,INTERNAL);
	delete this->item(row_,EXTERNAL);

	this->setItem(row_,TIME_COUNT,time);
	this->setItem(row_,SPEAKER_COUNT,speaker);
	this->setItem(row_,NOTE_COUNT,notes);
	this->setItem(row_,INTERNAL,iReference);
	this->setItem(row_,EXTERNAL,eReference);

	//this->setVerticalHeaderItem(row_,vHItem);

	this->setCurrentCell(row_,SPEAKER_COUNT);
	this->editItem(this->currentItem());
}

QString Editor::dayTimeNow()
{
	QTime currentTime= QTime::currentTime();
	QString timeStamp="<font size=\"4\" color=\"red\">" + currentTime.toString("hh:mm:ss AP") + "</font>";
	return timeStamp;
}

void Editor::updateRow(int row_)
{
	QTableWidgetItem *rowTime = new QTableWidgetItem(); 
	QString timeStamp = this->item(row_,TIME_COUNT)->text();
	timeStamp.replace("#","");
	rowTime->setText(timeStamp);
	rowTime->setFlags(Qt::ItemIsEditable);
	this->setItem(row_,TIME_COUNT,rowTime);
	QTableWidgetItem *vHItem = new QTableWidgetItem();
	//vHItem->setIcon(fixedIco);
	//this->setVerticalHeaderItem(row_,vHItem);
}
void Editor::clear()
{
	//for removing rows from table widget
	for (int i=this->rowCount(); i >= 0; --i)
	{
		this->removeRow(i);	
	}
}
void Editor::tblCellChanged()
{
	if(this->layoutDirection()==Qt::RightToLeft)
	{
		this->resize(this->width()+1,this->height());
		this->resize(this->width()-1,this->height());
	}
}
void Editor::insertImportRows(QList<EventNoteInfo> allEventNotes_,bool state)
{
	int k=this->rowCount();
	QString cTime;

	foreach(EventNoteInfo info,allEventNotes_)
	{
		QTableWidgetItem *itemDateTime,*itemSpeaker,*itemNotes,*itemInternal,*itemExternal,*vHItem;

		itemDateTime=new QTableWidgetItem();
		itemSpeaker=new QTableWidgetItem();
		itemNotes=new QTableWidgetItem();
		itemInternal=new QTableWidgetItem();
		itemExternal=new QTableWidgetItem();

		vHItem=new QTableWidgetItem();

		if (state==true)
		{
			cTime=dayTimeNow();
			itemDateTime->setText("#"+cTime);
			itemDateTime->setFlags(Qt::ItemIsEditable);
		}
		else
		{
			QString time1="#"+info.getEventTime().time().toString("hh:mm:ss AP");
			QString strDate="<font size=\"4\" color=\"red\">" + time1 + "</font>";
			itemDateTime->setText(strDate);
			itemDateTime->setFlags(Qt::ItemIsEditable);
			QBrush brush=itemDateTime->foreground();
			brush.setColor(QColor(Qt::red));
			itemDateTime->setForeground(brush);
		}
		
		itemSpeaker->setText(info.getRichSpeaker());
		itemNotes->setText(info.getRichNote());
		itemInternal->setText(info.getElapsedTime());
		itemExternal->setText(info.getSessionURL());
		//vHItem->setIcon(fixedIco);

		this->insertRow(k);

		//this->setVerticalHeaderItem(k,vHItem);
		this->setItem(k,TIME_COUNT,itemDateTime);
		this->setItem(k,SPEAKER_COUNT,itemSpeaker);
		this->setItem(k,NOTE_COUNT,itemNotes);
		this->setItem(k,INTERNAL,itemInternal);
		this->setItem(k,EXTERNAL,itemExternal);

		k++;
	}

	updateToDatabase(allEventNotes_,cTime,state);

}
void Editor::updateToDatabase(QList<EventNoteInfo> allEventNotes,QString time_,bool status_)
{
	foreach(EventNoteInfo info,allEventNotes)
	{
		QString strDateTime;
		QDateTime daytime=sharedObj->getCurrentDateTime();
		QString date=daytime.date().toString("dd/MM/yyyy");
		QString time=sharedObj->plainText(time_);
		if (status_==true)
		{		
			strDateTime=date+" "+time;
		}
		else
		{
			strDateTime=info.getEventTime().toString("dd/MM/yyyy hh:mm:ss AP");
		}
		
		tempNotes notes;
		notes.eventTime=strDateTime;
		//notes.filePath=sharedObj->filterToServerPath(logSession->getCurrentFilePath(),logSession->getSharedPath());
		notes.richEventSpeaker=info.getRichSpeaker();
		notes.richEventNotes=info.getRichNote();
		notes.internalReference=info.getElapsedTime();
		notes.externalReference=info.getSessionURL();
		notes.visibility=1;
		QStringList toServerpath;
		toServerpath=sharedObj->filterToServerPath(logSession->getCurrentFilePath(),logSession->getSharedPath());

		for (int i=0;i<toServerpath.count();i++)
		{
			tempDB->insertNotes(notes,toServerpath[i],0);
		}
	}

}

void Editor::deactivate()
{
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
Editor::~Editor()
{

}
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/