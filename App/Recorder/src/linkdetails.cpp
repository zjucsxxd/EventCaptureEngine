#include "linkdetails.h"
#include "referencelinkutil.h"

LinkDetails::LinkDetails(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	tempDB=SQLiteDataBase::dataBase();

	logSession=LoginUserSession::session();
	connect(ui.btnPlus, SIGNAL(clicked()),this,SLOT(btnPlus_Clicked()));
	connect(ui.btnMinus, SIGNAL(clicked()),this,SLOT(btnMinus_Clicked()));
	connect(ui.btnOK, SIGNAL(clicked()),this,SLOT(btnOK_Clicked()));
	connect(ui.tblURL,SIGNAL(cellClicked(int,int)),this,SLOT(tblURL_indexChange(int,int)));
}

void LinkDetails::tblURL_indexChange(int row_ ,int col)
{
	remRow=row_;
}
void LinkDetails::btnPlus_Clicked()
{
	ReferenceLink *rLink=new ReferenceLink(this);
	rLink->setRowTime(Rowtime);
	rLink->setTabIndex(tabIndx);
	delete rLink;

}

void LinkDetails::btnOK_Clicked()
{
	this->close();
}
void LinkDetails::btnMinus_Clicked()
{
	ReferenceLinkutil *rutil=new ReferenceLinkutil(this);

	if (ui.tblURL->rowCount()==0)
	{
		return;
	}
	try
	{
		int referenceLinkID=ui.tblURL->item(ui.tblURL->currentRow(),0)->data(Qt::UserRole).toInt();
		rutil->deleteReferenceLink(referenceLinkID,logSession->getDriverName());
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

	QString lURL=ui.tblURL->item(ui.tblURL->currentRow(),0)->text();
	QString lDescription=ui.tblURL->item(ui.tblURL->currentRow(),1)->text();

	tempDB->deleteLink(lURL,lDescription);

	ui.tblURL->removeRow(remRow);
}

void LinkDetails::readFile()
{
	ReferenceLinkutil *rutil=new ReferenceLinkutil(this);
	int k=0;

	for (int i=ui.tblURL->rowCount(); i >= 0; --i)
	{
		ui.tblURL->removeRow(i);	
	}

	try
	{
		QList<ReferenceLinkInfo> referencelink;
		QString url,description;
	
		referencelink=rutil->selectReferenceLinkByEvent(logSession->getEventID(),logSession->getDriverName());

		foreach(ReferenceLinkInfo rinfo,referencelink)
		{
			url=rinfo.getURL();
			description=rinfo.getURLDescription();

			QTableWidgetItem *item1=new QTableWidgetItem();	
			item1->setText(url);
			item1->setData(Qt::UserRole,rinfo.getURLID());
			QTableWidgetItem *item2=new QTableWidgetItem();
			item2->setText(description);

			ui.tblURL->insertRow(k);

			ui.tblURL->setItem(k,0,item1);		
			ui.tblURL->setItem(k,1,item2);
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


	QList<QStringList> rLink;
	rLink=tempDB->loadReferenceLink();

	if (rLink.count()<0)
	{
		return;
	}
	else
	{
		QString url,description;

		foreach(QStringList temp,rLink)
		{

			for (int r=0;r<temp.count();r++)
			{
				if (r==0)
				{
					url=temp[r];
				}
				if (r==1)
				{
					description=temp[r];
				}
			}


			QTableWidgetItem *item1=new QTableWidgetItem();	
			item1->setText(url);
			QTableWidgetItem *item2=new QTableWidgetItem();
			item2->setText(description);

			ui.tblURL->insertRow(k);

			ui.tblURL->setItem(k,0,item1);		
			ui.tblURL->setItem(k,1,item2);
			k++;
		}
	}

}
void LinkDetails::loadLinkDetails(QString name_,QString desc_)
{
	int rcount=ui.tblURL->rowCount();

	if (ui.tblURL->rowCount()==0)
	{
		QTableWidgetItem *item1=new QTableWidgetItem();	
		item1->setText(name_);
		QTableWidgetItem *item2=new QTableWidgetItem();
		item2->setText(desc_);

		ui.tblURL->insertRow(rcount);

		ui.tblURL->setItem(rcount,0,item1);		
		ui.tblURL->setItem(rcount,1,item2);
		rcount++;
	}
	else
	{
		for (int j=0;j<ui.tblURL->rowCount();j++)
		{
			QTableWidgetItem *itemtext=new QTableWidgetItem();
			QTableWidgetItem *itemtext2=new QTableWidgetItem();
		
			itemtext=ui.tblURL->item(j,0);
			itemtext2=ui.tblURL->item(j,1);

			QString text=itemtext->text();
			QString text1=itemtext2->text();
			
			if (text==name_ && text1==desc_)
			{
				return;
			}
		}

		QTableWidgetItem *item1=new QTableWidgetItem();	
		item1->setText(name_);

		QTableWidgetItem *item2=new QTableWidgetItem();
		item2->setText(desc_);
		ui.tblURL->insertRow(rcount);

		ui.tblURL->setItem(rcount,0,item1);		
		ui.tblURL->setItem(rcount,1,item2);
		
		rcount++;
	}	

}
void LinkDetails::setTab(int indx_)
{
	tabIndx=indx_;
	readFile();
	exec();

}
void LinkDetails::setRowTime(QString time_)
{
	Rowtime=time_;
	
}
LinkDetails::~LinkDetails()
{

}
