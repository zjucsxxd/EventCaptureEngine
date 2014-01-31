#include "writetodisk.h"

#include "locationutil.h"
#include "roomutil.h"
#include "eventtypeutil.h"
#include "roomeventtypeutil.h"
#include "userroomeventtypeutil.h"
#include "userutil.h"
#include "sessionutil.h"
#include "appsettingsutil.h"
#include "eventutil.h"
#include "fileutil.h"
#include "systemfetch.h"
#include "pcutil.h"
#include "appsettingsutil.h"
#include "hashmanager.h"
#include "eventnoteutil.h"

#include <QDir>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QTextDocument>
#include <QPrinter>
#include <QQueue>

WriteToDisk::WriteToDisk(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	sharedObj=new DLCRShared();
	logSession=LoginUserSession::session();
	srvr=BackupServerConnection::connection();
	driveAnalizer=new DiskAnalizer(this);
	copyThread=NULL;

	icoSession=QIcon(QPixmap(":/Recorder/Resources/session-manager.png"));
	icoOpen=QIcon(QPixmap(":/Recorder/Resources/webcamera.png"));
	icoClosed=QIcon(QPixmap(":/Recorder/Resources/closed.png"));

	connect(ui.cmbLocationName,		SIGNAL(currentIndexChanged(int)),			this,SLOT(cmbLocationName_indexChanged(int)));
	connect(ui.cmbRoomName,			SIGNAL(currentIndexChanged(int)),			this,SLOT(cmbRoomName_indexChanged(int)));
	connect(ui.tblEventDetails,		SIGNAL(cellClicked(int,int)),				this,SLOT(eventDetails_cellClicked(int,int)));
	connect(ui.tblEventDetails,		SIGNAL(currentCellChanged(int,int,int,int)),this, SLOT(tblEventDetails_cellChanged()));
	connect(ui.cmbSearchCategory,	SIGNAL(currentIndexChanged(int)),			this,SLOT(cmbSearchCategory_indexChanged(int)));
	connect(ui.txtSearch,			SIGNAL(textChanged(QString)),				this,SLOT(txtSearch_textChanged(QString)));
	connect(ui.listSessions,		SIGNAL(itemClicked ( QListWidgetItem *)),	this,SLOT(listSessionItemClicked()));
	connect(ui.listSessionsToCopy,	SIGNAL(itemClicked ( QListWidgetItem *)),	this,SLOT(copySessionListItemClicked()));
	connect(ui.btnAdd,				SIGNAL(clicked()),							this,SLOT(btnAddClicked()));
	connect(ui.btnRemove,			SIGNAL(clicked()),							this,SLOT(btnRemoveClicked()));
	connect(ui.btnBrows,			SIGNAL(clicked()),							this,SLOT(btnBrowsClicked()));
	connect(ui.btnWrite,			SIGNAL(clicked()),							this,SLOT(btnWriteClicked()));
	connect(ui.cbxOpticalStatus,	SIGNAL(stateChanged(int)),					this,SLOT(cbxOpticalChanged(int)));
	connect(ui.cbxRemovableStatus,	SIGNAL(stateChanged(int)),					this,SLOT(cbxRemovableChanged(int)));
	connect(ui.availableSpace,		SIGNAL(valueChanged(int)),					this,SLOT(showAvailability(int)));

	loadUserDetails();
	readSrvrSettings();
	removeDirectory(QDir::homePath()+"/Application Data/Evidencer_Temp");

	QString drive=QDir::homePath().left(3);
	double bytesAvailable=driveAnalizer->getFreeSpace(drive);
	qint64 avail=(qint64)(bytesAvailable)/1024;
	avalableSpace=avail;
	ui.availableSpace->setRange(0,avalableSpace);
	totalAdded=0;
	isRunning=false;

	ui.frameBrowsDisk->setHidden(true);
	ui.lblStatus->setHidden(true);
	ui.frameTotalProgress->setHidden(true);
	ui.btnAdd->setEnabled(false);
	ui.btnRemove->setEnabled(false);
	ui.btnWrite->setEnabled(false);

	if(logSession->canWriteOpticalDisk() && NOT logSession->canWriteRemovableDisk())
	{
		ui.cbxOpticalStatus->setChecked(true);
		ui.cbxOpticalStatus->setEnabled(false);
		ui.cbxRemovableStatus->setVisible(false);
	}

	if(NOT logSession->canWriteOpticalDisk() && logSession->canWriteRemovableDisk())
	{
		ui.cbxRemovableStatus->setChecked(true);
		ui.cbxRemovableStatus->setEnabled(false);
		ui.cbxOpticalStatus->setVisible(false);
	}
}

