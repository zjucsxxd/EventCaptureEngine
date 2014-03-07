/*Owner & Copyrights: Vance King Saxbe. A.*//* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "imagepreview.h"
#include "attachedfileutil.h"
#include <QLabel>
#include <QImage>

ImagePreview::ImagePreview(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	logSession=LoginUserSession::session();
	tempDB=SQLiteDataBase::dataBase();
}
void ImagePreview::setPreview(int attachedFileID,QString fileName,QString fileSize,QString fileType)
{
	AttachedFileUtil *autil=new AttachedFileUtil(this);
	AttachedFileInfo ainfo;
	try
	{

		ainfo=autil->loadAttachedFile(attachedFileID,logSession->getDriverName());
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
	delete autil;

	QLabel *imageLabel = new QLabel(this);
	QImage image(ainfo.getAttachedPath());
	imageLabel->setPixmap(QPixmap::fromImage(image));
	imageLabel->setAlignment(Qt::AlignCenter);

	ui.scrollArea->setBackgroundRole(QPalette::Shadow);
	ui.scrollArea->setWidget(imageLabel);

	QStringList fNameList;

	fNameList=tempDB->loadReferenceFile(fileSize,fileType);

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
			if (file==fileName)
			{
				QLabel *imageLabel = new QLabel(this);
				QImage image(fNameList[i]);
				imageLabel->setPixmap(QPixmap::fromImage(image));
				imageLabel->setAlignment(Qt::AlignCenter);

				ui.scrollArea->setBackgroundRole(QPalette::Shadow);
				ui.scrollArea->setWidget(imageLabel);
			}
		}
	}
	exec();
}
ImagePreview::~ImagePreview()
{

}
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/