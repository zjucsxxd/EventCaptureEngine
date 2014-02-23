/* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include <QtGui/QApplication>
#include <QFileInfo>

#include "recorder.h"
#include "dlcrmain.h"
#include "uiloginuser.h"
#include "wizard.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	bool status;
	QFileInfo fi(QDir::currentPath()+"\\WConfig.ini");  
	status=fi.exists();

	if (status==false)
	{
		Wizard wizd;
		wizd.exec();
	
		LoginUser w;

		if (w.checkDBConnection()!=true)
		{
			return 1;
		}

		w.exec();

		DLCRMain *m;

		if (w.isValidUser())
		{
			m=new DLCRMain();
		
			if(m->canLoadSettings())
			{
				if(w.languageIndex()==1)
				{ 	
					qApp->setLayoutDirection(Qt::RightToLeft); 	
					m->setLayoutDirection(Qt::RightToLeft); 
					m->addDockWidget(Qt::LeftDockWidgetArea, m->ui.dockWidget);
				}
				else
				{
					qApp->setLayoutDirection(Qt::LeftToRight);
					m->setLayoutDirection(Qt::LeftToRight);			
					m->addDockWidget(Qt::RightDockWidgetArea, m->ui.dockWidget);
				}
				m->startSchedulerTimer();
				m->showMaximized();	
			}
			else
			{
				Wizard wizd;
				wizd.exec();;
			}
		}	
	}
	else
	{
		LoginUser w;

		if (w.checkDBConnection()!=true)
		{
			return 1;
		}

		w.exec();

		DLCRMain *m;

		if (w.isValidUser())
		{
			m=new DLCRMain();

			if(m->canLoadSettings())
			{
				if(w.languageIndex()==1)
				{ 	
					qApp->setLayoutDirection(Qt::RightToLeft); 	
					m->setLayoutDirection(Qt::RightToLeft); 
					m->addDockWidget(Qt::LeftDockWidgetArea, m->ui.dockWidget);
				}
				else
				{
					qApp->setLayoutDirection(Qt::LeftToRight);
					m->setLayoutDirection(Qt::LeftToRight);			
					m->addDockWidget(Qt::RightDockWidgetArea, m->ui.dockWidget);
				}
				m->startSchedulerTimer();
				m->showMaximized();	
			}
			else
			{
				Wizard wizd;
				wizd.exec();
			}
		}
		else
		{
			return 1;
		}
	}

	return a.exec();
}