void WriteToDisk::loadUserDetails()
{
	ui.txtSearch->clear();

	loadLocations();

	LocationUtil *lUtil=new LocationUtil(this);
	RoomUtil *rUtil=new RoomUtil(this);

	try
	{
		LocationInfo lInfo=lUtil->loadLocation(logSession->getCurrentLocation(),logSession->getDriverName());
		RoomInfo rInfo=rUtil->loadRoom(logSession->getCurrentRoom(),logSession->getDriverName());

		int locationIndex=ui.cmbLocationName->findText(lInfo.getLocationName(),Qt::MatchExactly | Qt::MatchCaseSensitive);
		ui.cmbLocationName->setCurrentIndex(locationIndex);

		int roomIndex=ui.cmbRoomName->findText(rInfo.getRoomName(),Qt::MatchExactly | Qt::MatchCaseSensitive);
		ui.cmbRoomName->setCurrentIndex(roomIndex);
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

	delete lUtil;
	delete rUtil;
}
void WriteToDisk::loadLocations()
{
	LocationUtil *util=new LocationUtil(this);

	try
	{
		ui.cmbLocationName->blockSignals(true);
		ui.cmbLocationName->clear();
		ui.cmbLocationName->blockSignals(false);

		QList<int> userLocationList=logSession->getUserLocationIDs();
		for(int i=0;i<userLocationList.size();i++)
		{
			LocationInfo info=util->loadLocation(userLocationList[i],logSession->getDriverName());
			ui.cmbLocationName->insertItem(i,info.getLocationName(),info.getLocationID());
		}
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

	cmbLocationName_indexChanged(ui.cmbLocationName->currentIndex());
}

bool WriteToDisk::isAssigned(int userID_,int roomID_,int eventTypeID_)
{			

	UserRoomEventTypeUtil *ureUtil=new UserRoomEventTypeUtil(this);
	bool assignedStatus;

	try
	{
		RoomEventTypeUtil *util=new RoomEventTypeUtil(this);

		//selecting room event id by room and event id's
		int roomEventTypeID=util->selectRoomEventTypeID(roomID_,eventTypeID_,logSession->getDriverName());

		//loading info by user id and room event type id
		UserRoomEventTypeInfo info=ureUtil->loadUserEventType(userID_,roomEventTypeID,logSession->getDriverName());

		//checking visibility of event type
		if (info.getVisibility()!=0)
		{
			assignedStatus=true;
		}
		else
		{
			assignedStatus=false;
		}

		delete util;
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

	delete ureUtil;

	return assignedStatus;

}

void WriteToDisk::loadAllEvents()
{
	EventUtil *util=new EventUtil(this);

	try
	{
		int roomID=ui.cmbRoomName->itemData(ui.cmbRoomName->currentIndex(),Qt::UserRole).toInt();

		allEvents.clear();

		QList<EventInfo> totalEvents=util->loadEventsOFRoom(roomID,logSession->getDriverName());

		int k=0;

		for(int i=0;i<totalEvents.size();i++)
		{			
			if (totalEvents[i].getEventStatus()=="Closed")
			{
				if(logSession->canViewClosedEvent()) allEvents<<totalEvents[i];
				else continue;
			}
			else
			{
				allEvents<<totalEvents[i];
			}

			k++;
		}
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
void WriteToDisk::loadRescentEvents()
{
	EventUtil *util=new EventUtil(this);

	try
	{
		int roomID=ui.cmbRoomName->itemData(ui.cmbRoomName->currentIndex(),Qt::UserRole).toInt();

		allEvents.clear();

		QList<EventInfo> totalEvents=util->loadEventsOFRoom(roomID,logSession->getDriverName());

		int k=0;

		for(int i=totalEvents.size()-1;i>=0;i--)
		{

			if (totalEvents[i].getEventStatus()=="Closed")
			{
				if(logSession->canViewClosedEvent()) allEvents<<totalEvents[i];
				else continue;
			}
			else
			{
				allEvents<<totalEvents[i];
			}

			if(k==6) break;

			k++;
		}
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
void WriteToDisk::listEvents(QString str_)
{

	int k=0;

	foreach(EventInfo info,allEvents)
	{
		QTableWidgetItem *itemEventNo,*itemEventTitle,*itemEventDescription,*itemCreatedDate,*itemFinishedDate,*itemEventType,*itemStaus,*vHItem;

		itemEventNo=new QTableWidgetItem();
		itemEventTitle=new QTableWidgetItem();
		itemEventDescription=new QTableWidgetItem();
		itemCreatedDate=new QTableWidgetItem();
		itemFinishedDate=new QTableWidgetItem();
		itemEventType=new QTableWidgetItem();
		itemStaus=new QTableWidgetItem();
		vHItem=new QTableWidgetItem();

		itemEventNo->setText(info.getEventNo());
		itemEventNo->setData(Qt::UserRole,info.getEventID());
		itemEventTitle->setText(info.getEventName());
		itemEventDescription->setText(info.getEventDescription());
		itemCreatedDate->setText(info.getCreatedDate().toString("d-M-yyyy"));

		if (info.getEventStatus()!="Closed")
		{
			itemFinishedDate->setText("--");
			itemEventNo->setIcon(icoOpen);
			//vHItem->setIcon(icoOpen);
		}
		else 
		{
			if(logSession->canViewClosedEvent()) itemFinishedDate->setText(info.getFinishedDate().toString("d-M-yyyy"));
			else continue;
			itemEventNo->setIcon(icoClosed);
			//vHItem->setIcon(icoClosed);
		}

		EventTypeUtil *etUtil=new EventTypeUtil(this);
		itemEventType->setText(etUtil->loadEventType(info.getEventTypeID(),logSession->getDriverName()).getEventTypeName());
		itemEventType->setData(Qt::UserRole,info.getEventTypeID());
		delete etUtil;

		itemStaus->setText(info.getEventStatus());

		if(!info.getEventNo().contains(str_)) continue;

		ui.tblEventDetails->insertRow(k);

		ui.tblEventDetails->setItem(k,0,itemEventNo);
		ui.tblEventDetails->setItem(k,1,itemEventTitle);
		ui.tblEventDetails->setItem(k,2,itemEventDescription);
		ui.tblEventDetails->setItem(k,3,itemCreatedDate);
		ui.tblEventDetails->setItem(k,4,itemFinishedDate);
		ui.tblEventDetails->setItem(k,5,itemEventType);
		ui.tblEventDetails->setItem(k,6,itemStaus);

		k++;
	}
}
void WriteToDisk::cmbLocationName_indexChanged(int index_)
{
	RoomUtil *util =new RoomUtil(this);

	try
	{
		ui.cmbRoomName->blockSignals(true);
		ui.cmbRoomName->clear();
		ui.cmbRoomName->blockSignals(false);

		int locationID=ui.cmbLocationName->itemData(index_,Qt::UserRole).toInt();
		//getting all rooms by location
		QList<RoomInfo> locationRooms=util->selectRoomsOfLocation(locationID,logSession->getDriverName());

		QList<int> userRoomList=logSession->getUserRoomIDs();

		for (int i=0;i<userRoomList.size();i++)
		{
			//inserting location details as widget items to tblLocations
			foreach(RoomInfo info,locationRooms)
			{
				if (info.getRoomID()==userRoomList[i])
				{
					ui.cmbRoomName->insertItem(i,info.getRoomName(),info.getRoomID());
					break;
				}
			}
		}
		ui.listSessions->clear();
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
void WriteToDisk::cmbRoomName_indexChanged(int index_)
{
	cmbSearchCategory_indexChanged(ui.cmbSearchCategory->currentIndex());
	ui.listSessions->clear();
}
void WriteToDisk::cmbSearchCategory_indexChanged(int index_)
{
	if (index_==0)
	{
		loadAllEvents();
		txtSearch_textChanged(ui.txtSearch->text());
	}
	else
	{
		loadRescentEvents();
		txtSearch_textChanged(ui.txtSearch->text());
	}	
}
void WriteToDisk::txtSearch_textChanged(QString str_)
{
	//for removing rows from table widget
	for (int i=ui.tblEventDetails->rowCount(); i >= 0; --i)
	{
		ui.tblEventDetails->removeRow(i);	
	}

	listEvents(str_);
}
QString WriteToDisk::getSessionString(QDateTime startDate_,QDateTime endDate_)
{
	QString lstString;
	QString startDate=startDate_.date().toString("dd-MM-yyyy");
	QString time1=startDate_.time().toString("hh-mm-ss AP");
	QString time2=endDate_.time().toString("hh-mm-ss AP");
	lstString=startDate+" "+"("+time1+" to "+time2+")"+"";
	return lstString;
}
void WriteToDisk::eventDetails_cellClicked(int row_,int column_)
{
	
	SessionUtil *util=new SessionUtil(this);

	try
	{
		int eventID = ui.tblEventDetails->item(row_,0)->data(Qt::UserRole).toInt();

		QList<SessionInfo> allSessions=util->selectSessionsByEventID(eventID,logSession->getDriverName());

		ui.listSessions->clear();

		int k=0;

		foreach(SessionInfo info,allSessions)
		{
			QString sessionName=getSessionString(info.getStartedTime(),info.getEndedTime());

			QListWidgetItem *item=new QListWidgetItem();
			item->setText(sessionName);
			item->setData(Qt::UserRole,info.getSessionID());
			item->setIcon(icoSession);

			ui.listSessions->insertItem(k,item);

			k++;
		}
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

	ui.btnAdd->setEnabled(false);
	ui.btnRemove->setEnabled(false);
}
void WriteToDisk::tblEventDetails_cellChanged()
{
	if(this->layoutDirection()==Qt::RightToLeft)
	{
		ui.tblEventDetails->resize(ui.tblEventDetails->width()+1,ui.tblEventDetails->height());
		ui.tblEventDetails->resize(ui.tblEventDetails->width()-1,ui.tblEventDetails->height());
	}

	ui.btnAdd->setEnabled(false);
	ui.btnRemove->setEnabled(false);
}
void WriteToDisk::btnAddClicked()
{
	if(ui.listSessions->currentItem()==NULL)
	{
		return;
	}

	QListWidgetItem *item=new QListWidgetItem();
	item->setText(ui.listSessions->currentItem()->text());
	item->setData(Qt::UserRole,ui.listSessions->currentItem()->data(Qt::UserRole));
	item->setIcon(icoSession);
	
	bool existence=false;
	
	for(int i=0;i<ui.listSessionsToCopy->count();i++)
	{
		if(item->data(Qt::UserRole).toInt()==ui.listSessionsToCopy->item(i)->data(Qt::UserRole).toInt())
		{
			existence=true;
			break;
		}
	}

	if(existence!=true) 
	{		
		int sessionID=item->data(Qt::UserRole).toInt();		
		
		FileUtil *util=new FileUtil(this);
		QList<FileInfo> allFiles=util->selectFilesBySession(sessionID,logSession->getDriverName());
		
		int max_size=0;
		int k=0;

		foreach(FileInfo info,allFiles)
		{
			if(srvr->createConnection(backupServer,serverAdminName,serverPassword))
			{
				QFile *fp=new QFile(info.getFilePath());
				if(fp->exists())
				{
					qint64 len = fp->bytesAvailable()/1024;
					max_size=max_size+len;					
				}
				else
				{
					k++;
				}
			}		
		}
		delete util;
		
		if(ui.availableSpace->maximum()<(totalAdded+max_size))
		{
			SHOW_INFOMESSAGE(tr("ERROR!!!"),tr("Your maximum available space is completely filled."),tr("You can't add any more sessions to the write list."),tr("Ok"));
		}
		else
		{			
			ui.listSessionsToCopy->addItem(item);
			totalAdded=totalAdded+max_size;
			ui.availableSpace->setValue(totalAdded);
		}

		if(k!=0)
		{
			if(k==1)
			{
				SHOW_INFOMESSAGE(tr("ERROR!!!"),tr("One file is missing at Server, Check with Administrator."),tr("File may be not uploaded to server."),tr("Ok"));
			}
			else
			{
				SHOW_INFOMESSAGE(tr("ERROR!!!"),QString::number(k)+tr(" files are missing at Server, Check with Administrator."),tr("Files may be not uploaded to server."),tr("Ok"));
			}
		}
	}

	ui.btnAdd->setEnabled(false);
}
void WriteToDisk::btnRemoveClicked()
{

	if(ui.listSessionsToCopy->currentItem()==NULL)
	{
		return;
	}

	int row=ui.listSessionsToCopy->currentRow();	
	int sessionID=ui.listSessionsToCopy->currentItem()->data(Qt::UserRole).toInt();	
	int max_size=0;
	
	FileUtil *util=new FileUtil(this);
	QList<FileInfo> allFiles=util->selectFilesBySession(sessionID,logSession->getDriverName());
	
	foreach(FileInfo info,allFiles)
	{
		if(srvr->createConnection(backupServer,serverAdminName,serverPassword))
		{
			QFile *fp=new QFile(info.getFilePath());
			if(fp->exists())
			{
				qint64 len = fp->bytesAvailable()/1024;
				max_size=max_size+len;								
			}		
		}		
	}
	delete util;

	totalAdded=totalAdded-max_size;
	ui.availableSpace->setValue(totalAdded);

	delete ui.listSessionsToCopy->takeItem(row);
	ui.btnRemove->setEnabled(false);
}
void WriteToDisk::listSessionItemClicked()
{
	ui.btnAdd->setEnabled(true);
}
void WriteToDisk::copySessionListItemClicked()
{
	ui.btnRemove->setEnabled(true);
}
void WriteToDisk::cbxOpticalChanged(int status_)
{
	if(status_==2)
	{
		ui.cbxRemovableStatus->blockSignals(true);
		ui.cbxRemovableStatus->setChecked(false);
		ui.cbxRemovableStatus->blockSignals(false);

		ui.frameBrowsDisk->setHidden(true);
		ui.btnWrite->setEnabled(true);

		int maxVal=ui.availableSpace->value();
		QString drive=QDir::homePath().left(3);
		double bytesAvailable=driveAnalizer->getFreeSpace(drive);
		qint64 avail=(qint64)(bytesAvailable)/1024;
		avalableSpace=avail;
		if(maxVal>avalableSpace)
		{
			QString msg=tr("The temporary space available in %1 is limited.").arg(drive);
			SHOW_INFOMESSAGE(tr("ERROR!!!"),msg,tr("Click ok to clear the copy list..."),tr("Ok"));
			ui.listSessionsToCopy->clear();
			totalAdded=0;
			maxVal=0;
		}
		ui.availableSpace->setRange(0,avalableSpace);
		ui.availableSpace->setValue(maxVal);

		ui.btnWrite->setText(tr("Burn"));
	}
	else
	{
		ui.cbxRemovableStatus->setChecked(true);		
	}	
}
void WriteToDisk::cbxRemovableChanged(int status_)
{
	if(status_==2)
	{
		ui.cbxOpticalStatus->blockSignals(true);
		ui.cbxOpticalStatus->setChecked(false);
		ui.cbxOpticalStatus->blockSignals(false);

		ui.frameBrowsDisk->setHidden(false);
		if(NOT ui.txtBrowsedFolder->text().isEmpty()) 
		{
			ui.btnWrite->setEnabled(true);

			int maxVal=ui.availableSpace->value();
			QString drive=ui.txtBrowsedFolder->text().left(3);
			double bytesAvailable=driveAnalizer->getFreeSpace(drive);
			qint64 avail=(qint64)(bytesAvailable)/1024;
			avalableSpace=avail;
			if(maxVal>avalableSpace)
			{
				QString msg=tr("The space available in %1 is limited.").arg(drive);;
				SHOW_INFOMESSAGE(tr("ERROR!!!"),msg,tr("Click ok to clear the copy list..."),tr("Ok"));
				ui.listSessionsToCopy->clear();
				totalAdded=0;
				maxVal=0;
			}
			ui.availableSpace->setRange(0,avalableSpace);
			ui.availableSpace->setValue(maxVal);
		}
		else
		{
			ui.btnWrite->setEnabled(false);

			int maxVal=ui.availableSpace->value();
			QString drive=QDir::homePath().left(3);
			double bytesAvailable=driveAnalizer->getFreeSpace(drive);
			qint64 avail=(qint64)(bytesAvailable)/1024;
			avalableSpace=avail;
			if(maxVal>avalableSpace)
			{
				QString msg=tr("The temporary space available in %1 is limited.").arg(drive);;
				SHOW_INFOMESSAGE(tr("ERROR!!!"),msg,tr("Click ok to clear the copy list..."),tr("Ok"));
				ui.listSessionsToCopy->clear();
				totalAdded=0;
				maxVal=0;
			}
			ui.availableSpace->setRange(0,avalableSpace);
			ui.availableSpace->setValue(maxVal);
		}
		
		ui.btnWrite->setText(tr("Write"));
	}
	else
	{
		ui.cbxOpticalStatus->setChecked(true);
	}	
}
void WriteToDisk::btnBrowsClicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"",QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
	if(NOT dir.isEmpty())
	{
		ui.txtBrowsedFolder->setText(dir);
		ui.btnWrite->setEnabled(true);

		int maxVal=ui.availableSpace->value();
		QString drive=dir.left(3);
		double bytesAvailable=driveAnalizer->getFreeSpace(drive);
		qint64 avail=(qint64)(bytesAvailable)/1024;
		avalableSpace=avail;
		if(maxVal>avalableSpace)
		{
			QString msg=tr("The space available in %1 is limited.").arg(drive);;
			SHOW_INFOMESSAGE(tr("ERROR!!!"),msg,tr("Click ok to clear the copy list..."),tr("Ok"));
			ui.listSessionsToCopy->clear();
			totalAdded=0;
			maxVal=0;
		}
		ui.availableSpace->setRange(0,avalableSpace);
		ui.availableSpace->setValue(maxVal);
	}	
}
void WriteToDisk::btnWriteClicked()
{
	if(ui.listSessionsToCopy->count()==0)
	{
		SHOW_MESSAGE(tr("Empty List"),tr("There is no sessions selected to save on disk."),tr("Ok"));
		return;
	}

	if(ui.btnWrite->text()=="Finish")
	{
		this->close();
		return;
	}

	isRunning=true;
	ui.cbxOpticalStatus->setEnabled(false);
	ui.cbxPlayerStatus->setEnabled(false);
	ui.cbxRemovableStatus->setEnabled(false);
	ui.btnWrite->setEnabled(false);
	ui.btnBrows->setEnabled(false);
	ui.frame->setEnabled(false);
	
	if(ui.cbxOpticalStatus->isChecked())
	{
		prepareBurning();
	}
	else
	{
		writeRemovableDisk();
	}
	isRunning=false;	
	ui.cbxOpticalStatus->setEnabled(true);
	ui.cbxPlayerStatus->setEnabled(true);
	ui.cbxRemovableStatus->setEnabled(true);
	ui.btnWrite->setEnabled(true);
	ui.btnBrows->setEnabled(true);
	ui.frame->setEnabled(true);
	ui.btnWrite->setText(tr("Finish"));
}
void WriteToDisk::prepareBurning()
{
	int max_size=0;
	int k=0;
	QStringList fileList;
	for(int i=0;i<ui.listSessionsToCopy->count();i++)
	{
		int sessionID=ui.listSessionsToCopy->item(i)->data(Qt::UserRole).toInt();
		FileUtil *util=new FileUtil(this);
		QList<FileInfo> allFiles=util->selectFilesBySession(sessionID,logSession->getDriverName());
		foreach(FileInfo info,allFiles)
		{
			if(srvr->createConnection(backupServer,serverAdminName,serverPassword))
			{
				QFile *fp=new QFile(info.getFilePath());
				if(fp->exists())
				{
					qint64 len = fp->bytesAvailable()/1024;
					max_size=max_size+len;
					fileList<<info.getFilePath();
				}
				else
				{
					k++;
				}
			}
			else
			{
				SHOW_MESSAGE(tr("ERROR!!!"),tr("Server connection failed."),tr("Ok"))
			}
		}
		delete util;
	}

	if(k!=0)
	{
		if(k==1)
		{
			SHOW_INFOMESSAGE(tr("ERROR!!!"),tr("One file is missing at Server, Check with Administrator."),tr("File may be not uploaded to server."),tr("Ok"));
		}
		else
		{
			SHOW_INFOMESSAGE(tr("ERROR!!!"),QString::number(k)+tr(" files are missing at Server, Check with Administrator."),tr("Files may be not uploaded to server."),tr("Ok"));
		}
		return;
	}

	ui.totalProgress->setRange(0,max_size);
	ui.totalProgress->setValue(0);

	ui.lblStatus->setText(tr("Reading source files to burn. Please wait..."));
	ui.lblStatus->setHidden(false);
	ui.frameTotalProgress->setHidden(false);

	if(ui.cbxPlayerStatus->isChecked())
	{
		copyPlayer(QDir::homePath()+"/Application Data/Evidencer_Temp");
	}
	if(copyFiles(fileList,QDir::homePath()+"/Application Data/Evidencer_Temp"))
	{
		QString tempString=QDir::homePath()+"/Application Data/Evidencer_Temp";
		tempString=tempString.replace("/","\\");
		
 		QStringList tempList;
 		tempList<<"burn"<<"/d"<<tempString;
 		tempList<<"/l"<<"CRS_BCK_"+sharedObj->getCurrentDateTime().toString("dd-MM-yyyy");
 		tempList<<"/eject";
		
		burn.start("Burn.exe",tempList);
		burn.waitForStarted();
		if(burn.error()==QProcess::FailedToStart)
		{
			SHOW_INFOMESSAGE(tr("ERROR!!!"),tr("Some files are missing in ")+QDir::currentPath(),tr("Please re-install the application to fix the problem."),tr("Ok"));
			ui.lblStatus->setText(tr("Burning Failed and process terminated abnormally."));
			qApp->processEvents();
			return;
		}
		else
		{
			ui.lblStatus->setText(tr("Burning started. Please wait..."));
		}

		burn.waitForFinished(-1);
		
		ui.lblStatus->setText(tr("Burning Process Finished successfully."));
	}
	else
	{
		ui.lblStatus->setText(tr("Reading source files Failed and terminated."));
	}
}

void WriteToDisk::writeRemovableDisk()
{
	int max_size=0;
	int k=0;
	QStringList fileList;
	for(int i=0;i<ui.listSessionsToCopy->count();i++)
	{
		int sessionID=ui.listSessionsToCopy->item(i)->data(Qt::UserRole).toInt();
		FileUtil *util=new FileUtil(this);
		QList<FileInfo> allFiles=util->selectFilesBySession(sessionID,logSession->getDriverName());
		foreach(FileInfo info,allFiles)
		{
			if(srvr->createConnection(backupServer,serverAdminName,serverPassword))
			{
				QFile *fp=new QFile(info.getFilePath());
				if(fp->exists())
				{
					qint64 len = fp->bytesAvailable()/1024;
					max_size=max_size+len;
					fileList<<info.getFilePath();
				}
				else
				{
					k++;
				}
			}
			else
			{
				SHOW_MESSAGE(tr("ERROR!!!"),tr("Server connection failed."),tr("Ok"))
			}
		}
		delete util;
	}

	if(k!=0)
	{
		if(k==1)
		{
			SHOW_INFOMESSAGE(tr("ERROR!!!"),tr("One file is missing at Server, Check with Administrator."),tr("File may be not uploaded to server."),tr("Ok"));
		}
		else
		{
			SHOW_INFOMESSAGE(tr("ERROR!!!"),QString::number(k)+tr(" files are missing at Server, Check with Administrator."),tr("Files may be not uploaded to server."),tr("Ok"));
		}
		return;
	}

	ui.totalProgress->setRange(0,max_size);
	ui.totalProgress->setValue(0);

	ui.lblStatus->setText(tr("Writing Process Started. Please wait..."));
	ui.lblStatus->setHidden(false);
	ui.frameTotalProgress->setHidden(false);

	if(ui.cbxPlayerStatus->isChecked())
	{
		copyPlayer(ui.txtBrowsedFolder->text());
	}
	if(copyFiles(fileList,ui.txtBrowsedFolder->text()))
	{
		ui.lblStatus->setText(tr("Writing Process Finished successfully."));
	}
	else
	{
		ui.lblStatus->setText(tr("Writing Process Failed and terminated."));
	}
}
bool WriteToDisk::copyFiles(QStringList fileList_,QString path_)
{

	int totalProgress=0;

	for(int i=0;i<ui.listSessionsToCopy->count();i++)
	{
		QString sessionName=ui.listSessionsToCopy->item(i)->text();
		int sessionID=ui.listSessionsToCopy->item(i)->data(Qt::UserRole).toInt();	

		QString sessionTemp=sessionName;
		sessionTemp=sessionTemp.replace("\\","_");
		sessionTemp=sessionTemp.replace("/","_");
		sessionTemp=sessionTemp.replace(":","_");
		sessionTemp=sessionTemp.replace("*","_");	
		sessionTemp=sessionTemp.replace("?","_");
		sessionTemp=sessionTemp.replace('"',"_");
		sessionTemp=sessionTemp.replace("<","_");
		sessionTemp=sessionTemp.replace(">","_");
		sessionTemp=sessionTemp.replace("|","_");

		FileUtil *util=new FileUtil(this);
		QList<FileInfo> allFiles=util->selectFilesBySession(sessionID,logSession->getDriverName());
		int k=0;
		foreach(FileInfo info,allFiles)
		{
			k++;
			if(srvr->createConnection(backupServer,serverAdminName,serverPassword))
			{
				copyThread=new CopyEngine(this);
				copyThread->setSrcFile(info.getFilePath());		
				
				QString dest=path_+"/"+sessionTemp+"_File_"+QString::number(k)+".avl";	
				dest=dest.replace("/","\\");

				copyThread->setDstFile(dest);
				copyThread->start(QThread::LowPriority);

				int status;
				totalProgress=ui.totalProgress->value();

				while(!copyThread->isFinished())
				{
					if(i==0 && k==1) ui.totalProgress->setValue(copyThread->getProgress());
					else ui.totalProgress->setValue(totalProgress+copyThread->getProgress());
					qApp->processEvents();
				}
					
				status=copyThread->getStatus();

				if(status==0)
				{
					SHOW_INFOMESSAGE(tr("ERROR!!!"),tr("Writing failed, Process terminated."),tr("May be you don't have privilege to write in specified path or session is already saved."),tr("Ok"));
					return false;
				}

				SAFE_DELETE(copyThread);
			}
			else
			{
				SHOW_MESSAGE(tr("ERROR!!!"),tr("Server connection failed."),tr("Ok"))
					return false;
			}	
			qApp->processEvents();
		}
		delete util;
		
		QList<QStringList> allNotes=readSessionNotes(sessionID);
		QString html=convertToHtml(allNotes);
		QString destinationPath=path_+"/"+sessionTemp+".pdf";
		createPdfFile(destinationPath,html);

		//create session notes
		qApp->processEvents();
	}

	ui.lblStatus->setText(tr("Reading source files Finished successfully."));

	return true;
}
QList<QStringList> WriteToDisk::readSessionNotes(int sessionID_)
{
	QList<QStringList> allNotes;

	EventNoteUtil *util=new EventNoteUtil(this);
	try
	{
		QList<EventNoteInfo> sessionFiles=util->selectEventNotesBySession(sessionID_,logSession->getDriverName());
		foreach(EventNoteInfo info,sessionFiles)
		{
			QStringList tmpList;
			tmpList<<info.getEventTime().toString("dd-MM-yyyy hh:mm:ss AP");			
			tmpList<<info.getEventSpeaker();
			tmpList<<info.getEventNote();

			allNotes<<tmpList;
		}

		int eventID=0,eventTypeID=0,roomID=0,locationID=0;

		EventNoteInfo etnInfo=util->getEventDetailsBySession(sessionID_);

		eventID=etnInfo.getEventID();
		roomID=etnInfo.getRoomID();

		EventUtil *eUtil=new EventUtil(this);

		EventInfo eInfo=eUtil->loadEvent(eventID,logSession->getDriverName());
		eventNo=eInfo.getEventNo();
		eventTitle=eInfo.getEventName();
		eventDescription=eInfo.getEventDescription();
		createdDate=eInfo.getCreatedDate().toString("dd/MM/yyyy");
		eventTypeID=eInfo.getEventTypeID();
		delete eUtil;

		EventTypeUtil *etUtil=new EventTypeUtil(this);

		EventTypeInfo etInfo=etUtil->loadEventType(eventTypeID,logSession->getDriverName());
		eventType=etInfo.getEventTypeName();
		delete etUtil;

		RoomUtil *rUtil=new RoomUtil(this);

		RoomInfo rInfo=rUtil->loadRoom(roomID,logSession->getDriverName());
		room=rInfo.getRoomName();
		locationID=rInfo.getLocationID();
		delete rUtil;

		LocationUtil *lUtil=new LocationUtil(this);

		LocationInfo lInfo=lUtil->loadLocation(locationID,logSession->getDriverName());
		location=lInfo.getLocationName();
		delete lUtil;
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

	return allNotes;
}
void WriteToDisk::copyPlayer(QString destination_)
{
	bool has_err = false;
	QString destination=destination_+"/Player";
	QString playerPath=QDir::currentPath()+"/Portable Player";
	copyFolder(playerPath,destination);
	QFile::copy(playerPath+"/Autorun.inf",destination_+"/Autorun.inf");
}
void WriteToDisk::copyFolder(QString sourceFolder,QString destFolder)
{
	QQueue< QPair<QString, QString> > queue;

	queue.enqueue(qMakePair(sourceFolder, destFolder));

	while (!queue.isEmpty())
	{
		QPair<QString, QString> pair = queue.dequeue();
		QDir sourceDir(pair.first);
		QDir destDir(pair.second);

		if(!sourceDir.exists())
			continue;

		if(!destDir.exists())
			destDir.mkpath(pair.second);

		QStringList files = sourceDir.entryList(QDir::Files);
		for(int i = 0; i < files.count(); i++)
		{
			QString srcName = pair.first + "/" + files.at(i);
			QString destName = pair.second + "/" + files.at(i);
			QFile::copy(srcName, destName);
		}

		QStringList dirs = sourceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
		for(int i = 0; i < dirs.count(); i++)
		{
			QString srcName = pair.first + "/" + dirs.at(i);
			QString destName = pair.second + "/" + dirs.at(i);
			queue.enqueue(qMakePair(srcName, destName));
		}
	}
}
bool WriteToDisk::removeDirectory(QString path_)
{

 	bool has_err = false;
 	
 	QDir *aDir=new QDir(path_);
 	
 	if (aDir->exists())
 	{
 		QFileInfoList entries = aDir->entryInfoList(QDir::NoDotAndDotDot |
 		QDir::Dirs | QDir::Files);
 		int count = entries.size();
 		for (int idx = 0; ((idx < count) && (NOT has_err)); idx++)
 		{
 			QFileInfo entryInfo = entries[idx];
 			QString path = entryInfo.absoluteFilePath();
 			if (entryInfo.isDir())
 			{
 				has_err = removeDirectory(path);
 			}
 			else
 			{
 				QFile file(path);
 				if (!file.remove())
 					has_err = true;
 			}
 		}
 		if (!aDir->rmdir(aDir->absolutePath()))
 			has_err = true;
 	}
 	return has_err;
}
void WriteToDisk::readSrvrSettings()
{
	AppSettingsUtil *util=new AppSettingsUtil(this);
	HashManager *generate=new HashManager(this);
	PCUtil *pUtil=new PCUtil(this);

	try
	{
		SystemFetch *fetch=new SystemFetch();

		PCInfo pcInfo=pUtil->loadPCDetails(fetch->getSystemID(),logSession->getDriverName());

		AppSettingsInfo info=util->loadAppSettings(pcInfo.getPCID(),logSession->getDriverName());

		backupServer=info.getSharedServerIP();
		serverAdminName=info.getServerUserName();
		serverPassword=generate->decrypts(info.getServerUserPassword());

		delete fetch;
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

	delete pUtil;
	delete generate;
	delete util;
}
void WriteToDisk::showAvailability(int value_)
{
	int used=ui.availableSpace->value();

	if(used<=1024)
	{
		ui.lblAvailable->setText(QString::number(used,'f',2)+" KB");
	}
	else if(used>1024 && used<=1048576)
	{
		ui.lblAvailable->setText(QString::number(used/1024,'f',2)+" MB");
	}
	else
	{
		ui.lblAvailable->setText(QString::number(used/(1024*1024),'f',2)+" GB");
	}
}
QString WriteToDisk::convertToHtml(QList<QStringList> selectedRows_)
{
	QString htmlString="<html>"
		"<body>"
		"<br>"
		"<table width=\"600\" border=\"0\" bordercolor=\"#EFEFFB\" cellspacing=\"0\" cellpadding=\"0\">"
		"<tr><td colspan=\"10\">&nbsp;</td></tr>"
		"<tr>"
		"<td colspan=\"10\" height=\"213\" valign=\"bottom\" align=\"center\">&nbsp;<img src=\":/Recorder/Resources/AdjdLogo.png\" height=\"140\"></td>"
		"</tr>"
		"<tr><td colspan=\"10\" height=\"100\">"
		"<table width=\"600\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">"
		"<tr><td colspan=\"3\">&nbsp;</td></tr>"
		"<tr>"
		"<th align=\"left\"><font face=\"arial\" size=\"2\" color=\"#003664\">&nbsp;Event No</font></th>"			
		"<th align=\"left\"><font face=\"arial\" size=\"2\" color=\"#003664\">&nbsp;Event Title</font></th>"			
		"<th colspan=\"2\" align=\"left\"><font face=\"arial\" size=\"2\" color=\"#003664\">&nbsp;Description</font></th>"
		"</tr>"	
		"<tr>"
		"<td>&nbsp;<table width=\"147\" border=\"1\" bordercolor=\"#f1f1f1\" cellspacing=\"0\" cellpadding=\"0\"><tr><td><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;"+eventNo+"</font></td></tr></table></td>"	
		"<td>&nbsp;<table width=\"147\" border=\"1\" bordercolor=\"#f1f1f1\" cellspacing=\"0\" cellpadding=\"0\"><tr><td><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;"+eventTitle+"</font></td></tr></table></td>"
		"<td colspan=\"2\">&nbsp;<table width=\"300\" border=\"1\" bordercolor=\"#f1f1f1\" cellspacing=\"0\" cellpadding=\"0\"><tr><td><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;"+eventDescription+"</font></td></tr></table></td>"	
		"</tr>"
		"<tr><td colspan=\"3\">&nbsp;</td></tr>"
		"<tr>"
		"<th align=\"left\"><font face=\"arial\" size=\"2\" color=\"#003664\">&nbsp;Location</font></th>"		
		"<th align=\"left\"><font face=\"arial\" size=\"2\" color=\"#003664\">&nbsp;Room</font></th>"
		"<th align=\"left\"><font face=\"arial\" size=\"2\" color=\"#003664\">&nbsp;Event Type</font></th>"	
		"<th align=\"left\"><font face=\"arial\" size=\"2\" color=\"#003664\">&nbsp;Created Date</font></th>"				
		"</tr>"	
		"<tr>"
		"<td>&nbsp;<table width=\"147\" border=\"1\" bordercolor=\"#f1f1f1\" cellspacing=\"0\" cellpadding=\"0\"><tr><td><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;"+location+"</font></td></tr></table></td>"
		"<td>&nbsp;<table width=\"147\" border=\"1\" bordercolor=\"#f1f1f1\" cellspacing=\"0\" cellpadding=\"0\"><tr><td><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;"+room+"</font></td></tr></table></td>"	
		"<td>&nbsp;<table width=\"147\" border=\"1\" bordercolor=\"#f1f1f1\" cellspacing=\"0\" cellpadding=\"0\"><tr><td><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;"+eventType+"</font></td></tr></table></td>"
		"<td>&nbsp;<table width=\"150\" border=\"1\" bordercolor=\"#f1f1f1\" cellspacing=\"0\" cellpadding=\"0\"><tr><td><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;"+createdDate+"</font></td></tr></table></td>"				
		"</tr>"
		"<tr><td colspan=\"3\">&nbsp;</td></tr>"
		"</table>"
		"</td></tr>"
		"<tr><td colspan=\"10\">"
		"<table width=\"600\" border=\"0\" bordercolor=\"#EFEFFB\" cellspacing=\"0\" cellpadding=\"0\">"
		"<tr>"
		"<th bgcolor=\"#003664\" width=\"150\" height=\"30\" align=\"left\"><font face=\"arial\" size=\"3\" color=\"#FFFFFF\">&nbsp;Date & Time</font></th>"
		"<th bgcolor=\"#003664\" width=\"150\" height=\"30\" align=\"left\"><font face=\"arial\" size=\"3\" color=\"#FFFFFF\">&nbsp;Speaker</font></th>"
		"<th bgcolor=\"#003664\" width=\"300\" height=\"30\" align=\"left\"><font face=\"arial\" size=\"3\" color=\"#FFFFFF\">&nbsp;Notes</font></th>"
		"</tr>";

	int k=0;

	foreach(QStringList itemList,selectedRows_)
	{
		QString rowColor;

		if(k==0){rowColor="#FFFFFF"; k=1;}	
		else{rowColor="#D8D8D8"; k=0;}

		htmlString.append("<tr>");

		htmlString.append("<td bgcolor="+rowColor+" width=\"150\" height=\"25\"><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;");
		htmlString.append(itemList[0]);
		htmlString.append("</font></td>");	

		htmlString.append("<td bgcolor="+rowColor+" width=\"150\" height=\"25\"><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;");
		htmlString.append(itemList[1]);
		htmlString.append("</font></td>");	

		htmlString.append("<td bgcolor="+rowColor+" width=\"300\" height=\"25\"><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;");
		htmlString.append(itemList[2]);
		htmlString.append("</font></td>");	

		htmlString.append("</tr>");
	}

	htmlString=htmlString+"</table>"
		"</td></tr>"
		"<tr>"
		"<td colspan=\"10\">&nbsp;</td>"
		"</tr>"
		"</table>"
		"</body>"
		"</html>";

	return htmlString;
}
void WriteToDisk::createPdfFile(QString filePath_,QString html_)
{
	QTextDocument *printtextEdit= new QTextDocument();
	QPrinter printer(QPrinter::HighResolution);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOutputFileName(filePath_);
	printtextEdit->setHtml(html_);
	printtextEdit->print(&printer);
}
void WriteToDisk::closeEvent(QCloseEvent * event)
{
	if (isRunning)
	{
		QMessageBox msgBox;
		msgBox.setText(tr("Copying on progress Are you sure you want to stop writer ?"));
		msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Cancel);
		int ret = msgBox.exec();

		switch (ret)
		{
		case QMessageBox::Ok:
			if(copyThread) 
			{
				copyThread->setExitStatus(true);

				while(copyThread->isFinished())
				{
					close();
				}				
			}			
			
			break;
		case QMessageBox::Cancel:
			if(copyThread==NULL)
			{
				ui.totalProgress->setValue(ui.totalProgress->maximum());
			}
			event->ignore();
			break;
		}
	}
	else
	{
		close();
	}
}
WriteToDisk::~WriteToDisk()
{
	delete sharedObj;
	delete driveAnalizer;
}
