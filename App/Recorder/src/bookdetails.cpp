/* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "bookdetails.h"
#include "referencebookutil.h"

BookDetails::BookDetails(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	logSession=LoginUserSession::session();
	tempDB=SQLiteDataBase::dataBase();

	connect(ui.btnPlus, SIGNAL(clicked()),this,SLOT(btnPlus_Clicked()));
	connect(ui.btnMinus, SIGNAL(clicked()),this,SLOT(btnMinus_Clicked()));
	connect(ui.btnOK, SIGNAL(clicked()),this,SLOT(btnOK_Clicked()));
	connect(ui.tbllbook,SIGNAL(cellClicked(int,int)),this,SLOT(tblbook_indexChange(int,int)));
}
void BookDetails::tblbook_indexChange(int row_ ,int col)
{
	remRow=row_;
}
void BookDetails::btnPlus_Clicked()
{
	BookReference *rbook=new BookReference(this);
	rbook->setRowTime(Rowtime);
	rbook->setTabIndex(tabIndx);
	delete rbook;

}

void BookDetails::btnOK_Clicked()
{
	this->close();
}
void BookDetails::btnMinus_Clicked()
{
	ReferenceBookUtil *butil=new ReferenceBookUtil(this);

	if (ui.tbllbook->rowCount()==0)
	{
		return;
	}
	try
	{
		int referenceBookID=ui.tbllbook->item(ui.tbllbook->currentRow(),0)->data(Qt::UserRole).toInt();
		butil->deleteReferenceBook(referenceBookID,logSession->getDriverName());
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


	QString bName=ui.tbllbook->item(ui.tbllbook->currentRow(),0)->text();
	QString bAuthor=ui.tbllbook->item(ui.tbllbook->currentRow(),1)->text();
	QString bEdition=ui.tbllbook->item(ui.tbllbook->currentRow(),2)->text();
	QString bDescription=ui.tbllbook->item(ui.tbllbook->currentRow(),3)->text();
	
	tempDB->deleteBook(bName,bAuthor,bEdition,bDescription);

	ui.tbllbook->removeRow(remRow);

}

void BookDetails::readFile()
{
	ReferenceBookUtil *rutil=new ReferenceBookUtil(this);
	int k=0;

	for (int i=ui.tbllbook->rowCount(); i >= 0; --i)
	{
		ui.tbllbook->removeRow(i);	
	}

	try
	{
		QList<ReferenceBookinfo> referencebooks;
		QString name,author,edition,description;
	
		referencebooks=rutil->selectReferenceBookByEvent(logSession->getEventID(),logSession->getDriverName());

		foreach(ReferenceBookinfo rinfo,referencebooks)
		{
			name=rinfo.getBookName();
			author=rinfo.getAuthor();
			edition=rinfo.getEdition();
			description=rinfo.getBookDecription();

			QTableWidgetItem *item1=new QTableWidgetItem();	
			item1->setText(name);
			item1->setData(Qt::UserRole,rinfo.getReferenceBookID());
			QTableWidgetItem *item2=new QTableWidgetItem();
			item2->setText(author);
			QTableWidgetItem *item3=new QTableWidgetItem();
			item3->setText(edition);
			QTableWidgetItem *item4=new QTableWidgetItem();
			item4->setText(description);

			ui.tbllbook->insertRow(k);

			ui.tbllbook->setItem(k,0,item1);		
			ui.tbllbook->setItem(k,1,item2);
			ui.tbllbook->setItem(k,2,item3);
			ui.tbllbook->setItem(k,3,item4);

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

	QList<QStringList> rBookList;
	rBookList=tempDB->loadReferenceBook();

	if (rBookList.count()<0)
	{
		return;
	}
	else
	{
		QString name,author,edition,description;

		foreach(QStringList temp,rBookList)
		{

			for (int r=0;r<temp.count();r++)
			{
				if (r==0)
				{
					name=temp[r];
				}
				if (r==1)
				{
					author=temp[r];
				}
				if (r==2)
				{
					edition=temp[r];
				}
				if (r==3)
				{
					description==temp[r];
				}
			}


			QTableWidgetItem *item1=new QTableWidgetItem();	
			item1->setText(name);
			QTableWidgetItem *item2=new QTableWidgetItem();
			item2->setText(author);
			QTableWidgetItem *item3=new QTableWidgetItem();
			item3->setText(edition);
			QTableWidgetItem *item4=new QTableWidgetItem();
			item4->setText(description);

			ui.tbllbook->insertRow(k);

			ui.tbllbook->setItem(k,0,item1);		
			ui.tbllbook->setItem(k,1,item2);
			ui.tbllbook->setItem(k,2,item3);
			ui.tbllbook->setItem(k,3,item4);

			k++;

		}
	}

}
void BookDetails::loadBookDetails(QString name_,QString author_,QString edition_,QString desc_)
{
	int rcount=ui.tbllbook->rowCount();

	if (ui.tbllbook->rowCount()==0)
	{
		QTableWidgetItem *item1=new QTableWidgetItem();	
		item1->setText(name_);
		QTableWidgetItem *item2=new QTableWidgetItem();
		item2->setText(author_);
		QTableWidgetItem *item3=new QTableWidgetItem();
		item3->setText(edition_);
		QTableWidgetItem *item4=new QTableWidgetItem();
		item4->setText(desc_);

		ui.tbllbook->insertRow(rcount);

		ui.tbllbook->setItem(rcount,0,item1);		
		ui.tbllbook->setItem(rcount,1,item2);
		ui.tbllbook->setItem(rcount,2,item3);
		ui.tbllbook->setItem(rcount,3,item4);
		rcount++;
	}
	else
	{
		for (int j=0;j<ui.tbllbook->rowCount();j++)
		{
			QTableWidgetItem *itemtext=new QTableWidgetItem();
			QTableWidgetItem *itemtext2=new QTableWidgetItem();
			QTableWidgetItem *itemtext3=new QTableWidgetItem();
			itemtext=ui.tbllbook->item(j,0);
			itemtext2=ui.tbllbook->item(j,1);
			itemtext3=ui.tbllbook->item(j,2);

			QString text=itemtext->text();
			QString text1=itemtext2->text();
			QString text2=itemtext3->text();

			if (text==name_ && text1==author_ && text2==edition_ )
			{
				return;
			}
		}

		QTableWidgetItem *item1=new QTableWidgetItem();	
		item1->setText(name_);
		QTableWidgetItem *item2=new QTableWidgetItem();
		item2->setText(author_);
		QTableWidgetItem *item3=new QTableWidgetItem();
		item3->setText(edition_);
		QTableWidgetItem *item4=new QTableWidgetItem();
		item4->setText(desc_);
		ui.tbllbook->insertRow(rcount);

		ui.tbllbook->setItem(rcount,0,item1);		
		ui.tbllbook->setItem(rcount,1,item2);
		ui.tbllbook->setItem(rcount,2,item3);
		ui.tbllbook->setItem(rcount,3,item4);
		rcount++;
	}	

}
void BookDetails::setTab(int indx_)
{
	tabIndx=indx_;
	readFile();
	exec();

}
void BookDetails::setRowTime(QString time_)
{
	Rowtime=time_;
}
BookDetails::~BookDetails()
{

}
