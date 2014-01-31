#include "wizard.h"
#include "audiodevices.h"
#include "videodevices.h"
#include "appsettingsutil.h"
#include "pcutil.h"
#include "hashmanager.h"
#include "systemfetch.h"
#include "locationutil.h"
#include "roomutil.h"
#include "videodeviceutil.h"
#include "audiodeviceutil.h"
#include "etrprofilegenerator.h"
#include <QSettings>
#include <QRegExp>
#include <QValidator>
#include <QDir>

Wizard::Wizard(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	refershWizard();

	connect(ui.btnRegister,		SIGNAL(clicked()),				 this,SLOT(btnRegisterPC_click()));
	connect(ui.btnConnect,		SIGNAL(clicked()),				 this,SLOT(btnConnect_click()));
	connect(ui.btnVideoPlus,	SIGNAL(clicked()),				 this,SLOT(btnVideoPlus_click()));
	connect(ui.btnVideoMinus,	SIGNAL(clicked()),				 this,SLOT(btnVideoMinus_click()));
	connect(ui.btnAudioPlus,	SIGNAL(clicked()),				 this,SLOT(btnAudioPlus_click()));
	connect(ui.btnAudioMinus,	SIGNAL(clicked()),				 this,SLOT(btnAudioMinus_click()));
	connect(ui.btnOk,			SIGNAL(clicked()),				 this,SLOT(btnOk_click()));
	connect(ui.cmbLocation,		SIGNAL(currentIndexChanged(int)),this,SLOT(cmbLocationName_indexChange(int)));
	connect(ui.cmbRoom,			SIGNAL(currentIndexChanged(int)),this,SLOT(cmbRoomName_indexChange(int)));
	connect(ui.videoTableWidget,SIGNAL(cellClicked(int,int)),	 this,SLOT(videotable_indexChange(int,int)));
	connect(ui.audioTableWidget,SIGNAL(cellClicked(int,int)),	 this,SLOT(audiotable_indexChange(int,int)));
	connect(ui.tabWidget,		SIGNAL(currentChanged(int)),	 this,SLOT(wizardTabChanged(int)));
	connect(ui.btnRestore,		SIGNAL(clicked()),				 this,SLOT(btnRestore_click()));
	connect(ui.txtPCDescription,SIGNAL(textChanged(QString)),	 this,SLOT(text_PCChanged()));

	QRegExp numberField("^[0-9]+$");
	QValidator *numberValidator = new QRegExpValidator(numberField, this);

	ui.txtPortNo->setValidator(numberValidator);
	ui.txtBitRate->setValidator(numberValidator);
	ui.txtQuality->setValidator(numberValidator);
	ui.txtKeyFrame->setValidator(numberValidator);	

	ui.btnVideoMinus->setEnabled(false);
	ui.btnAudioMinus->setEnabled(false);
}
void Wizard::text_PCChanged()
{
	ui.tabWidget->setTabEnabled (4,false);
}
void Wizard::btnRestore_click()
{
	ui.txtBitRate->setText("1500000");
	ui.txtQuality->setText("90");
	ui.txtKeyFrame->setText("30000000");	
}
void Wizard::audiotable_indexChange(int row_,int column_)
{
	audioColumn=row_;
	ui.btnAudioMinus->setEnabled(true);
}

void Wizard::videotable_indexChange(int row_,int column_)
{
	videoColumn=row_;

	ui.btnVideoMinus->setEnabled(true);
}

void Wizard::setDriverName(QString driver_)
{
	driverName=driver_;
}

void Wizard::refershWizard()
{
	ui.tabWidget->setCurrentIndex(0);
	ui.tabWidget->setTabEnabled (1,false);
	ui.tabWidget->setTabEnabled (2,false);
	ui.tabWidget->setTabEnabled (3,false);
	ui.tabWidget->setTabEnabled (4,false);
	ui.btnOk->setEnabled(false);

	QSettings settings("WConfig.ini",QSettings::IniFormat);
	HashManager *generate = new HashManager(this);

	ui.txtDataBaseServer->setText(settings.value("SERVER").toString());
	ui.txtDBName->setText(settings.value("DATABASE").toString());
	ui.txtUserName->setText(settings.value("UID").toString());
	ui.txtPassword->setText(generate->decrypts(settings.value("PWD").toString()));
	ui.txtBitRate->setText(settings.value("Bitrate").toString());
	ui.txtQuality->setText(settings.value("Quality").toString());
	ui.txtKeyFrame->setText(settings.value("FrameSpacing").toString());

	delete generate;
}

