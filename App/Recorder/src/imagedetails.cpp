#include "imagedetails.h"
#include "attachedfileutil.h"
#include "imagepreview.h"

ImageDetails::ImageDetails(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	logSession=LoginUserSession::session();
	tempDB=SQLiteDataBase::dataBase();

	connect(ui.btnPlus,		SIGNAL(clicked()),				this,SLOT(btnPlus_Clicked()));
	connect(ui.btnMinus,	SIGNAL(clicked()),				this,SLOT(btnMinus_Clicked()));
	connect(ui.btnOK,		SIGNAL(clicked()),				this,SLOT(btnOK_Clicked()));
	connect(ui.tblImages,	SIGNAL(cellClicked(int,int)),	this,SLOT(tblImages_indexChange(int,int)));

	ui.tblImages->setColumnWidth(0,300);
	ui.tblImages->setColumnWidth(2,100);
}

void ImageDetails::tblImages_indexChange(int row_ ,int col)
{	
	remRow=row_;

	ImagePreview *imgPreview=new ImagePreview(this);
	int attachedFileID=ui.tblImages->item(ui.tblImages->currentRow(),0)->data(Qt::UserRole).toInt();
	QString fName=ui.tblImages->item(ui.tblImages->currentRow(),0)->text();
	QString fSize=ui.tblImages->item(ui.tblImages->currentRow(),1)->text();
	QString fType=ui.tblImages->item(ui.tblImages->currentRow(),2)->text();
	imgPreview->setPreview(attachedFileID,fName,fSize,fType);
	delete imgPreview;
}
void ImageDetails::btnPlus_Clicked()
{
	ReferenceImage *rimage=new ReferenceImage(this);
	rimage->setReferenceTime(activeRowTime);
	rimage->setTabIndex(tabIndx);
	delete rimage;
}

void ImageDetails::btnOK_Clicked()
{
	this->close();
}

void ImageDetails::btnMinus_Clicked()
{
	AttachedFileUtil *autil=new AttachedFileUtil(this);

	if (ui.tblImages->rowCount()==0)
	{
		return;
	}
	try
	{
		int attachedFileID=ui.tblImages->item(ui.tblImages->currentRow(),0)->data(Qt::UserRole).toInt();
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

	QString fName=ui.tblImages->item(ui.tblImages->currentRow(),0)->text();
	QString fSize=ui.tblImages->item(ui.tblImages->currentRow(),1)->text();
	QString fType=ui.tblImages->item(ui.tblImages->currentRow(),2)->text();
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

	ui.tblImages->removeRow(remRow);
}

void ImageDetails::readFile()
{
	AttachedFileUtil *autil=new AttachedFileUtil(this);

	int k=0;

	for (int i=ui.tblImages->rowCount(); i >= 0; --i)
	{
		ui.tblImages->removeRow(i);	
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
				QTableWidgetItem *item1=new QTableWidgetItem();	
				item1->setText(FilePath);
				item1->setData(Qt::UserRole,ainfo.getAttachedFileID());

				QTableWidgetItem *item2=new QTableWidgetItem();
				item2->setText(size);

				QTableWidgetItem *item3=new QTableWidgetItem();
				item3->setText(type);

				//item3->setToolTip("<img src =\":/Recorder/Resources/current.png\">");
				//item3->setToolTip("<img src =\":/Recorder/Resources/current.png\" width=\"300\" height=\"400\">");

				ui.tblImages->insertRow(k);

				ui.tblImages->setItem(k,0,item1);		
				ui.tblImages->setItem(k,1,item2);
				ui.tblImages->setItem(k,2,item3);
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
				QTableWidgetItem *item1=new QTableWidgetItem();	
				item1->setText(FilePath);

				QTableWidgetItem *item2=new QTableWidgetItem();
				item2->setText(size);

				QTableWidgetItem *item3=new QTableWidgetItem();
				item3->setText(type);

				ui.tblImages->insertRow(k);

				ui.tblImages->setItem(k,0,item1);		
				ui.tblImages->setItem(k,1,item2);
				ui.tblImages->setItem(k,2,item3);
				k++;
			}
		}
	}
}

void ImageDetails::loadFileDetails(QString filepath_,QString size_,QString type_)
{
	int rcount=ui.tblImages->rowCount();

	QStringList temp=filepath_.split("/");
	QString fileName=temp.last();

	if (ui.tblImages->rowCount()==0)
	{
		QTableWidgetItem *item1=new QTableWidgetItem();	
		item1->setText(fileName);

		QTableWidgetItem *item2=new QTableWidgetItem();
		item2->setText(size_);

		QTableWidgetItem *item3=new QTableWidgetItem();
		item3->setText(type_);

		ui.tblImages->insertRow(rcount);

		ui.tblImages->setItem(rcount,0,item1);		
		ui.tblImages->setItem(rcount,1,item2);
		ui.tblImages->setItem(rcount,2,item3);
		rcount++;
	}
	else
	{
		for (int j=0;j<ui.tblImages->rowCount();j++)
		{
			QTableWidgetItem *itemtext=new  QTableWidgetItem();
			QTableWidgetItem *itemtext2=new QTableWidgetItem();
			QTableWidgetItem *itemtext3=new QTableWidgetItem();

			itemtext=ui.tblImages->item(j,0);
			itemtext2=ui.tblImages->item(j,1);
			itemtext3=ui.tblImages->item(j,2);

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

		ui.tblImages->insertRow(rcount);

		ui.tblImages->setItem(rcount,0,item1);		
		ui.tblImages->setItem(rcount,1,item2);
		ui.tblImages->setItem(rcount,2,item3);
		rcount++;
	}	
}

void ImageDetails::setTab(int indx_)
{
	tabIndx=indx_;
	readFile();
	exec();

}
void ImageDetails::setActiveRowTime(QString time_)
{
	activeRowTime=time_;
}
ImageDetails::~ImageDetails()
{

}
