/*Owner & Copyrights: Vance King Saxbe. A.*//* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "customprogressbar.h"

#include "dlcrshared.h"

#include <QDir>
#include <QFile>

CopyFileProgress::CopyFileProgress(QWidget *parent)
	: QDialog(parent, QFlag(Qt::WA_StaticContents |Qt::WA_DeleteOnClose))
{
	ui.setupUi(this);
}
bool CopyFileProgress::copyFile(QString sourceFile_,QString destinationFile_)
{
	QFile src(sourceFile_);
	QFile dst(destinationFile_);

	QString path=destinationFile_.left(destinationFile_.lastIndexOf("\\"));
	QDir().mkpath(path);

	if(!src.open(QIODevice::ReadOnly))
	{
		PRINT_ERROR("can't open Source File "+src.errorString());	
		SHOW_INFOMESSAGE(tr("Copying Failed!!!"),tr("Unable to open source file."),tr("The file may be corrupted or cannot be accessed."),tr("Ok"));
		return false;
	}

	if(!dst.open(QIODevice::WriteOnly))
	{
		PRINT_ERROR("can't open Destination File "+src.errorString());	
		SHOW_INFOMESSAGE(tr("Copying Failed!!!"),tr("Unable to open destination file."),tr("Access to path ")+path+tr(" denied or server connection failed."),tr("Ok"));
		return false;
	}

	show();

	qint64 len = src.bytesAvailable()/1024;
	ui.progressBar->setRange(0,len);
	ui.progressBar->show();
	char ch;
	int val;
	while(!src.atEnd())
	{
		src.getChar(&ch);
		if(dst.putChar(ch))
		{
			val+=1; 
			qApp->processEvents();
			if(val>=1024)
			{
				val=0;
				ui.progressBar->setValue(ui.progressBar->value()+1);
			}
		}	
	}

	close();
}

bool CopyFileProgress::copyTranslationFile(QString sourceFile_,QString destinationFile_)
{
	QFile src(sourceFile_);
	QFile dst(destinationFile_);

	if(!src.open(QIODevice::ReadOnly))
	{
		PRINT_ERROR("can't open Source File "+src.errorString());	
		SHOW_INFOMESSAGE(tr("Copying Failed!!!"),tr("Unable to open source file."),tr("The file may be corrupted or cannot be accessed."),tr("Ok"));
		return false;
	}

	if(!dst.open(QIODevice::WriteOnly))
	{
		PRINT_ERROR("can't open Destination File "+src.errorString());	
		//SHOW_INFOMESSAGE(tr("Copying Failed!!!"),tr("Unable to open destination file."),tr("Access to path ")+path+tr(" denied or server connection failed."),tr("Ok"));
		return false;
	}

	show();

	qint64 len = src.bytesAvailable()/1024;
	ui.progressBar->setRange(0,len);
	ui.progressBar->show();
	char ch;
	int val;
	while(!src.atEnd())
	{
		src.getChar(&ch);
		if(dst.putChar(ch))
		{
			val+=1; 
			qApp->processEvents();
			if(val>=1024)
			{
				val=0;
				ui.progressBar->setValue(ui.progressBar->value()+1);
			}
		}	
	}

	close();
}
CopyFileProgress::~CopyFileProgress()
{

}
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/