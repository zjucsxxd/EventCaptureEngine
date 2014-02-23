/* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "retriever.h"

#include <qDebug>
#include <QDateTime>
#define TIME_COUNT 0
#define SPEAKER_COUNT 1
#define NOTE_COUNT 2
#define INTERNAL 3
#define EXTERNAL 4

Retriever::Retriever(QWidget *parent)
	: QTableWidget(parent)
{
	//fixedIco=QIcon(QPixmap(":/Recorder/Resources/attach.png"));
	//currentIco=QIcon(QPixmap(":/Recorder/Resources/edit-normal.png"));

	connect(this,	SIGNAL(cellClicked(int,int)),	this,SLOT(updateRowHeader(int,int)));
	connect(this,	SIGNAL(currentCellChanged(int,int,int,int)),this, SLOT(tblCellChanged()));
}
void Retriever::clear()
{
	//for removing rows from table widget
	for (int i=this->rowCount(); i >= 0; --i)
	{
		this->removeRow(i);	
	}
}
void Retriever::insertRows(QList<EventNoteInfo> allEventNotes_)
{
	int k=0;

	foreach(EventNoteInfo info,allEventNotes_)
	{
		QTableWidgetItem *itemDateTime,*itemSpeaker,*itemNotes,*itemInternal,*itemExternal,*vHItem;

		itemDateTime=new QTableWidgetItem();
		itemSpeaker=new QTableWidgetItem();
		itemNotes=new QTableWidgetItem();
		itemInternal=new QTableWidgetItem();
		itemExternal=new QTableWidgetItem();

		vHItem=new QTableWidgetItem();

		QString strDate="<font size=\"4\" color=\"red\">" + info.getEventTime().toString("dd-MM-yyyy hh:mm:ss AP") + "</font>";
		itemDateTime->setText(strDate);
		itemDateTime->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable);
		itemDateTime->setData(Qt::UserRole,info.getEventNoteID());
		QBrush brush=itemDateTime->foreground();
		brush.setColor(QColor(Qt::red));
		itemDateTime->setForeground(brush);
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
}
void Retriever::insertImportRows(QList<EventNoteInfo> allEventNotes_)
{
	int k=this->rowCount();

	foreach(EventNoteInfo info,allEventNotes_)
	{
		QTableWidgetItem *itemDateTime,*itemSpeaker,*itemNotes,*itemInternal,*itemExternal,*vHItem;

		itemDateTime=new QTableWidgetItem();
		itemSpeaker=new QTableWidgetItem();
		itemNotes=new QTableWidgetItem();
		itemInternal=new QTableWidgetItem();
		itemExternal=new QTableWidgetItem();

		vHItem=new QTableWidgetItem();

		QString strDate="<font size=\"4\" color=\"red\">" + info.getEventTime().toString("dd-MM-yyyy hh:mm:ss AP") + "</font>";
		itemDateTime->setText(strDate);
		itemDateTime->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable);
		itemDateTime->setData(Qt::UserRole,info.getEventNoteID());
		QBrush brush=itemDateTime->foreground();
		brush.setColor(QColor(Qt::red));
		itemDateTime->setForeground(brush);
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
}
void Retriever::updateRowHeader(int row_,int column_)
{
	for(int i=0;i<this->rowCount();i++)
	{
		//this->verticalHeaderItem(i)->setIcon(fixedIco);
	}

	//this->verticalHeaderItem(row_)->setIcon(currentIco);	
}
void Retriever::tblCellChanged()
{
	if(this->layoutDirection()==Qt::RightToLeft)
	{
		this->resize(this->width()+1,this->height());
		this->resize(this->width()-1,this->height());
	}
}
void Retriever::insertRow(int row_)
{
	QTableWidget::insertRow(row_);

	QTableWidgetItem *time,*notes,*speaker,*itemInternal,*itemExternal,*vHItem;

	time   = new QTableWidgetItem();
	notes  = new QTableWidgetItem();
	speaker = new QTableWidgetItem();	
	itemInternal=new QTableWidgetItem();
	itemExternal=new QTableWidgetItem();
	vHItem = new QTableWidgetItem();

	time->setFlags(Qt::ItemIsEditable);	
	notes->setFlags(Qt::ItemIsEditable);	
	speaker->setFlags(Qt::ItemIsEditable);
	itemInternal->setFlags(Qt::ItemIsEditable);
	itemExternal->setFlags(Qt::ItemIsEditable);

	this->setItem(row_,TIME_COUNT,time);
	this->setItem(row_,SPEAKER_COUNT,speaker);
	this->setItem(row_,NOTE_COUNT,notes);
	this->setItem(row_,INTERNAL,itemInternal);
	this->setItem(row_,EXTERNAL,itemExternal);

	//this->setVerticalHeaderItem(row_,vHItem);
}

void Retriever::activate(int row_)
{	
	QTableWidgetItem *time,*notes,*speaker,*itemInternal,*itemExternal,*vHItem;

	time   = new QTableWidgetItem();
	notes  = new QTableWidgetItem();
	speaker = new QTableWidgetItem();
	itemInternal=new QTableWidgetItem();
	itemExternal=new QTableWidgetItem();
	vHItem = new QTableWidgetItem();	

	//vHItem->setIcon(currentIco);
	time->setText("#"+dayTimeNow());
	time->setFlags(Qt::ItemIsEditable);

	speaker->setText(this->item(row_,SPEAKER_COUNT)->text());
	notes->setText(this->item(row_,NOTE_COUNT)->text());
	itemInternal->setText(this->item(row_,INTERNAL)->text());
	itemExternal->setText(this->item(row_,EXTERNAL)->text());

	delete this->item(row_,TIME_COUNT);
	delete this->item(row_,SPEAKER_COUNT);
	delete this->item(row_,NOTE_COUNT);
	delete this->item(row_,INTERNAL);
	delete this->item(row_,EXTERNAL);

	this->setItem(row_,TIME_COUNT,time);
	this->setItem(row_,SPEAKER_COUNT,speaker);
	this->setItem(row_,NOTE_COUNT,notes);
	this->setItem(row_,INTERNAL,itemInternal);
	this->setItem(row_,EXTERNAL,itemExternal);

	//this->setVerticalHeaderItem(row_,vHItem);

	this->setCurrentCell(row_,SPEAKER_COUNT);
	this->editItem(this->currentItem());
}

QString Retriever::dayTimeNow()
{
	QDateTime currentTime= QDateTime::currentDateTime();
	QString timeStamp="<font size=\"4\" color=\"red\">" + currentTime.toString("dd.MM.yyyy hh:mm:ss AP") + "</font>";
	return timeStamp;
}

Retriever::~Retriever()
{

}
