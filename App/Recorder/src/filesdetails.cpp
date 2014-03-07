/*Owner & Copyrights: Vance King Saxbe. A.*//* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "filesdetails.h"
#include "attachedfileutil.h"

FilesDetails::FilesDetails(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	tempDB=SQLiteDataBase::dataBase();
	logSession=LoginUserSession::session();

	connect(ui.btnPlus,	 SIGNAL(clicked()),			  this,SLOT(btnPlus_Clicked()));
	connect(ui.btnMinus, SIGNAL(clicked()),			  this,SLOT(btnMinus_Clicked()));
	connect(ui.btnOK,	 SIGNAL(clicked()),			  this,SLOT(btnOK_Clicked()));
	connect(ui.tblFile,	 SIGNAL(cellClicked(int,int)),this,SLOT(tblFiles_indexChange(int,int)));
}
void FilesDetails::tblFiles_indexChange(int row_ ,int col)
{
	remRow=row_;
}
void FilesDetails::btnPlus_Clicked()
{
	ReferenceFile *refer=new ReferenceFile(this);
	refer->setCurrentTime(RowTime);
	refer->setTabIndex(tabIndx);
	delete refer;
}

void FilesDetails::btnOK_Clicked()
{
	this->close();
}

void FilesDetails::btnMinus_Clicked()
{
	AttachedFileUtil *autil=new AttachedFileUtil(this);

	if (ui.tblFile->rowCount()==0)
	{
		return;
	}
	try
	{
		int attachedFileID=ui.tblFile->item(ui.tblFile->currentRow(),0)->data(Qt::UserRole).toInt();
		autil->deleteAttachedFile(attachedFileID,logSession->getDriverName());
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

	QString fName=ui.tblFile->item(ui.tblFile->currentRow(),0)->text();
	QString fSize=ui.tblFile->item(ui.tblFile->currentRow(),1)->text();
	QString fType=ui.tblFile->item(ui.tblFile->currentRow(),2)->text();
	QStringList fNameList;

	fNameList=tempDB->loadReferenceFile(fSize,fType);

	if (fNameList.count()<0)
	{

	} 
	else
	{
		for (int i=0;i<fNameList.count();i++)
		{
			QString file;
			QStringList path=fNameList[i].split("\\");
			file=path.last();
			if (file==fName)
			{
				tempDB->deleteReferenceFile(fNameList[i],fSize,fType);
			}
		}
	}
	ui.tblFile->removeRow(remRow);
}

void FilesDetails::readFile()
{
	AttachedFileUtil *autil=new AttachedFileUtil(this);

	int k=0;

	for (int i=ui.tblFile->rowCount(); i >= 0; --i)
	{
		ui.tblFile->removeRow(i);	
	}

	try
	{
		QList<AttachedFileInfo> attachedFiles;
		QString FilePath,date,size,type;
		
		attachedFiles=autil->selectAttachedFilesByEvent(logSession->getEventID(),logSession->getDriverName());

		foreach(AttachedFileInfo ainfo,attachedFiles)
		{
			QString file=ainfo.getAttachedPath();
			QStringList path=file.split("\\");
			FilePath=path.last();
			date=ainfo.getCreatedDate().toString("dd/MM/yyyy hh:mm:ss AP");
			size=ainfo.getAttachedSize();
			type=ainfo.getAttachedType();

			if (type=="Animation File" || type=="Image File")
			{
				
			}
			else
			{
				QTableWidgetItem *item1=new QTableWidgetItem();	
				item1->setText(FilePath);
				item1->setData(Qt::UserRole,ainfo.getAttachedFileID());

				QTableWidgetItem *item2=new QTableWidgetItem();
				item2->setText(size);

				QTableWidgetItem *item3=new QTableWidgetItem();
				item3->setText(type);

				ui.tblFile->insertRow(k);

				ui.tblFile->setItem(k,0,item1);		
				ui.tblFile->setItem(k,1,item2);
				ui.tblFile->setItem(k,2,item3);
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

	QList<QStringList> rFileList;
	rFileList=tempDB->loadReferenceFile();

	if (rFileList.count()<0)
	{
		return;
	}
	else
	{
		int i=0;
		QString FilePath,size,type;

		foreach(QStringList temp,rFileList)
		{
			for (int r=0;r<temp.count();r++)
			{
				if (r==0)
				{
					QStringList path=temp[r].split("\\");
					FilePath=path.last();
				}
				if (r==1)
				{
					size=temp[r];
				}
				if (r==2)
				{
					type=temp[r];
				}
			}
			
			
			if (type=="Animation File" || type=="Image File")
			{

			}
			else
			{
				QTableWidgetItem *item1=new QTableWidgetItem();	
				item1->setText(FilePath);
			
				QTableWidgetItem *item2=new QTableWidgetItem();
				item2->setText(size);

				QTableWidgetItem *item3=new QTableWidgetItem();
				item3->setText(type);

				ui.tblFile->insertRow(k);

				ui.tblFile->setItem(k,0,item1);		
				ui.tblFile->setItem(k,1,item2);
				ui.tblFile->setItem(k,2,item3);
				k++;
			}
		}
	}
}

void FilesDetails::loadFileDetails(QString filepath_,QString size_,QString type_)
{
	int rcount=ui.tblFile->rowCount();

	QStringList temp=filepath_.split("/");
	QString fileName=temp.last();

	if (ui.tblFile->rowCount()==0)
	{
		QTableWidgetItem *item1=new QTableWidgetItem();	
		item1->setText(fileName);
		item1->setData(Qt::UserRole,filepath_);

		QTableWidgetItem *item2=new QTableWidgetItem();
		item2->setText(size_);

		QTableWidgetItem *item3=new QTableWidgetItem();
		item3->setText(type_);
		
		ui.tblFile->insertRow(rcount);

		ui.tblFile->setItem(rcount,0,item1);		
		ui.tblFile->setItem(rcount,1,item2);
		ui.tblFile->setItem(rcount,2,item3);
		rcount++;
	}
	else
	{
		for (int j=0;j<ui.tblFile->rowCount();j++)
		{
			QTableWidgetItem *itemtext=new QTableWidgetItem();
			QTableWidgetItem *itemtext2=new QTableWidgetItem();
			QTableWidgetItem *itemtext3=new QTableWidgetItem();

			itemtext=ui.tblFile->item(j,0);
			itemtext2=ui.tblFile->item(j,1);
			itemtext3=ui.tblFile->item(j,2);
			
			QString text=itemtext->text();
			QString text1=itemtext2->text();
			QString text2=itemtext3->text();

			if (text==fileName && text1==size_ && text2==type_ )
			{
				return;
			}
		}

		QTableWidgetItem *item1=new QTableWidgetItem();	
		item1->setText(fileName);

		QTableWidgetItem *item2=new QTableWidgetItem();
		item2->setText(size_);

		QTableWidgetItem *item3=new QTableWidgetItem();
		item3->setText(type_);

		ui.tblFile->insertRow(rcount);

		ui.tblFile->setItem(rcount,0,item1);		
		ui.tblFile->setItem(rcount,1,item2);
		ui.tblFile->setItem(rcount,2,item3);
		rcount++;
	}	

}

void FilesDetails::setTab(int indx_)
{
	tabIndx=indx_;
	readFile();
	exec();
}

void FilesDetails::setRowTime(QString time_)
{
	RowTime=time_;
}

FilesDetails::~FilesDetails()
{

}
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/