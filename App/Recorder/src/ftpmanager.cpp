/*Owner & Copyrights: Vance King Saxbe. A.*//* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "ftpmanager.h"
#include <Windows.h>
#include <QDir>
#include <QDebug>

FTPManager::FTPManager(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ftp = NULL;

	ui.txtHostName->setLayoutDirection(Qt::LeftToRight);
	ui.lblStatus->setText(tr("Not Connected"));

	ui.fileList->setEnabled(false);
	ui.btnBack->setEnabled(false);
	ui.btnDelete->setVisible(false);
	ui.lblPassword->setVisible(false);
	ui.txtPassword->setVisible(false);
	ui.btnUpload->setEnabled(false);
	ui.btnDownload->setEnabled(false);
	ui.fileList->setRootIsDecorated(false);
	ui.fileList->setHeaderLabels(QStringList() << tr("Name") << tr("Size") << tr("Owner") << tr("Group") << tr("Time"));
	ui.fileList->header()->setStretchLastSection(false);
	progressDialog = new QProgressDialog(this);
	connect(ui.fileList,         SIGNAL(itemActivated(QTreeWidgetItem *, int)),
		this, SLOT(processItem(QTreeWidgetItem *, int)));
	connect(ui.fileList,         SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
		this, SLOT(enableDownloadButton()));
	connect(progressDialog,      SIGNAL(canceled()), this, SLOT(cancelDownload()));
	connect(ui.btnConnect,       SIGNAL(clicked()),  this, SLOT(connectOrDisconnect()));
	connect(ui.btnBack, SIGNAL(clicked()),  this, SLOT(cdToParent()));
	connect(ui.btnDownload,      SIGNAL(clicked()),  this, SLOT(downloadFile()));
	connect(ui.btnUpload,        SIGNAL(clicked()),  this, SLOT(Upload()));
	connect(ui.btnQuit,          SIGNAL(clicked()),  this, SLOT(close()));
	connect(ui.btnDelete,          SIGNAL(clicked()),  this, SLOT(deleteFileFromFTP()));
	setWindowTitle("FTP");
	timerUp =new QTimer();
	connect(timerUp,             SIGNAL(timeout()),   this, SLOT(uploadNext()));
	
	ui.fileList->setColumnWidth(0,311);
	ui.fileList->setColumnWidth(1,100);
	ui.fileList->setColumnWidth(2,100);
	ui.fileList->setColumnWidth(3,100);
	ui.fileList->setColumnWidth(4,100);
}
void FTPManager::refresh(ftpDetails details_)
{
	ui.txtHostName->setText("ftp://"+details_.hostName);
	ui.txtUserName->setText(details_.userName);
	ui.txtPassword->setText(details_.password);
	ui.txtPort->setText(QString::number(details_.portNo));
}
bool FTPManager::connectToServer()
{
	if (connectOrDisconnect()==1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/* For getting The size */
QSize FTPManager::sizeHint() const
{
	return QSize(500, 300);
}

