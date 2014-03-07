/*Owner & Copyrights: Vance King Saxbe. A.*//* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "filedownloader.h"

#include "attachedfileutil.h"
#include "dlcrshared.h"
#include "backupserverconnection.h"
#include <QDesktopServices>
#include <QListWidgetItem>
#include <QFileInfo>
#include <QUrl>
#include "QFileDialog"

FileDownloader::FileDownloader(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	logSession=LoginUserSession::session();

	uiClosed=false;

	icoWordDoc=QIcon(QPixmap(":/Recorder/Resources/word_icon.png"));
	icoXls=QIcon(QPixmap(":/Recorder/Resources/xls-icon.png"));
	icoPdfDoc=QIcon(QPixmap(":/Recorder/Resources/file_pdf.png"));
	icoTextDoc=QIcon(QPixmap(":/Recorder/Resources/Text-Document-icon.png"));
	icoImage=QIcon(QPixmap(":/Recorder/Resources/jpeg-43.png"));
	icoVideo=QIcon(QPixmap(":/Recorder/Resources/Movie-file-icon.png"));
	icoAudio=QIcon(QPixmap(":/Recorder/Resources/audio-file-icon.png"));
	icoUnknown=QIcon(QPixmap(":/Recorder/Resources/unknown.png"));

	connect(ui.btnDownload,			SIGNAL(clicked()),		this,SLOT(btnDownload_click()));
	connect(ui.downloadListWidget,	SIGNAL(itemClicked(QListWidgetItem *)),		this,SLOT(downloadListWidget_click()));
	connect(ui.downloadListWidget,	SIGNAL(itemDoubleClicked(QListWidgetItem *)),		this,SLOT(downloadListWidget_doubleclick()));
	connect(ui.btnDelete,			SIGNAL(clicked()),		this,SLOT(btnDelete_click()));
	connect(ui.btnCancel,			SIGNAL(clicked()),		this,SLOT(btnClose_click()));

}
void FileDownloader::btnClose_click()
{
	uiClosed=true;
}
void FileDownloader::btnDownload_click()
{
	BackupServerConnection *conn=BackupServerConnection::connection();

	conn->createConnection(logSession->getServerIP(),logSession->getServerAdminName(),logSession->getServerAdminPassword());
	if (!conn->isOpen())
	{
		SHOW_MESSAGE(tr("Server Connection Failed!!!"),tr("Please create connection to attach files"),tr("Ok"));
		return;
	}

	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
		"/home",
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);
	if(dir.isEmpty()) return;
	AttachedFileUtil *util=new AttachedFileUtil(this);
	AttachedFileInfo info=util->loadAttachedFile(ui.downloadListWidget->currentItem()->data(Qt::UserRole).toInt(),logSession->getDriverName());
	QString sourceFile=info.getAttachedPath();
	sourceFile=sourceFile.replace("/","\\");
	QStringList tempList=sourceFile.split("\\");
	QString destinationFile=dir+"/"+tempList.last();

	QFile src(sourceFile);
	QFile dst(destinationFile);
	
	if(dst.exists())
	{
		SHOW_INFOMESSAGE(tr("Information"),tr("The filename is already existed!!!"), tr("Please rename existed file and download..."),tr("Ok"));
		return;
	}
	if(!src.open(QIODevice::ReadOnly))
	{
		PRINT_ERROR("can't open Source File "+src.errorString());
		SHOW_INFOMESSAGE(tr("Downloading Failed!!!"),tr("Unable to open source file."),tr("Access to path ")+sourceFile+tr(" denied or server connection failed or file is corrupted."),tr("Ok"));
		return;
	}
	if(!dst.open(QIODevice::WriteOnly))
	{
		PRINT_ERROR("can't create Destination File "+dst.errorString());
		SHOW_INFOMESSAGE(tr("Downloading Failed!!!"),tr("Unable to open destination file."),tr("Access to path ")+dir+tr(" denied or cannot be accessed."),tr("Ok"));
		return;
	}

	ui.progressBar->setHidden(false);	
	ui.btnDownload->setDisabled(true);
	ui.btnDelete->setDisabled(true);
	ui.downloadListWidget->setDisabled(true);

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

		if(uiClosed)
		{
			dst.remove();
			return;
		}
	}

	ui.btnDownload->setDisabled(false);
	ui.btnDelete->setDisabled(false);
	ui.downloadListWidget->setDisabled(false);

	if(src.size()!=dst.size())
	{
		SHOW_MESSAGE(tr("Error"),tr("Attaching file failed."),tr("Ok"));
	}

	close();
}
void FileDownloader::downloadListWidget_click()
{
	ui.btnDownload->setEnabled(true);
	ui.btnDelete->setEnabled(true);
}
void FileDownloader::downloadListWidget_doubleclick()
{
	AttachedFileUtil *util=new AttachedFileUtil(this);
	AttachedFileInfo info=util->loadAttachedFile(ui.downloadListWidget->currentItem()->data(Qt::UserRole).toInt(),logSession->getDriverName());
	QUrl url;

	url.setScheme("file");
	url.setUrl(info.getAttachedPath());
	QDesktopServices::openUrl(url);
}
void FileDownloader::refreshDialog(int eventID_)
{
	AttachedFileUtil *util=new AttachedFileUtil(this);
	
	try
	{
		ui.downloadListWidget->clear();

		QList<AttachedFileInfo> allFiles=util->selectAttachedFilesByEvent(eventID_,logSession->getDriverName());

		if (allFiles.size()==0)
		{
			SHOW_MESSAGE(tr("Attached Documents"),tr("No attached documents founded"),tr("Ok"));
			close();
			return;
		}

		foreach(AttachedFileInfo info,allFiles)
		{
			QListWidgetItem *item=new QListWidgetItem();

			QStringList tempList=info.getAttachedPath().split("\\");

			item->setText(tempList.last());
			item->setData(Qt::UserRole,info.getAttachedFileID());
			item->setToolTip(info.getCreatedDate().toString("dd/MM/yyyy"));

			QString suffix=QFileInfo(info.getAttachedPath()).suffix();

			if(suffix=="doc" || suffix=="docx")
			{
				item->setIcon(icoWordDoc);
			}
			else if(suffix=="xls" || suffix=="xlsx")
			{
				item->setIcon(icoXls);
			}
			else if(suffix=="pdf")
			{
				item->setIcon(icoPdfDoc);
			}
			else if(suffix=="txt")
			{
				item->setIcon(icoTextDoc);
			}
			else if(suffix=="jpg" || suffix=="jpeg" || suffix=="bmp" || suffix=="png" || suffix=="gif" || suffix=="tif")
			{
				item->setIcon(icoImage);
			}
			else if(suffix=="vob" || suffix=="wmv" || suffix=="xvid" || suffix.contains("mov") || suffix.contains("mpg") || suffix=="flv" || suffix=="dvx" || suffix=="avi" || suffix.contains("3gp"))
			{
				item->setIcon(icoVideo);
			}
			else if(suffix=="aac" || suffix=="mid" || suffix=="mp3" || suffix=="wav" || suffix=="wma")
			{
				item->setIcon(icoAudio);
			}
			else
			{
				item->setIcon(icoUnknown);
			}
			
			ui.downloadListWidget->addItem(item);
		}

		ui.btnDownload->setEnabled(false);
		ui.progressBar->setHidden(true);

		currentEvent=eventID_;

		exec();
	}
	catch(DBExeption e)//catching db exeptions
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
	catch(...)//catching unknown wxwptions
	{
		PRINT_ERROR("UNKnown ERROR");
	}

	delete util;
}

void FileDownloader::btnDelete_click()
{
	AttachedFileUtil *util=new AttachedFileUtil(this);
	int FileID=ui.downloadListWidget->currentItem()->data(Qt::UserRole).toInt();
	util->deleteAttachedFile(FileID,logSession->getDriverName());
	refreshDialog(currentEvent);
	delete util;
	ui.btnDelete->setEnabled(false);
}

FileDownloader::~FileDownloader()
{

}
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/