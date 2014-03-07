/*Owner & Copyrights: Vance King Saxbe. A.*//* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "languages.h"
#include "addlanguage.h"
#include "languageutil.h"
#include "pcutil.h"
#include "systemfetch.h"
#include "customprogressbar.h"
#include <QDir>

Languages::Languages(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	logSession=LoginUserSession::session();
	connect(ui.btnPlus, SIGNAL(clicked()),this,SLOT(btnPlus_Clicked()));
	connect(ui.btnMinus, SIGNAL(clicked()),this,SLOT(btnMinus_Clicked()));
	connect(ui.btnOK, SIGNAL(clicked()),this,SLOT(btnOK_Clicked()));
	connect(ui.tbllanguage,SIGNAL(cellClicked(int,int)),	this,SLOT(tbllanguage_indexChange(int,int)));
	readLanguages();
}

void Languages::tbllanguage_indexChange(int row_ ,int col)
{
	languageRow=row_;

}
void Languages::btnPlus_Clicked()
{
	AddLanguage *alan=new AddLanguage(this);
	alan->exec();
	delete alan;

}

void Languages::btnOK_Clicked()
{
	this->close();
}
void Languages::btnMinus_Clicked()
{
	LanguageUtil *lutil=new LanguageUtil(this);
	int langID;
	try
	{
		langID=ui.tbllanguage->item(ui.tbllanguage->currentRow(),0)->data(Qt::UserRole).toInt();
		lutil->deleteLanguage(langID,logSession->getDriverName());
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

	ui.tbllanguage->removeRow(languageRow);
}
void Languages::loadLanguages(QString name,QString language,QString country,QString File)
{
	LanguageUtil *lutil=new LanguageUtil(this);
	SystemFetch *fetch=new SystemFetch();
	PCUtil *pUtil=new PCUtil(this);

	QStringList tmpList=File.split("/");
	QString path="C:/Program Files/TapeLanguages";
	QString destinationPath=path+"/"+tmpList.last();

	QDir dirPath=QDir(path);
	if (!dirPath.exists())
	{
		dirPath.mkpath(path);
	}

	CopyFileProgress *progress=new CopyFileProgress(this);

	bool status=progress->copyTranslationFile(File,destinationPath);

	delete progress;

	if(status==false)
	{
		return;

	}
	try
	{
		PCInfo pInfo;
		pInfo=pUtil->loadPCDetails(fetch->getSystemID(),logSession->getDriverName());

		LanguageInfo lInfo;
		lInfo.setAliasName(name);
		lInfo.setLanguage(language);
		lInfo.setCountry(country);
		lInfo.setQMPath(destinationPath);
		lInfo.setPCID(pInfo.getPCID());
		lInfo.setUserID(logSession->getUserID());
		lInfo.setIdentity("Recorder");

		lutil->insertOrUpdateLanguage(lInfo,logSession->getDriverName());
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
	delete lutil;
	delete pUtil;
	delete fetch;

	readLanguages();
}
void Languages::readLanguages()
{
	SystemFetch *fetch=new SystemFetch();
	PCUtil *pUtil=new PCUtil(this);
	LanguageUtil *lutil=new LanguageUtil(this);
	try
	{
		PCInfo pInfo;
		pInfo=pUtil->loadPCDetails(fetch->getSystemID(),logSession->getDriverName());

		QList<LanguageInfo> alllanguages=lutil->selectLanguageByUser(pInfo.getPCID(),logSession->getUserID(),"Recorder",logSession->getDriverName());

		int k=0;

		for (int i=ui.tbllanguage->rowCount(); i >= 0; --i)
		{
			ui.tbllanguage->removeRow(i);	
		}

		foreach(LanguageInfo linfo,alllanguages)
		{
			//creating table widget items with corresponding text
			QTableWidgetItem *item0=new QTableWidgetItem();
			item0->setText(linfo.getAliasName());
			item0->setData(Qt::UserRole,linfo.getLanguageID());
			QTableWidgetItem *item1=new QTableWidgetItem();	
			item1->setText(linfo.getLanguage());
			QTableWidgetItem *item2=new QTableWidgetItem();	
			item2->setText(linfo.getCountry());
			QTableWidgetItem *item3=new QTableWidgetItem();	

			QStringList temp=linfo.getQMPath().split("/");
			QString fileName=temp.last();
			item3->setText(fileName);
			//inserting row in table locations
			ui.tbllanguage->insertRow(k);

			//inserting widget items in table location		
			ui.tbllanguage->setItem(k,0,item0);		
			ui.tbllanguage->setItem(k,1,item1);
			ui.tbllanguage->setItem(k,2,item2);
			ui.tbllanguage->setItem(k,3,item3);

			k++;//iteration
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

	delete lutil;
	delete pUtil;
	delete fetch;
}

Languages::~Languages()
{

}
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/