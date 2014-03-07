/*Owner & Copyrights: Vance King Saxbe. A.*//* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "dictionary.h"
#include "adddictionary.h"
#include "dictionaryutil.h"
#include "pcutil.h"
#include "systemfetch.h"
#include "customprogressbar.h"

Dictionary::Dictionary(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	logSession=LoginUserSession::session();
	connect(ui.btnPlus, SIGNAL(clicked()),this,SLOT(btnPlus_Clicked()));
	connect(ui.btnMinus, SIGNAL(clicked()),this,SLOT(btnMinus_Clicked()));
	connect(ui.btnOk, SIGNAL(clicked()),this,SLOT(btnOK_Clicked()));
	connect(ui.tblDictionary,SIGNAL(cellClicked(int,int)),	this,SLOT(tblDictionary_indexChange(int,int)));
	readDictionary();
}

void Dictionary::tblDictionary_indexChange(int row_ ,int col)
{
	DicRow=row_;
}
void Dictionary::btnPlus_Clicked()
{
	AddDictionary *adic=new AddDictionary(this);
	adic->exec();
	delete adic;
}

void Dictionary::btnOK_Clicked()
{
	this->close();
}

void Dictionary::btnMinus_Clicked()
{
	DictionaryUtil *util=new DictionaryUtil(this);
	int dicID;
	try
	{
		dicID=ui.tblDictionary->item(ui.tblDictionary->currentRow(),0)->data(Qt::UserRole).toInt();
		util->deleteDictionary(dicID,logSession->getDriverName());
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
	ui.tblDictionary->removeRow(DicRow);
}

void Dictionary::loadDictionary(QString name,QString language,QString country,QString File)
{
	DictionaryUtil *util=new DictionaryUtil(this);
	SystemFetch *fetch=new SystemFetch();
	PCUtil *pUtil=new PCUtil(this);

	QStringList tmpList=File.split("/");
	QString path="C:/Program Files/STPL";
	QString destinationPath=path+"/"+tmpList.last();

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

		DictionaryInfo Info;
		Info.setAliasName(name);
		Info.setLanguage(language);
		Info.setCountry(country);
		Info.setDicpath(destinationPath);
		Info.setUserID(logSession->getUserID());
		Info.setPCID(pInfo.getPCID());
		util->insertOrUpadteDictionary(Info,logSession->getDriverName());
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

	readDictionary();
}

void Dictionary::readDictionary()
{
 	DictionaryUtil *util=new DictionaryUtil(this);
	SystemFetch *fetch=new SystemFetch();
	PCUtil *pUtil=new PCUtil(this);
	try
	{

		PCInfo pInfo;
		pInfo=pUtil->loadPCDetails(fetch->getSystemID(),logSession->getDriverName());
		QList<DictionaryInfo> allDictionary=util->selectDictionaryByUser(pInfo.getPCID(),logSession->getUserID(),logSession->getDriverName());

		int k=0;

		for (int i=ui.tblDictionary->rowCount(); i >= 0; --i)
		{
			ui.tblDictionary->removeRow(i);	
		}

		foreach(DictionaryInfo info,allDictionary)
		{
			//creating table widget items with corresponding text
			QTableWidgetItem *item0=new QTableWidgetItem();
			item0->setText(info.getAliasName());
			item0->setData(Qt::UserRole,info.getDictionaryID());
			QTableWidgetItem *item1=new QTableWidgetItem();	
			item1->setText(info.getLanguage());
			QTableWidgetItem *item2=new QTableWidgetItem();	
			item2->setText(info.getCountry());
			QTableWidgetItem *item3=new QTableWidgetItem();	

			QStringList temp=info.getDicPath().split("/");
			QString fileName=temp.last();
			item3->setText(fileName);
			//inserting row in table locations
			ui.tblDictionary->insertRow(k);

			//inserting widget items in table location		
			ui.tblDictionary->setItem(k,0,item0);		
			ui.tblDictionary->setItem(k,1,item1);
			ui.tblDictionary->setItem(k,2,item2);
			ui.tblDictionary->setItem(k,3,item3);

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

	delete util;
}

Dictionary::~Dictionary()
{

}
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/