void Wizard::resizeEvent ( QResizeEvent * )
{
	int videoWidgetHeaderWidth=ui.videoTableWidget->width(); 
	ui.videoTableWidget->setColumnWidth(0,videoWidgetHeaderWidth/2);
	ui.videoTableWidget->setColumnWidth(1,videoWidgetHeaderWidth/2);

	int audioWidgetHeaderWidth=ui.audioTableWidget->width(); 
	ui.audioTableWidget->setColumnWidth(0,audioWidgetHeaderWidth/2);
	ui.audioTableWidget->setColumnWidth(1,audioWidgetHeaderWidth/2);
}

void Wizard::btnVideoPlus_click()
{	
	VideoDevices *vidDev=new VideoDevices(this);
	vidDev->exec();
	delete vidDev;
}

void Wizard::btnVideoMinus_click()
{
	VideoDeviceUtil *vutil=new VideoDeviceUtil(this);

	try
	{
		if (ui.videoTableWidget->rowCount()==0)
		{
			return;
		}

		int videoDeviceID=ui.videoTableWidget->item(ui.videoTableWidget->currentRow(),0)->data(Qt::UserRole).toInt();
		vutil->deleteVideoDevice(videoDeviceID,driverName);

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

	ui.videoTableWidget->removeRow(videoColumn);
	ui.btnVideoMinus->setEnabled(false);
}

void Wizard::btnAudioPlus_click()
{
	AudioDevices *audDev=new AudioDevices(this);
	audDev->exec();
	delete audDev;
}

void Wizard::btnAudioMinus_click()
{
	AudioDeviceUtil *autil=new AudioDeviceUtil(this);
	
	try
	{
		if (ui.audioTableWidget->rowCount()==0)
		{
			return;
		}

		int audioDeviceID=ui.audioTableWidget->item(ui.audioTableWidget->currentRow(),0)->data(Qt::UserRole).toInt();
		autil->deleteAudioDevice(audioDeviceID,driverName);
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


	ui.audioTableWidget->removeRow(audioColumn);
	ui.btnAudioMinus->setEnabled(false);
}
void Wizard::checkDBConnection()
{
	db=QSqlDatabase::addDatabase("QODBC");

	QString dsn=QString("DRIVER={SQL Native Client};SERVER=%1;DATABASE=%2;UID=%3;PWD=%4;").arg(ui.txtDataBaseServer->text().trimmed()).arg(ui.txtDBName->text().trimmed()).arg(ui.txtUserName->text().trimmed()).arg(ui.txtPassword->text().trimmed());

	db.setDatabaseName(dsn);

	if(!db.open())
	{

		PRINT_ERROR(db.lastError().text());
		ui.lblstatus->setText("Connection Failed!!!");
		ui.tabWidget->setTabEnabled (1,false);
		ui.tabWidget->setTabEnabled (2,false);
		ui.tabWidget->setTabEnabled (3,false);
		ui.tabWidget->setTabEnabled (4,false);
		return;
	}

	setDriverName(ui.cmbDriver->currentText());

	ui.lblstatus->setText("Connected successfully");

	ui.tabWidget->setTabEnabled (1,true);
	ui.tabWidget->setTabEnabled (2,true);
	ui.tabWidget->setTabEnabled (3,true);
	ui.tabWidget->setTabEnabled (4,true);

	//ui.btnConnect->setEnabled(false);
	//ui.btnOk->setEnabled(true);
	loadPCRegDetails();
	readSettings();
}
void Wizard::btnConnect_click()
{
	if(ui.txtDataBaseServer->text().trimmed()=="" || ui.txtDBName->text().trimmed()=="" || ui.txtUserName->text().trimmed()=="" || ui.txtPassword->text().trimmed()=="")
	{
		return;
	}

	db=QSqlDatabase::addDatabase("QODBC");

	QString dsn=QString("DRIVER={SQL Native Client};SERVER=%1;DATABASE=%2;UID=%3;PWD=%4;").arg(ui.txtDataBaseServer->text().trimmed()).arg(ui.txtDBName->text().trimmed()).arg(ui.txtUserName->text().trimmed()).arg(ui.txtPassword->text().trimmed());

	db.setDatabaseName(dsn);

	if(!db.open())
	{

		PRINT_ERROR(db.lastError().text());
		ui.lblstatus->setText("Connection Failed!!!");
		ui.tabWidget->setTabEnabled (1,false);
		ui.tabWidget->setTabEnabled (2,false);
		ui.tabWidget->setTabEnabled (3,false);
		ui.tabWidget->setTabEnabled (4,false);
		return;
	}

	setDriverName(ui.cmbDriver->currentText());

	ui.lblstatus->setText("Connected successfully");

	ui.tabWidget->setTabEnabled (1,true);
	ui.tabWidget->setTabEnabled (2,true);
	ui.tabWidget->setTabEnabled (3,true);
	ui.tabWidget->setTabEnabled (4,true);

	ui.btnConnect->setEnabled(false);
	//ui.btnOk->setEnabled(true);

	loadPCRegDetails();
	readSettings();
}

void Wizard::readSettings()
{
	readPCDetails();
	readAudioSettings();
	readVideoSettings();
	readAppSettings();
}

void Wizard::writeSettings()
{
	writePCDetails();
	writeAppSettings();
	writeVideoSettings();
	writeAudioSettings();
}

void Wizard::btnOk_click()
{

	HashManager *generate = new HashManager(this);
	QSettings settings("WConfig.ini",QSettings::IniFormat);

	settings.setValue("SERVER",ui.txtDataBaseServer->text().trimmed());
	settings.setValue("DATABASE",ui.txtDBName->text().trimmed());
	settings.setValue("UID",ui.txtUserName->text().trimmed());
	settings.setValue("PWD",generate->encrypts(ui.txtPassword->text().trimmed()));
	settings.setValue("DRIVER",ui.cmbDriver->currentText().trimmed());
	settings.setValue("DomainController",ui.cmbDomainName->currentText().trimmed());
	settings.setValue("Bitrate",ui.txtBitRate->text().trimmed());
	settings.setValue("Quality",ui.txtQuality->text().trimmed());
	settings.setValue("FrameSpacing",ui.txtKeyFrame->text().trimmed());
	delete generate;

	if (ui.audioTableWidget->rowCount()==0 && ui.videoTableWidget->rowCount()==0)
	{
		SHOW_MESSAGE(tr("ERROR MESSAGE"),tr("No Video Device and No Audio device Found!!!"),tr("Ok"));
		return;
	}
	writeSettings();

	ETRProfileGenerator *prx=new ETRProfileGenerator(this);
	prx->setBit_Rate(ui.txtBitRate->text().toInt());
	prx->setFrame_Spacing(ui.txtKeyFrame->text().toInt());
	prx->setQuality(ui.txtQuality->text().toInt());

	int channelCount;

	channelCount=0;

	for (int j=0;j<ui.audioTableWidget->rowCount();j++)
	{
		QTableWidgetItem *itemtext2=new QTableWidgetItem();
		itemtext2=ui.audioTableWidget->item(j,1);
		QString type=itemtext2->text();

		if (type=="Stereo")
		{
			channelCount=channelCount+2;
		}
// 		else
// 		{
// 			channelCount=channelCount+1;
// 		}
	}
	
	if (ui.audioTableWidget->rowCount()==0)
	{
		for (int i=0;i<ui.videoTableWidget->rowCount();i++)
		{
			QTableWidgetItem *itemtext2=new QTableWidgetItem();
			itemtext2=ui.videoTableWidget->item(i,1);
			QString Res=itemtext2->text();
			QStringList r=Res.split("x");
			QString res1=r.first();
			QString res2=r.last();
			QString vprx="video_"+QString::number(i)+".prx";
			prx->setResolution(res1.toInt(),res2.toInt());
			prx->generateVideo_Profile(vprx);
		}
	}

	if (ui.videoTableWidget->rowCount()==0)
	{
		prx->generatePofile(channelCount);
	}

	if (ui.audioTableWidget->rowCount()!=0 && ui.videoTableWidget->rowCount()!=0)
	{
		for (int i=0;i<ui.videoTableWidget->rowCount();i++)
		{
			if (i==0)
			{
				QTableWidgetItem *itemtext2=new QTableWidgetItem();
				itemtext2=ui.videoTableWidget->item(i,1);
				QString Res=itemtext2->text();
				QStringList r=Res.split("x");
				QString res1=r.first();
				QString res2=r.last();
				prx->setResolution(res1.toInt(),res2.toInt());
				QString vprx="video_"+QString::number(i)+".prx";
				int audiorow=ui.audioTableWidget->rowCount();

				if (audiorow==0)
				{
					prx->generateVideo_Profile(vprx);
				}
				else
				{
					if (channelCount%2==0)
					{
						prx->generatePofile(channelCount,vprx);
					}
				}		
			}
			else
			{
				QTableWidgetItem *itemtext2=new QTableWidgetItem();
				itemtext2=ui.videoTableWidget->item(i,1);
				QString Res=itemtext2->text();
				QStringList r=Res.split("x");
				QString res1=r.first();
				QString res2=r.last();
				QString vprx="video_"+QString::number(i)+".prx";
				prx->setResolution(res1.toInt(),res2.toInt());

				prx->generateVideo_Profile(vprx);
			}
		}
	}
	setChannelCount(channelCount);
	ui.btnOk->setEnabled(false);

// 	process->start("Finish.bat",QStringList()<<"");
// 	process->waitForStarted();
// 	if(process->error()==QProcess::FailedToStart)
// 	{
// 		SHOW_INFOMESSAGE(tr("ERROR!!!"),tr("Some files are missing in ")+QDir::currentPath(),tr("Please re-install the application to fix the problem."),tr("Ok"));
// 		qApp->processEvents();
// 		return;
// 	}
// 	process->waitForFinished(-1); 
}

void Wizard::setChannelCount(int count_)
{
	channelsCount=count_;
}
int Wizard::getChannelCount()
{
	return channelsCount;
}
void Wizard::loadPCRegDetails()
{
	SystemFetch *fetch=new SystemFetch();

	ui.txtPCName->setText(fetch->getSystemName());
	ui.txtPCName->setReadOnly(true);

	delete fetch;

	LocationUtil *util =new LocationUtil(this);

	try
	{
		//getting all location details
		QList<LocationInfo> allLocations=util->selectAllLocations(driverName);

		int k=0;

		/*************clearing all location combo box*************************/
		//block signals is to prevent unusual index change signal emits

		ui.cmbLocation->blockSignals(true);
		ui.cmbLocation->clear();
		ui.cmbLocation->blockSignals(false);

		foreach(LocationInfo info,allLocations)
		{
			/*************adding locations in location combo box*************************/
			//location id is settled as combo index data 
			ui.cmbLocation->insertItem(k,info.getLocationName(),info.getLocationID());
			k++;//iteration
		}

		cmbLocationName_indexChange(ui.cmbLocation->currentIndex());
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
void Wizard::btnRegisterPC_click()
{
	if (ui.txtPCName->text().trimmed()=="" || ui.txtPCDescription->text().trimmed()=="" || ui.cmbRoom->currentText().trimmed()=="")
	{
		return;
	}
	writePCDetails();
	try
	{
		PCUtil *util=new PCUtil(this);
		SystemFetch *fetch=new SystemFetch();
		PCInfo info=util->loadPCDetails(fetch->getSystemID(),driverName);
		if(info.getPCID()==0)
		{
			//SHOW_MESSAGE(tr("Wizard"),tr("PC Registration Failed..."),tr("     Ok     "));
			ui.lblstatus->setText("PC Registration Failed...");
		}
		else
		{
			//SHOW_MESSAGE(tr("Wizard"),tr("PC Registered Successfully..."),tr("     Ok     "));
			ui.lblstatus->setText("PC Registered Successfully...");
		}
		delete fetch;
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
	catch(...)//catching unknown exceptions
	{
		PRINT_ERROR("UNKnown ERROR");
	}

	ui.tabWidget->setTabEnabled (4,true);
}

void Wizard::cmbLocationName_indexChange(int index_)
{
	RoomUtil *util =new RoomUtil(this);

	try
	{
		int locationID=ui.cmbLocation->itemData(index_,Qt::UserRole).toInt();
		//getting all rooms by location
		QList<RoomInfo> locationRooms=util->selectRoomsOfLocation(locationID,driverName);

		int k=0;

		ui.cmbRoom->blockSignals(true);
		ui.cmbRoom->clear();
		ui.cmbRoom->blockSignals(false);

		//inserting location details as widget items to tblLocations
		foreach(RoomInfo info,locationRooms)
		{
			ui.cmbRoom->insertItem(k,info.getRoomName(),info.getRoomID());

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

	ui.tabWidget->setTabEnabled (4,false);
}
void Wizard::cmbRoomName_indexChange(int i)
{
	ui.tabWidget->setTabEnabled (4,false);
}
void Wizard::writePCDetails()
{
	PCUtil *util=new PCUtil(this);

	try
	{
		SystemFetch *fetch=new SystemFetch();	
		PCInfo info;
		info.setPCNo(fetch->getSystemID());
		info.setPCName(ui.txtPCName->text().trimmed());
		info.setPCDescription(ui.txtPCDescription->text().trimmed());
		int roomID=ui.cmbRoom->itemData(ui.cmbRoom->currentIndex(),Qt::UserRole).toInt();
		info.setRoomID(roomID);
		delete fetch;

		util->insertOrUpdatePCDetails(info,driverName);
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
	catch(...)//catching unknown exceptions
	{
		PRINT_ERROR("UNKnown ERROR");
	}

	delete util;
}

void Wizard::writeAppSettings()
{
	QString tmpUploadPath=ui.txtServerPath->text();
	if(tmpUploadPath.right(1)=="\\")
	{
		tmpUploadPath=tmpUploadPath.replace(tmpUploadPath.size()-1,1,' ');
		tmpUploadPath=tmpUploadPath.trimmed();
	}
	AppSettingsUtil *util=new AppSettingsUtil(this);

	try
	{
		AppSettingsInfo info;

		HashManager *generate=new HashManager(this);

		info.setDriverName(ui.cmbDriver->currentText());
		info.setSharedServerIP(ui.txtServerName->text());
		info.setSharedServerPath(tmpUploadPath);
		info.setServerUserName(ui.txtServerUserName->text());
		info.setServerUserPassword(generate->encrypts(ui.txtServerPassword->text()));
		info.setFTPHostName(ui.txtHostName->text());
		info.setFTPPortNo(ui.txtPortNo->text().toInt());
		info.setFTPUserName(ui.txtFTPUserName->text());
		info.setFTPUserPassword(generate->encrypts(ui.txtFTPPassword->text()));

		int splittingDuration;
		if (ui.cmbSplit->currentIndex()!=5)
		{
			splittingDuration=ui.cmbSplit->currentText().toInt()*60;
		}
		else
		{
			splittingDuration=1000000;
		}

		info.setSplittingDuration(splittingDuration);

		DBServerConnection *cn=new DBServerConnection();

		info.setDatabaseServerMachine(cn->getComputerNameFromSQLServerInstance());

		delete cn;
		delete generate;

		SystemFetch *fetch=new SystemFetch();
		PCUtil *pUtil=new PCUtil(this);

		PCInfo pInfo=pUtil->loadPCDetails(fetch->getSystemID(),driverName);
		info.setPCID(pInfo.getPCID());

		delete pUtil;
		delete fetch;

		util->insertOrUpdateAppSettings(info,driverName);
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
			PRINT_WARNING("Empty Result Set");
		}
	}
	catch(...)//catching unknown exceptions
	{
		PRINT_ERROR("UNKnown ERROR");
	}

	delete util;
}

void Wizard::writeVideoSettings()
{
	bool videoDeviceExistence;
	AppSettingsUtil *appUtil=new AppSettingsUtil(this);
	SystemFetch *fetch=new SystemFetch();
	PCUtil *pUtil=new PCUtil(this);
	VideoDeviceUtil *util=new VideoDeviceUtil(this);
	try
	{
		AppSettingsInfo ainfo;

		PCInfo pInfo=pUtil->loadPCDetails(fetch->getSystemID(),driverName);

		ainfo=appUtil->loadAppSettings(pInfo.getPCID(),driverName);

		for (int j=0;j<ui.videoTableWidget->rowCount();j++)
		{
			QTableWidgetItem *itemtext=new QTableWidgetItem();
			QTableWidgetItem *itemtext2=new QTableWidgetItem();
			itemtext=ui.videoTableWidget->item(j,0);
			itemtext2=ui.videoTableWidget->item(j,1);
			QString VideoDevice=itemtext->text();
			QString Res=itemtext2->text();

		VideoDeviceinfo info;
		info.setVideoDeviceName(VideoDevice);
		info.setResolution(Res);
		info.setAppSettingsID(ainfo.getAppSettingsID());
		info.setIndexID(j+1);

		videoDeviceExistence=util->Indexexistence(j+1,ainfo.getAppSettingsID(),driverName);
		if (videoDeviceExistence==true)
		{
			info.setVisibility(1);
			util->insertOrUpdateAudioSettings(info,driverName);
		}
		else
		{
			info.setVisibility(1);
			util->insertOrUpdateAudioSettings(info,driverName);
			//return;
		}
		}
		QList<VideoDeviceinfo> allvideoDetails=util->selectVideoDeviceByAppID(ainfo.getAppSettingsID(),driverName);

		int k;
		k=1;
		foreach(VideoDeviceinfo info,allvideoDetails)
		{
			int count=ui.videoTableWidget->rowCount();
			if (k>count)
			{
				info.setVisibility(0);
				util->insertOrUpdateAudioSettings(info,driverName);
			}
			k++;
		}
		delete fetch;
		delete pUtil;
		delete appUtil;
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
	catch(...)//catching unknown exceptions
	{
		PRINT_ERROR("UNKnown ERROR");
	}

	delete util;

}

void Wizard::loadVideoDevices()
{

	int rcount=ui.videoTableWidget->rowCount();

	if (ui.videoTableWidget->rowCount()==0)
	{
		QTableWidgetItem *item1=new QTableWidgetItem();	
		item1->setText(VideoDeviceName);
		QTableWidgetItem *item2=new QTableWidgetItem();
		item2->setText(Resolution);

		ui.videoTableWidget->insertRow(rcount);
	
		ui.videoTableWidget->setItem(rcount,0,item1);		
		ui.videoTableWidget->setItem(rcount,1,item2);

		rcount++;
	}
	else
	{
		for (int j=0;j<ui.videoTableWidget->rowCount();j++)
		{
			QTableWidgetItem *itemtext=new QTableWidgetItem();
			QTableWidgetItem *itemtext2=new QTableWidgetItem();
			itemtext=ui.videoTableWidget->item(j,0);
			itemtext2=ui.videoTableWidget->item(j,1);
			QString text=itemtext->text();
			QString text1=itemtext2->text();

			if (text==VideoDeviceName)
			{
				return;
			}
		}

		QTableWidgetItem *item1=new QTableWidgetItem();	
		item1->setText(VideoDeviceName);
		QTableWidgetItem *item2=new QTableWidgetItem();
		item2->setText(Resolution);

		ui.videoTableWidget->insertRow(rcount);
		
		ui.videoTableWidget->setItem(rcount,0,item1);		
		ui.videoTableWidget->setItem(rcount,1,item2);

		rcount++;
	}	
}

void Wizard::getvideoDevices(QString device_)
{
	VideoDeviceName=device_;
}
void Wizard::getResolution(QString reslo_)
{
	Resolution=reslo_;	
}

void Wizard::writeAudioSettings()
{
	bool audioDeviceExistence;
	AppSettingsUtil *appUtil=new AppSettingsUtil(this);
	SystemFetch *fetch=new SystemFetch();
	PCUtil *pUtil=new PCUtil(this);
	AudioDeviceUtil *util=new AudioDeviceUtil(this);
	try
	{
		AppSettingsInfo appinfo;

		PCInfo pInfo=pUtil->loadPCDetails(fetch->getSystemID(),driverName);

		appinfo=appUtil->loadAppSettings(pInfo.getPCID(),driverName);
		for (int j=0;j<ui.audioTableWidget->rowCount();j++)
		{
			QTableWidgetItem *itemtext=new QTableWidgetItem();
			QTableWidgetItem *itemtext2=new QTableWidgetItem();
			itemtext=ui.audioTableWidget->item(j,0);
			itemtext2=ui.audioTableWidget->item(j,1);
			QString AudioDevice=itemtext->text();
			QString type=itemtext2->text();

		AudioDeviceinfo info,ainfo;
		info.setAudioDeviceName(AudioDevice);
		info.setAudioType(type);
		info.setAppSettingsID(appinfo.getAppSettingsID());
		info.setIndexID(j+1);
		
		ainfo=util->loadAudioDevice(appinfo.getAppSettingsID(),driverName);
		audioDeviceExistence=util->Indexexistence(j+1,appinfo.getAppSettingsID(),driverName);
		if (audioDeviceExistence==true)
		{	info.setVisibility(1);
			util->insertOrUpdateAudioSettings(info,driverName);
		}
		else
		{
			info.setVisibility(1);
			util->insertOrUpdateAudioSettings(info,driverName);
		
		}
		}
	QList<AudioDeviceinfo> allAudioDetails=util->selectAllAudioDeviceByAppID(appinfo.getAppSettingsID(),driverName);

	int k;
	k=1;
	foreach(AudioDeviceinfo info,allAudioDetails)
	{
		int c=ui.audioTableWidget->rowCount();
		if (k>c)
		{
			info.setVisibility(0);
			util->insertOrUpdateAudioSettings(info,driverName);
		}
		k++;
	}

		delete fetch;
		delete pUtil;
		delete appUtil;
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
	catch(...)//catching unknown exceptions
	{
		PRINT_ERROR("UNKnown ERROR");
	}

	delete util;
}

void Wizard::loadAudioDevices()
{

	int rcount=ui.audioTableWidget->rowCount();

	if (ui.audioTableWidget->rowCount()==0)
	{
		QTableWidgetItem *item1=new QTableWidgetItem();	
		item1->setText(AudioDeviceName);
		QTableWidgetItem *item2=new QTableWidgetItem();
		item2->setText(AudioType);

		ui.audioTableWidget->insertRow(rcount);
	
		ui.audioTableWidget->setItem(rcount,0,item1);		
		ui.audioTableWidget->setItem(rcount,1,item2);
		rcount++;
	}
	else
	{
		for (int j=0;j<ui.audioTableWidget->rowCount();j++)
		{
			QTableWidgetItem *itemtext=new QTableWidgetItem();
			QTableWidgetItem *itemtext2=new QTableWidgetItem();
			itemtext=ui.audioTableWidget->item(j,0);
			itemtext2=ui.audioTableWidget->item(j,1);
			QString text=itemtext->text();
			QString text1=itemtext2->text();

			if (text==AudioDeviceName && text1==AudioType)
			{
				return;
			}
		}

		QTableWidgetItem *item1=new QTableWidgetItem();	
		item1->setText(AudioDeviceName);
		QTableWidgetItem *item2=new QTableWidgetItem();
		item2->setText(AudioType);

		ui.audioTableWidget->insertRow(rcount);
	
		ui.audioTableWidget->setItem(rcount,0,item1);		
		ui.audioTableWidget->setItem(rcount,1,item2);
		rcount++;
	}
}

void Wizard::getAudioDevices(QString device_)
{
	AudioDeviceName=device_;
}

void Wizard::getAudioType(QString type_)
{
	AudioType=type_;
}

void Wizard::readPCDetails()
{
	PCUtil *util=new PCUtil(this);
	try
	{
		SystemFetch *fetch=new SystemFetch();	
		PCInfo info=util->loadPCDetails(fetch->getSystemID(),driverName);
		delete fetch;

		ui.txtPCDescription->setText(info.getPCDescription());
		RoomUtil *rUtil=new RoomUtil(this);
		int roomID=info.getRoomID();
		RoomInfo rInfo=rUtil->loadRoom(roomID,driverName);
		int locationIndex=ui.cmbLocation->findData(rInfo.getLocationID(),Qt::UserRole);
		ui.cmbLocation->setCurrentIndex(locationIndex);
		cmbLocationName_indexChange(locationIndex);
		delete rUtil;
		int roomIndex=ui.cmbRoom->findData(roomID,Qt::UserRole);
		ui.cmbRoom->setCurrentIndex(roomIndex);
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
			PRINT_WARNING("Empty Result set");
		}
	}
	catch(...)//catching unknown exceptions
	{
		PRINT_ERROR("UNKnown ERROR");
	}

	delete util;
}

void Wizard::readAppSettings()
{
	AppSettingsUtil *util=new AppSettingsUtil(this);

	try
	{
		AppSettingsInfo info;

		HashManager *generate=new HashManager(this);
		SystemFetch *fetch=new SystemFetch();
		PCUtil *pUtil=new PCUtil(this);

		PCInfo pInfo=pUtil->loadPCDetails(fetch->getSystemID(),driverName);

		info=util->loadAppSettings(pInfo.getPCID(),driverName);

		if(info.getSharedServerIP()!="")
		{
			ui.txtServerName->setText(info.getSharedServerIP());
			ui.txtServerUserName->setText(info.getServerUserName());
			ui.txtServerPassword->setText(generate->decrypts(info.getServerUserPassword()));
			ui.txtServerPath->setText(info.getSharedServerPath());
			ui.txtHostName->setText(info.getFTPHostName());
			ui.txtPortNo->setText(QString::number(info.getFTPPortNo()));
			ui.txtFTPUserName->setText(info.getFTPUserName());
			ui.txtFTPPassword->setText(generate->decrypts(info.getFTPUserPassword()));

			if (info.getSplittingDuration()==300)
			{
				ui.cmbSplit->setCurrentIndex(0);
			}
			else if(info.getSplittingDuration()==600)
			{
				ui.cmbSplit->setCurrentIndex(1);
			}
			else if(info.getSplittingDuration()==900)
			{
				ui.cmbSplit->setCurrentIndex(2);
			}
			else if (info.getSplittingDuration()==1800)
			{
				ui.cmbSplit->setCurrentIndex(3);
			}
			else if (info.getSplittingDuration()==36000)
			{
				ui.cmbSplit->setCurrentIndex(4);
			}
			else
			{
				ui.cmbSplit->setCurrentIndex(5);
			}
		}

		delete pUtil;
		delete fetch;
		delete generate;
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

void Wizard::readAudioSettings()
{
	AudioDeviceUtil *util=new AudioDeviceUtil(this);
	AppSettingsUtil *appUtil=new AppSettingsUtil(this);
	SystemFetch *fetch=new SystemFetch();
	PCUtil *pUtil=new PCUtil(this);
	try
	{
		AppSettingsInfo appinfo;
		PCInfo pInfo=pUtil->loadPCDetails(fetch->getSystemID(),driverName);

		appinfo=appUtil->loadAppSettings(pInfo.getPCID(),driverName);
		QList<AudioDeviceinfo> allAudioDetails=util->selectAudioDevice(appinfo.getAppSettingsID(),1,driverName);

		int k=0;

		for (int i=ui.audioTableWidget->rowCount(); i >= 0; --i)
		{
			ui.audioTableWidget->removeRow(i);	
		}

		foreach(AudioDeviceinfo info,allAudioDetails)
		{
			//creating table widget items with corresponding text
			QTableWidgetItem *item0=new QTableWidgetItem();
			item0->setText(info.getAudioDeviceName());
			item0->setData(Qt::UserRole,info.getAudioDeviceID());
			QTableWidgetItem *item1=new QTableWidgetItem();	
			item1->setText(info.getAudioType());

			//inserting row in table locations
			ui.audioTableWidget->insertRow(k);

			//inserting widget items in table location		
			ui.audioTableWidget->setItem(k,0,item0);		
			ui.audioTableWidget->setItem(k,1,item1);

			k++;//iteration
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

void Wizard::readVideoSettings()
{
	VideoDeviceUtil *util=new VideoDeviceUtil(this);
	AppSettingsUtil *appUtil=new AppSettingsUtil(this);
	SystemFetch *fetch=new SystemFetch();
	PCUtil *pUtil=new PCUtil(this);
	try
	{
		AppSettingsInfo appinfo;
		PCInfo pInfo=pUtil->loadPCDetails(fetch->getSystemID(),driverName);

		appinfo=appUtil->loadAppSettings(pInfo.getPCID(),driverName);
		QList<VideoDeviceinfo> allVideoDetails=util->selectVideoDevice(appinfo.getAppSettingsID(),1,driverName);

		int k=0;

		for (int i=ui.videoTableWidget->rowCount(); i >= 0; --i)
		{
			ui.videoTableWidget->removeRow(i);	
		}

		foreach(VideoDeviceinfo info,allVideoDetails)
		{
			//creating table widget items with corresponding text
			QTableWidgetItem *item0=new QTableWidgetItem();
			item0->setText(info.getVideoDeviceName());
			item0->setData(Qt::UserRole,info.getVideoDeviceID());
			QTableWidgetItem *item1=new QTableWidgetItem();	
			item1->setText(info.getResolution());

			//inserting row in table locations
			ui.videoTableWidget->insertRow(k);

			//inserting widget items in table location		
			ui.videoTableWidget->setItem(k,0,item0);		
			ui.videoTableWidget->setItem(k,1,item1);

			k++;//iteration
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
void Wizard::wizardTabChanged(int index_)
{
	ui.lblstatus->clear();

// 	if (index_==3)
// 	{
		try
		{
			PCUtil *util=new PCUtil(this);
			SystemFetch *fetch=new SystemFetch();
			PCInfo info=util->loadPCDetails(fetch->getSystemID(),driverName);

			if(info.getPCID()==0)
			{
				ui.tabWidget->setTabEnabled (4,false);
			}
			else
			{
				ui.tabWidget->setTabEnabled (4,true);
			}

			delete util;
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

	//}

	if (index_==4)
	{
		ui.btnOk->setEnabled(true);
	}
	else
	{
		ui.btnOk->setEnabled(false);
	}
}

bool Wizard::validateFields()
{
	QString server=ui.txtServerName->text().trimmed();
	QString serverAdminName=ui.txtServerUserName->text().trimmed();
	QString serverAdminPass=ui.txtServerPassword->text().trimmed();
	QString uploadPath=ui.txtServerPath->text().trimmed();
	QString domainController=ui.cmbDomainName->currentText().trimmed();

	if(server=="" || serverAdminName=="" || serverAdminPass=="" || uploadPath=="" || domainController=="")
	{
		return false;
	}

	QString pcName=ui.txtPCName->text().trimmed();
	QString pcDesc=ui.txtPCDescription->text().trimmed();
	QString pcLoc=ui.cmbLocation->currentText().trimmed();
	QString pcRoom=ui.cmbRoom->currentText().trimmed();

	if (pcName=="" || pcDesc=="" || pcLoc=="" || pcRoom=="")
	{
		return false;
	}

	QString bitRate=ui.txtBitRate->text().trimmed();
	QString quality=ui.txtQuality->text().trimmed();
	QString frame=ui.txtKeyFrame->text().trimmed();


		if(bitRate=="" || quality=="" || frame=="")
		{
			return false;
		}
	return true;
}

Wizard::~Wizard()
{

}