/* Connect Disconnect Routine */
int FTPManager::connectOrDisconnect()
{
	if (ftp) 
	{
		ftp->abort();
		ftp->deleteLater();
		ftp = 0;
		ui.fileList->clear();
		ui.fileList->setEnabled(false);
		ui.btnUpload->setEnabled(false);
		ui.btnBack->setEnabled(false);
		ui.btnDownload->setEnabled(false);
		ui.btnConnect->setEnabled(true);
		ui.btnConnect->setText("Connect");

#ifndef QT_NO_CURSOR
		setCursor(Qt::ArrowCursor);
#endif
		return 0;
	}

#ifndef QT_NO_CURSOR
	setCursor(Qt::WaitCursor);
#endif
	ftp = new QFtp(this);
	connect(ftp, SIGNAL(commandFinished(int, bool)),this, SLOT(ftpCommandFinished(int, bool)));
	connect(ftp, SIGNAL(listInfo(const QUrlInfo &)),this, SLOT(addToList(const QUrlInfo &)));
	connect(ftp, SIGNAL(dataTransferProgress(qint64, qint64)),this, SLOT(updateDataTransferProgress(qint64, qint64)));
	connect(ftp, SIGNAL(done ( bool  )),this, SLOT(ftpdone( bool)));
	ui.fileList->clear();
	currentPath.clear();
	isDirectory.clear();
	QUrl url(ui.txtHostName->text());
	url.setUserName(ui.txtUserName->text()); 
	url.setPassword(ui.txtPassword->text()); 
	if (!url.isValid() || url.scheme().toLower() != QLatin1String("ftp"))
	{
		ftp->connectToHost(ui.txtHostName->text(), ui.txtPort->text().toInt());
		ftp->login();
	} 
	else 
	{
		ftp->connectToHost(url.host(), url.port(ui.txtPort->text().toInt()));
		if (!url.userName().isEmpty())
			ftp->login(QUrl::fromPercentEncoding(url.userName().toLatin1()), url.password());
		else
			ftp->login();
		if (!url.path().isEmpty())
			ftp->cd(url.path());
	}
	ui.fileList->setEnabled(true);
	ui.btnUpload->setEnabled(true);
	ui.btnConnect->setEnabled(false);
	ui.btnConnect->setText(tr("Disconnect"));
	ui.lblStatus->setText(tr("Connecting to FTP server %1...").arg(ui.txtHostName->text()));
	return 1;
}
void FTPManager::reload()
{

// 	ui.fileList->clear();
// 	isDirectory.clear();
// 
// 	if (currentPath.isEmpty())
// 	{
// 		ftp->cd("/");
// 	}
// 	else
// 	{
// 		ftp->cd(currentPath);
// 	}	
// 
// 	ftp->list();

   	delete ftp;
   
   #ifndef QT_NO_CURSOR
   	setCursor(Qt::WaitCursor);
   #endif
   	ftp = new QFtp(this);
   	connect(ftp, SIGNAL(commandFinished(int, bool)),this, SLOT(ftpCommandFinished(int, bool)));
   	connect(ftp, SIGNAL(listInfo(const QUrlInfo &)),this, SLOT(addToList(const QUrlInfo &)));
   	connect(ftp, SIGNAL(dataTransferProgress(qint64, qint64)),this, SLOT(updateDataTransferProgress(qint64, qint64)));
   	connect(ftp, SIGNAL(done ( bool  )),this, SLOT(ftpdone( bool)));
   	ui.fileList->clear();
   	currentPath.clear();
   	isDirectory.clear();
   	QUrl url(ui.txtHostName->text());
   	url.setUserName(ui.txtUserName->text()); 
   	url.setPassword(ui.txtPassword->text()); 
   	if (!url.isValid() || url.scheme().toLower() != QLatin1String("ftp"))
   	{
   		ftp->connectToHost(ui.txtHostName->text(), ui.txtPort->text().toInt());
   		ftp->login();
   	} 
   	else 
   	{
   		ftp->connectToHost(url.host(), url.port(ui.txtPort->text().toInt()));
   		if (!url.userName().isEmpty())
   			ftp->login(QUrl::fromPercentEncoding(url.userName().toLatin1()), url.password());
   		else
   			ftp->login();
   		if (!url.path().isEmpty())
   			ftp->cd(url.path());
   	}

}
/*Download the file */
void FTPManager::downloadFile()
{
	
	QString fileName = ui.fileList->currentItem()->text(0);

	QString filePath=QDir::homePath()+"/My Documents/Downloads/"+fileName;
	if (QFile::exists(filePath)) 
	{
		QMessageBox::information(this, "FTP",
			tr("There already exists a file called %1 in "
			"the current directory.").arg(fileName));
		return;
	}
	file = new QFile(filePath);
	if (!file->open(QIODevice::WriteOnly)) 
	{
		QMessageBox::information(this, "FTP",
			tr("Unable to save the file %1: %2.").arg(fileName).arg(file->errorString()));
		delete file;
		return;
	}
	ftp->get(ui.fileList->currentItem()->text(0), file);
	progressDialog->setLabelText(tr("Downloading %1...").arg(filePath));
	ui.btnDownload->setEnabled(false);
	progressDialog->exec();
}

