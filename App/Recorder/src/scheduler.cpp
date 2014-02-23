/* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "scheduler.h"
#include "addscheduler.h"
#include "editschedulers.h"
#include "schedulerutil.h"
#include "eventutil.h"
#include <QDate>
#include <QTime>

Scheduler::Scheduler(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	logSession=LoginUserSession::session();

	connect(ui.btnplus,		SIGNAL(clicked()),			 this,SLOT(btnPlus_Clicked()));
	connect(ui.btnMinus,	SIGNAL(clicked()),			 this,SLOT(btnMinus_Clicked()));
	connect(ui.btnEdit,		SIGNAL(clicked()),			 this,SLOT(btnEdit_Clicked()));
	connect(ui.tblScheduler,SIGNAL(cellClicked(int,int)),this,SLOT(tblscheduler_indexChange(int,int)));

	readScheduler();

	ui.btnEdit->setEnabled(false);
	ui.btnMinus->setEnabled(false);
}
void Scheduler::tblscheduler_indexChange(int row_,int col_)
{
	deleteRow=row_;
	ui.btnEdit->setEnabled(true);
	ui.btnMinus->setEnabled(true);

}
void Scheduler::btnPlus_Clicked()
{
	AddScheduler *addschdedule=new AddScheduler(this);
	addschdedule->exec();
	delete addschdedule;
}
void Scheduler::btnMinus_Clicked()
{

	SchedulerUtil *sutil=new SchedulerUtil(this);

	if (ui.tblScheduler->rowCount()==0)
	{
		return;
	}
	try
	{
		int scheduleID=ui.tblScheduler->item(ui.tblScheduler->currentRow(),0)->data(Qt::UserRole).toInt();
		sutil->deleteScheduler(scheduleID,logSession->getDriverName());
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

	ui.tblScheduler->removeRow(deleteRow);
	ui.btnEdit->setEnabled(false);
	ui.btnMinus->setEnabled(false);

}
void Scheduler::btnEdit_Clicked()
{
	int scheduleID=ui.tblScheduler->item(ui.tblScheduler->currentRow(),0)->data(Qt::UserRole).toInt();
	EditSchedulers *edit=new EditSchedulers(this);
	edit->setEditSchduleID(scheduleID);
	delete edit;
	ui.btnEdit->setEnabled(false);
	ui.btnMinus->setEnabled(false);
}
void Scheduler::loadschedulersinfo(QString action,QString repeat,QString date,QString time,QString eventNo,QString eventName,QString eventType)
{
	readScheduler();
}
void Scheduler::readScheduler()
{
	EventUtil *eutil=new EventUtil(this);
	SchedulerUtil *sutil=new SchedulerUtil(this);

	for (int i=ui.tblScheduler->rowCount(); i >= 0; --i)
	{
		ui.tblScheduler->removeRow(i);	
	}

	try
	{
		QList<EventInfo> eventIDLists;
		QList<SchedulerInfo> schedulersLists;
		int eventIDOfRoom;
		QString sAction,SRepeat,sDate,sTime,SEventNo,sEventName,sEventType;
		int sun,mon,tues,wed,thur,fri,sat;
		int k=0;
	
			schedulersLists=sutil->selectAllSchedulerByStatus(1,logSession->getDriverName());
			foreach(SchedulerInfo sinfo,schedulersLists)
			{
				sDate=sinfo.getScheduleDate();
				sTime=sinfo.getScheduleTime();
				sAction=sinfo.getSchedulerAction();
				SRepeat=sinfo.getSchedulerRepeat();
				SEventNo=sinfo.getEventNo();
				sEventName=sinfo.getEventName();
				sEventType=sinfo.getEventType(); 
				sun=sinfo.getSunday();
				mon=sinfo.getMonday();
				tues=sinfo.getTuesday();
				wed=sinfo.getWenesday();
				thur=sinfo.getThursday();
				fri=sinfo.getFriday();
				sat=sinfo.getSaturday();

				QTableWidgetItem *item1=new QTableWidgetItem();	
				item1->setText(sAction);
				item1->setData(Qt::UserRole,sinfo.getScheduleID());
				QTableWidgetItem *item2=new QTableWidgetItem();
				item2->setText(SRepeat);
				QTableWidgetItem *item3=new QTableWidgetItem();
				item3->setText(sDate);
				QTableWidgetItem *item4=new QTableWidgetItem();
				item4->setText(sTime);
				QTableWidgetItem *item5=new QTableWidgetItem();

				QString date=QDate::currentDate().toString("M/d/yyyy");
				int currentMonth=QDate::currentDate().month();
				int currentDay=QDate::currentDate().day();

				QDate scheduleDate=QDate::fromString(sDate,"M/d/yyyy");
				int scheduleMonth=scheduleDate.month();
				int scheduleDay=scheduleDate.day();

				if (SRepeat=="Once")
				{
					if (date==sDate)
					{
						QTime time=QTime::fromString(sTime,"h:m:s AP");
						QTime t,cTime; 

						int schedulerTime=t.secsTo(time);
						int currentTime=cTime.secsTo(QTime::currentTime());
						int expectedTime=schedulerTime-currentTime;

						if (expectedTime<=0)
						{
							item5->setText("Task OverDue");
						}
						else
						{
							item5->setText("Task Pending");
						}
					}
					else
					{
						if (scheduleMonth==currentMonth)
						{
							if (scheduleDay>currentDay)
							{
								item5->setText("Task Pending");
							}
							else
							{
								item5->setText("Task OverDue");
							}
							
						}
						if (scheduleMonth>currentMonth)
						{
							item5->setText("Task Pending");
						}
						if (scheduleMonth<currentMonth)
						{
							item5->setText("Task OverDue");
						}
					}
				}
				
				if (SRepeat=="Daily")
				{
					QTime time=QTime::fromString(sTime,"h:m:s AP");
					QTime t,cTime; 

					int schedulerTime=t.secsTo(time);
					int currentTime=cTime.secsTo(QTime::currentTime());
					int expectedTime=schedulerTime-currentTime;

					if (expectedTime<=0)
					{
							item5->setText("Task Finished");
					}
					else
					{
							item5->setText("Task Pending");
					}	
				}
				if (SRepeat=="Weekly")
				{
					QTime time=QTime::fromString(sTime,"h:m:s AP");
					QTime t,cTime; 

					int schedulerTime=t.secsTo(time);
					int currentTime=cTime.secsTo(QTime::currentTime());
					int expectedTime=schedulerTime-currentTime;

					QDate cdate=QDate::currentDate();
					int dayOfWeek=cdate.dayOfWeek();

					if (dayOfWeek==7 && sun==1)
					{
						if (expectedTime<=0)
						{
							item5->setText("Task Finished");
						}
						else
						{
							item5->setText("Task Pending");
						}
					}
					else if (dayOfWeek==1 && mon==1)
					{
						if (expectedTime<=0)
						{
							item5->setText("Task Finished");
						}
						else
						{
							item5->setText("Task Pending");
						}
					}
					else if (dayOfWeek==2 && tues==1)
					{
						if (expectedTime<=0)
						{
							item5->setText("Task Finished");
						}
						else
						{
							item5->setText("Task Pending");
						}
						
					}
					else if (dayOfWeek==3 && wed==1)
					{
						if (expectedTime<=0)
						{
							item5->setText("Task Finished");
						}
						else
						{
							item5->setText("Task Pending");
						}
					}
					else if (dayOfWeek==4 && thur==1)
					{
						if (expectedTime<=0)
						{
							item5->setText("Task Finished");
						}
						else
						{
							item5->setText("Task Pending");
						}
					}
					else if (dayOfWeek==5 && fri==1)
					{
						if (expectedTime<=0)
						{
							item5->setText("Task Finished");
						}
						else
						{
							item5->setText("Task Pending");
						}
					}
					else if (dayOfWeek==6 && sat==1)
					{
						if (expectedTime<=0)
						{
							item5->setText("Task Finished");
						}
						else
						{
							item5->setText("Task Pending");
						}
					}

					else
					{
							item5->setText("Task Pending");
					}
				}
				QTableWidgetItem *item6=new QTableWidgetItem();
				item6->setText(SEventNo+","+sEventName+","+sEventType);


				ui.tblScheduler->insertRow(k);

				ui.tblScheduler->setItem(k,0,item1);		
				ui.tblScheduler->setItem(k,1,item2);
				ui.tblScheduler->setItem(k,2,item3);
				ui.tblScheduler->setItem(k,3,item4);
				ui.tblScheduler->setItem(k,4,item5);
				ui.tblScheduler->setItem(k,5,item6);
				k++;
			}

			for (int i=0;i<ui.tblScheduler->rowCount(); i++)
			{
				QTableWidgetItem *itemtext=new QTableWidgetItem();
				itemtext=ui.tblScheduler->item(i,4);
				QString StatusStr=itemtext->text();

				if (StatusStr=="Task Finished")
				{
					ui.tblScheduler->item(i,0)->setBackgroundColor(QColor(193,255,193));
					ui.tblScheduler->item(i,1)->setBackgroundColor(QColor(193,255,193));
					ui.tblScheduler->item(i,2)->setBackgroundColor(QColor(193,255,193));
					ui.tblScheduler->item(i,3)->setBackgroundColor(QColor(193,255,193));
					ui.tblScheduler->item(i,4)->setBackgroundColor(QColor(193,255,193));
					ui.tblScheduler->item(i,5)->setBackgroundColor(QColor(193,255,193));
				}
				if (StatusStr=="Task Pending")
				{
					ui.tblScheduler->item(i,0)->setBackgroundColor(QColor(244,164,96));
					ui.tblScheduler->item(i,1)->setBackgroundColor(QColor(244,164,96));
					ui.tblScheduler->item(i,2)->setBackgroundColor(QColor(244,164,96));
					ui.tblScheduler->item(i,3)->setBackgroundColor(QColor(244,164,96));
					ui.tblScheduler->item(i,4)->setBackgroundColor(QColor(244,164,96));
					ui.tblScheduler->item(i,5)->setBackgroundColor(QColor(244,164,96));
				}
				if (StatusStr=="Task OverDue")
				{
					ui.tblScheduler->item(i,0)->setBackgroundColor(QColor(255, 0, 0, 127));
					ui.tblScheduler->item(i,1)->setBackgroundColor(QColor(255, 0, 0, 127));
					ui.tblScheduler->item(i,2)->setBackgroundColor(QColor(255, 0, 0, 127));
					ui.tblScheduler->item(i,3)->setBackgroundColor(QColor(255, 0, 0, 127));
					ui.tblScheduler->item(i,4)->setBackgroundColor(QColor(255, 0, 0, 127));
					ui.tblScheduler->item(i,5)->setBackgroundColor(QColor(255, 0, 0, 127));
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

}
Scheduler::~Scheduler()
{

}
