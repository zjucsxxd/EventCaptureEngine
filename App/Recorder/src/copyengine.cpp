/* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "copyengine.h"

#include <QDir>
#include <QFile>

CopyEngine::CopyEngine(QObject *parent)
	: QThread(parent)
{
	exitStatus=false;
}

void CopyEngine::setSrcFile(QString srcFile_)
{
	srcFile=srcFile_;
}

void CopyEngine::setDstFile(QString dstFile_)
{
	dstFile=dstFile_;
}

void CopyEngine::copyFile(QString srcFile_, QString dstFile_)
{
	QFile src(srcFile_);
	QFile dst(dstFile_);

	if(srcFile_=="" || dstFile_=="" || !src.exists() || (src.exists() && dst.exists() && src.size()==dst.size()))
	{		
		errorString="source or destination file error";
		status=0;
		return;
	}

	QString path=dstFile_.left(dstFile_.lastIndexOf("\\"));
	QDir().mkpath(path);

	if(!src.open(QIODevice::ReadOnly))
	{
		errorString=src.errorString();
		status=0;
		return;
	}

	if(!dst.open(QIODevice::WriteOnly))
	{
		errorString=dst.errorString();
		status=0;
		return;
	}

	char ch;
	int val;

	while(!src.atEnd())
	{
		if(exitStatus==true) break;	

		if(!src.getChar(&ch)) 
		{
			break;
		}
		if(dst.putChar(ch))
		{	
			val+=1;

			if(val>=1024)
			{
				val=0;
				progress=progress+1;
			}
		}
		else
		{
			break;
		}		
	}

	if(src.size()==dst.size()) status=1;
	else status=0;
}
void CopyEngine::setExitStatus(bool status_)
{
	exitStatus=status_;
}
int CopyEngine::getProgress()
{
	return progress;
}
int CopyEngine::getStatus()
{
	return status;
}
QString CopyEngine::lastError()
{
	return errorString;
}

CopyEngine::~CopyEngine()
{

}