/* For canceling the download */
void FTPManager::cancelDownload()
{
	ftp->abort();
}
void FTPManager::deleteFileFromFTP()
{

	ftp->remove("/"+ui.fileList->currentItem()->text(0));

	reload();
}
/* FTP Command */
void FTPManager::ftpCommandFinished(int, bool error)
{
#ifndef QT_NO_CURSOR
	setCursor(Qt::ArrowCursor);
#endif
	if (ftp->currentCommand() == QFtp::ConnectToHost) 
	{
		if (error)
		{
			QMessageBox::information(this, tr("FTP"),
				tr("Unable to connect to the FTP server "
				"at %1. Please check that the host "
				"name is correct.")
				.arg(ui.txtHostName->text()));
			connectOrDisconnect();
			return;
		}
		ui.lblStatus->setText(tr("Logged onto %1.").arg(ui.txtHostName->text()));
		ui.fileList->setFocus();
		ui.btnDownload->setDefault(true);
		ui.btnConnect->setEnabled(true);
		return;
	}
	if (ftp->currentCommand() == QFtp::Login)
		ftp->list();	
	if (ftp->currentCommand() == QFtp::Put) 
	{
		if (ftp->currentCommand() == QFtp::UnknownError) 
		{
			ui.lblStatus->setText(tr("Canceled Upload of %1.").arg(file->errorString()));
			printf("\n%s",file->errorString().toAscii().data());

			file->close();
			file->remove();
		} 
		else
		{	
			ui.lblStatus->setText(tr("Uploading %1 to current directory.").arg(file->fileName()));
			printf("\n%s",file->fileName().toAscii().data());
			ftp->list();
			file->close();
		}
		delete file;
		enableDownloadButton();
		progressDialog->hide();
	} 
	else if (ftp->currentCommand() == QFtp::List) 
	{
		if (isDirectory.isEmpty()) 
		{
			ui.fileList->addTopLevelItem(new QTreeWidgetItem(QStringList() << tr("<empty>")));
			ui.fileList->setEnabled(false);
		}
	}
}

/* FTP completes */
void FTPManager::ftpdone( bool error)
{
	if(!error)
	{
		if(UploadFileInfo.size()>0)
		{
			printf("\nWe have Done and File Exists");
			QTreeWidgetItem *item = new QTreeWidgetItem;
			item->setText(0, UploadFileInfo.fileName ());
			item->setText(1, QString::number(UploadFileInfo.size()));
			if(UploadFileInfo.owner()==NULL) return;
			if(UploadFileInfo.group()==NULL) return;
			item->setText(2, UploadFileInfo.owner());
			item->setText(3, UploadFileInfo.group());
			item->setText(4, UploadFileInfo.lastModified().toString("MMM dd yyyy"));

			QPixmap pixmap(UploadFileInfo.isDir() ? ":/Recorder/Resources/dir.png" : ":/Recorder/Resources/file.png");
			item->setIcon(0, pixmap);

			isDirectory[UploadFileInfo.fileName()] = UploadFileInfo.isDir();
			ui.fileList->addTopLevelItem(item);

			if (!ui.fileList->currentItem())
			{
				ui.fileList->setCurrentItem(ui.fileList->topLevelItem(0));
				ui.fileList->setEnabled(true);
			}
		}

	}
}

/* Adding File to the List */
void FTPManager::addToList(const QUrlInfo &urlInfo)
{
	QTreeWidgetItem *item = new QTreeWidgetItem;

	item->setText(0, urlInfo.name());
	item->setText(1, QString::number(urlInfo.size()));
	item->setText(2, urlInfo.owner());
	item->setText(3, urlInfo.group());
	item->setText(4, urlInfo.lastModified().toString("MMM dd yyyy"));
	QPixmap dirpixmap(":/Recorder/Resources/dir.png");
	QPixmap filepixmap(":/Recorder/Resources/file.png");

	if (  urlInfo.isDir()==true)
	{
		item->setIcon(0, dirpixmap);
	}
	else
	{
		item->setIcon(0, filepixmap);
	}
	isDirectory[urlInfo.name()] = urlInfo.isDir();
	ui.fileList->addTopLevelItem(item);

	if (!ui.fileList->currentItem()) 
	{
		ui.fileList->setCurrentItem(ui.fileList->topLevelItem(0));
		ui.fileList->setEnabled(true);
	}
}

/* Sorting Folder Name up */
void FTPManager::folderNameUp(QString folder)
{
	folderUploading = folder;
	QDir directory(folder);
	num=0;
	forMkdir.append(folder);
	forMkdir = forMkdir.right(forMkdir.size()-forMkdir.lastIndexOf("/")-1);
	printf("\nThe Index forMkdir :%s",forMkdir.toAscii().data());
	folderFiles = directory.entryList();
	totalFolders = folderFiles.size();
	int connect = connectOrDisconnect();
	printf("\n connect---> %d",connect);
	printf("\ntotalFolders ---> %d",totalFolders);
	printf("\nfolderFiles===> %d",folderFiles);
	ftp->mkdir(forMkdir);
	ftp->cd(forMkdir);
	timerUp->start(1000);

}

/*Uploading the next file */
void FTPManager::uploadNext()
{
	if(totalFolders>num)
	{
		if(forMkdir=="."||forMkdir=="..")
		{
			printf("\n. & .. Invalid");
		}
		else
		{
			QString currentfolder = folderUploading+"/"+folderFiles[num];
			printf("\nUploadNext() The folders : %s",currentfolder.toAscii().data());
			Upload();
			num++;
		}
	}
	else
	{
		timerUp->stop();
	}
}

/* Uploading the media file */ 
void FTPManager::Upload()
{	
	QString path = QFileDialog::getOpenFileName(this, "Open File","/home","Text files (*.txt *.pdf *.doc *.docx *.xlsx *.xls);;"
		"Image files (*.jpg *.jpeg *.png *.bmp *.gif *.tif);;"
		"Video files (*.vob *.wmv *.xvid *.mov *.mpg *.flv *.dvx *.avi *.3gp);;"
		"Audio files (*.aac *.mid *.mp3 *.wav *.wma);;"
		"Any files (*)");

	if(path.isEmpty()) return;

	QString filename = path;

	UploadFileInfo.setFile(QFile(filename));

	file =new QFile(filename);
	if (!file->open(QIODevice::ReadOnly )) 
	{
		return;
	}
	QByteArray data =file->readAll();
	file->close();
	ftp->put(data,UploadFileInfo.fileName(),QFtp::Ascii); 
/*	qDebug()<<"Upload()"<<ftp->put(data, UploadFileInfo.fileName(), QFtp::Ascii); */
	
	ftp->close();
  	progressDialog->setLabelText(tr("Uploading %1...").arg(fileName));
  	progressDialog->exec();
	reload();

}

void FTPManager::processItem(QTreeWidgetItem *item, int /*column*/)
{
	QString name = item->text(0);
	if (isDirectory.value(name)) 
	{
		ui.fileList->clear();
		isDirectory.clear();
		currentPath += "/" + name;
		ftp->cd(name);
		ftp->list();
		ui.btnBack->setEnabled(true);
#ifndef QT_NO_CURSOR
		setCursor(Qt::WaitCursor);
#endif
		return;
	}
}

/* Moving to parent Direcctory  */
void FTPManager::cdToParent()
{
#ifndef QT_NO_CURSOR
	setCursor(Qt::WaitCursor);
#endif
	ui.fileList->clear();
	isDirectory.clear();
	currentPath = currentPath.left(currentPath.lastIndexOf('/'));
	if (currentPath.isEmpty()) {
		ui.btnBack->setEnabled(false);
		ftp->cd("/");
	} 
	else 
	{
		ftp->cd(currentPath);
	}
	ftp->list();
}

/* Activating Enable Button*/
void FTPManager::enableDownloadButton()
{
	QTreeWidgetItem *current = ui.fileList->currentItem();
	if (current)
	{
		QString currentFile = current->text(0);
		ui.btnDownload->setEnabled(!isDirectory.value(currentFile));
		ui.btnDelete->setEnabled(!isDirectory.value(currentFile));
	} 
	else
	{
		ui.btnDownload->setEnabled(false);
		ui.btnDelete->setEnabled(false);
	}
}

/* Updating Transfer */
void FTPManager::updateDataTransferProgress(qint64 readBytes,qint64 totalBytes)
{
	printf("\nreadBytes %d",(int)readBytes);
	printf("\ntotalBytes %d",(int)totalBytes);
	progressDialog->setMaximum(totalBytes);
	progressDialog->setValue(readBytes);
}

/* Keypress Event to stop */
void FTPManager::keyPressEvent(QKeyEvent *e)
{
	if(e->key()==Qt::Key_Escape)
	{
		this->close();
	}
}

/* Closing the event by hiding the button */ 
void FTPManager::closeEvent(QCloseEvent *event)
{
	event = NULL;
	emit hideFTPbutton();
}
FTPManager::~FTPManager()
{

}
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/