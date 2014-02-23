/* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "uiloginuser.h"

#include "hashmanager.h"
#include "domainoperations.h"
#include "systemfetch.h"
#include "permissions.h"
#include "pcsettings.h"

#include "userutil.h"
#include "pcutil.h"
#include "userlocationutil.h"
#include "userroomutil.h"
#include "userroomeventtypeutil.h"
#include "locationutil.h"
#include "roomutil.h"
#include "eventtypeutil.h"
#include "roomeventtypeutil.h"

#include <QSettings>
#include <QCompleter>
#include <QDir>

LoginUser::LoginUser(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	
	translator=new QTranslator(0);
	logSession=LoginUserSession::session();

	icoDown=QIcon(QPixmap(":/Recorder/Resources/down.png"));
	icoTop=QIcon(QPixmap(":/Recorder/Resources/top.png"));

	resize(376,160);

	this->setMinimumSize(376,160);
	this->setMaximumSize(376,160);

	ui.frame->setHidden(true);
	ui.btnMore->setIcon(icoDown);

	loginStatus=false;

	connect(ui.btnLogin,	SIGNAL(clicked()),	this,SLOT(btnLogin_click()));
	connect(ui.btnCancel,	SIGNAL(clicked()),	this,SLOT(btnCancel_click()));
	connect(ui.txtUserName,	SIGNAL(editingFinished()),	this,SLOT(readSavedPassword()));
	connect(ui.txtUserName,	SIGNAL(textChanged(QString)),	this,SLOT(clearUserPswrdEdit(QString)));
	connect(ui.btnMore,		SIGNAL(clicked()),	this,SLOT(btnMore_click()));
	connect(ui.cmbLocation,	SIGNAL(currentIndexChanged(int)),	this,SLOT(cmbLocation_indexChange(int)));
	//connect(ui.cmbLanguages,	SIGNAL(currentIndexChanged(int)),	this,SLOT(cmbLanguages_indexChange(int)));

	readLastLogin();

}
// void LoginUser::cmbLanguages_indexChange(int index_)
// {
// 	ui.cmbLanguages->blockSignals(true);
// 	if(index_==1)
// 	{
// 		translator->load("Recorder_Ar.qm");
// 		qApp->installTranslator(translator);
// 		qApp->setLayoutDirection(Qt::RightToLeft); 
// 		this->setLayoutDirection(Qt::RightToLeft);
// 		ui.grpLogin->setLayoutDirection(Qt::RightToLeft);
// 		ui.retranslateUi(this);	
// 
// 		ui.cmbLanguages->setCurrentIndex(1);
// 	}
// 	else
// 	{
// 		translator->load("Recorder_En.qm");
// 		qApp->installTranslator(translator);
// 		qApp->setLayoutDirection(Qt::LeftToRight);  		
// 		this->setLayoutDirection(Qt::LeftToRight);
// 		ui.grpLogin->setLayoutDirection(Qt::LeftToRight);
// 		ui.retranslateUi(this);
// 
// 		ui.cmbLanguages->setCurrentIndex(0);
// 	}
// 	ui.cmbLanguages->blockSignals(false);
// }
bool LoginUser::checkDBConnection()
{
	readSettings();

	if(connectDB()!=true)
	{		
		SHOW_MESSAGE(tr("DLCRS Login"),tr("Database connection failed!!!"),tr("Ok"));
		return false;
	}
	else
	{
		refreshDialog();

		if (checkPCRegistration()!=true)
		{
			return false;
		}

		return true;
	}
}
void LoginUser::refreshDialog()
{
	loadPCLocation();
	setupCompleter();
	loadLocations();

	int locationIndex;

	RoomUtil *util=new RoomUtil(this);

	try
	{
		RoomInfo info=util->loadRoom(logSession->getPCRoomID(),logSession->getDriverName());
		locationIndex=ui.cmbLocation->findData(info.getLocationID(),Qt::UserRole);
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
	catch(...)//catching unknown exwptions
	{
		PRINT_ERROR("UNKnown ERROR");
	}

	delete util;

	ui.cmbLocation->setCurrentIndex(locationIndex);

	int roomIndex=ui.cmbRoom->findData(logSession->getPCRoomID(),Qt::UserRole);
	ui.cmbRoom->setCurrentIndex(roomIndex);

}
void LoginUser::loadLocations()
{
	LocationUtil *util =new LocationUtil(this);

	try
	{
		//getting all location details
		QList<LocationInfo> allLocations=util->selectAllLocations(logSession->getDriverName());

		int k=0;

		/*************clearing all location combo box*************************/
		//block signals is to prevent unusual index change signal emits

		ui.cmbLocation->blockSignals(true);
		ui.cmbLocation->clear();
		ui.cmbLocation->blockSignals(false);

		/*************clearing all location combo box end*************************/

		//inserting location details as widget items to tblLocations
		foreach(LocationInfo info,allLocations)
		{
			ui.cmbLocation->insertItem(k,info.getLocationName(),info.getLocationID());
		
			k++;//iteration	
		}
		//refreshing and loading location rooms by index
		cmbLocation_indexChange(ui.cmbLocation->currentIndex());
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
void LoginUser::cmbLocation_indexChange(int index_)
{
	RoomUtil *util =new RoomUtil(this);

	try
	{
		int locationID=ui.cmbLocation->itemData(index_,Qt::UserRole).toInt();
		//getting all rooms by location
		QList<RoomInfo> locationRooms=util->selectRoomsOfLocation(locationID,logSession->getDriverName());

		int k=0;

		ui.cmbRoom->blockSignals(true);
		ui.cmbRoom->clear();
		ui.cmbRoom->blockSignals(false);

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
}

void LoginUser::btnMore_click()
{	
	if (ui.btnMore->isChecked()!=false)
	{
		resize(376,255);
		
		this->setMinimumSize(376,235);
		this->setMaximumSize(376,235);

		ui.btnMore->setIcon(icoTop);
		
		ui.frame->show();
	}
	else
	{
		ui.frame->hide();

		resize(376,160);

		this->setMinimumSize(376,160);
		this->setMaximumSize(376,160);

		ui.btnMore->setIcon(icoDown);

		loadLocations();
	}
}
void LoginUser::readSavedPassword()
{

	PCSettings *settingsSaved=new PCSettings(logSession->getPCID(),UserScope);
	HashManager *generate=new HashManager(this);

	QString key=logSession->getPCNo()+"_"+ui.txtUserName->text().trimmed()+"_PWD";

	try
	{
		QString passWord=settingsSaved->value(key).toString();
		
		if (passWord!="")
		{
			ui.txtUserPassword->setText(generate->decrypts(passWord));
			ui.chkSaveUser->setChecked(true);
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

	delete generate;
	delete settingsSaved;	
}
void LoginUser::clearUserPswrdEdit(QString user_)
{
	ui.txtUserPassword->clear();	
	ui.chkSaveUser->setChecked(false);
	readSavedPassword();
}
void LoginUser::setupCompleter()
{
	
	PCSettings *savedSettings=new PCSettings(logSession->getPCID());

	try
	{
		QString key=logSession->getPCNo()+"_%_PWD";

		QStringList keyList=savedSettings->loadSimilerKeys(key);
		QStringList savedUsers;

		for(int i=0;i<keyList.count();i++)
		{
			QString tempString=keyList[i];
			QStringList tempList=tempString.split("_");
			savedUsers<<tempList[1];
		}

		QCompleter *completer = new QCompleter(savedUsers,NULL);
		completer->setCompletionMode(QCompleter::PopupCompletion);
		completer->setCaseSensitivity(Qt::CaseInsensitive);
		ui.txtUserName->setCompleter(completer);

		readSavedPassword();
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

	delete savedSettings;
}
void LoginUser::btnLogin_click()
{
	if (ui.txtUserName->text().trimmed()=="" || ui.txtUserPassword->text().trimmed()=="")
	{
		return;
	}
	
	QString userName=ui.txtUserName->text().trimmed();
	QString passWord=ui.txtUserPassword->text();
	bool validUser=false;

	readSettings();

	DBServerConnection *cn=new DBServerConnection();

	if(!cn->isConnected())
	{
		if(connectDB()!=true)
		{		
			SHOW_MESSAGE(tr("DLCRS Login"),tr("Database connection failed!!!"),tr("Ok"));
			return;
		}
	}

	if (checkPCRegistration()!=true)
	{
		return;
	}	

	if (cn->getHostName()!=cn->getComputerNameFromSQLServerInstance())
	{
		logSession->setServerDBStatus(true);		
	}

	delete cn;	
	
	DomainOperations *userDomain=new DomainOperations();
	UserUtil *uUtil=new UserUtil(this);
	HashManager *generate=new HashManager(this);

	if(userDomain->authenticateDomainUser(logSession->getDomainControllerName(),userName,passWord)==false)
	{
		if(uUtil->authenticateUser(userName,generate->encrypts(passWord),logSession->getDriverName())==false)
		{
			SHOW_MESSAGE(tr("DLCRS Login"),tr("Authentication failed!!!"),tr("Ok"));
			return;
		}
		else
		{
			validUser=true;
		}		
	}
	else
	{
		if(uUtil->existence(userName,logSession->getDriverName())==false)
		{			
			SHOW_MESSAGE(tr("DLCRS Login"),tr("Authentication failed!!!"),tr("Ok"));
			return;
		}
		else
		{
			updateDomainPassword(userName,generate->encrypts(passWord));
			
			validUser=true;
		}
	}
	
	delete generate;
	delete uUtil;
	delete userDomain;
	
	writeSettings();
	 	
	if(validUser==true)
	{
		loadUserPermissions(userName);
		loadUserLocations();	
	}

	int locationID=ui.cmbLocation->itemData(ui.cmbLocation->currentIndex(),Qt::UserRole).toInt();
	int roomID=ui.cmbRoom->itemData(ui.cmbRoom->currentIndex(),Qt::UserRole).toInt();

	if(roomID==QVariant::Invalid)
	{
		QString msg=tr("No Valid Rooms Were Found In ")+ui.cmbLocation->currentText()+tr(". Please Verify Your Input.");
		SHOW_MESSAGE(tr("Unable To Login"),msg,tr("Ok"));
		return;
	}
	
	QList<int> roomIDList=logSession->getUserRoomIDs();
	if(roomIDList.contains(roomID)!=true)
	{
		QString msg=tr("You do not have access in ")+ui.cmbRoom->currentText()+", "+ui.cmbLocation->currentText()+tr(". Please verify your input or contact your administrator.");
		SHOW_MESSAGE(tr("Unable To Login"),msg,tr("Ok"));
		return;
	}

	if (ui.btnMore->isChecked()!=false && roomID!=logSession->getPCRoomID())
 	{
		hide();

		QString msg=tr("Please confirm that you Want to login to ")+ui.cmbRoom->currentText()+", "+ui.cmbLocation->currentText();

		QMessageBox msgBox(this);
		msgBox.setWindowTitle(tr("Advanced Login"));
		msgBox.setIcon(QMessageBox::Information);
		msgBox.setText(tr("<b>You are using an advanced Login facility.</b>"));
		msgBox.setInformativeText(msg);
// 		msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
// 		msgBox.setDefaultButton(QMessageBox::No);
		QPushButton *okButton = msgBox.addButton(tr("YES"), QMessageBox::YesRole);
		QPushButton *noButton = msgBox.addButton(tr("NO"), QMessageBox::NoRole);
		int ret=msgBox.exec();
		if (ret == 1)
		{
			msgBox.close();
			show();
			return;
		}	
		else
		{
			msgBox.close();			
		}		
 	}

	if (validUser!=false)
	{
		logSession->setCurrentLocation(locationID);
		logSession->setCurrentRoom(roomID);
		logSession->setLanguageIndex(ui.cmbLanguages->currentIndex());
		setupRecorder();
	}
}

void LoginUser::btnCancel_click()
{
	this->close();
}
bool LoginUser::checkPCRegistration()
{
	bool pcStatus=true;

	PCUtil *pUtil=new PCUtil(this);

	try
	{
		SystemFetch *fetch=new SystemFetch();

		PCInfo pInfo=pUtil->loadPCDetails(fetch->getSystemID(),logSession->getDriverName());		

		if(pInfo.getPCNo()!=fetch->getSystemID())
		{		
			SHOW_INFOMESSAGE(tr("DLCRS Login"),tr("<b>Your PC must be registered, please contact your administrator.</b>"),tr("PC authentication failed!!!"),tr("Ok"));
			pcStatus=false;
		}
		else
		{
			if (pInfo.getPCName()!=fetch->getSystemName())
			{		
				SHOW_INFOMESSAGE(tr("DLCRS Login"),tr("<b>Your PC must be re-registered. Please contact your administrator.</b>"),tr("PC name changed!!!"),tr("Ok"));
				pcStatus=false;
			}
		}	

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

	return pcStatus;
}
void LoginUser::updateDomainPassword(QString userName_,QString passWord_)
{ 
	UserUtil *uUtil=new UserUtil(this);

	try
	{
		uUtil->updatePassword(userName_,passWord_,logSession->getDriverName());
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

	delete uUtil;
}
void LoginUser::setupRecorder()
{	
	loginStatus=true;
	close();
}
void LoginUser::loadUserPermissions(QString userName_)
{	
	Permissions *userPermissions=new Permissions(userName_);

	if(userPermissions->canStartEvent()!=false) logSession->setStartEvent(true);
	else logSession->setStartEvent(false);

	if(userPermissions->canResumeEvent()!=false) logSession->setResumeEvent(true);
	else logSession->setResumeEvent(false);

	if(userPermissions->canCloseEvent()!=false) logSession->setCloseEvent(true);
	else logSession->setCloseEvent(false);

	if(userPermissions->canReopenEvent()!=false) logSession->setReopenEvent(true);
	else logSession->setReopenEvent(false);

	if(userPermissions->canViewClosedEvent()!=false) logSession->setViewClosedEvent(true);
	else logSession->setViewClosedEvent(false);

	if(userPermissions->canRetrieveEvent()!=false) logSession->setRetrieveEvent(true);
	else logSession->setRetrieveEvent(false);

	if(userPermissions->canEditRetrieved()!=false) logSession->setEditRetrieved(true);
	else logSession->setEditRetrieved(false);

	if(userPermissions->canExportToFile()!=false) logSession->setExportToFile(true);
	else logSession->setExportToFile(false);

	if(userPermissions->canPrintAccess()!=false) logSession->setPrintAccess(true);
	else logSession->setPrintAccess(false);

	if(userPermissions->canAttachFile()!=false) logSession->setAttachFile(true);
	else logSession->setAttachFile(false);

	if(userPermissions->canDownloadFile()!=false) logSession->setDownloadFile(true);
	else logSession->setDownloadFile(false);

	if(userPermissions->canUploadToFTP()!=false) logSession->setUploadToFTP(true);
	else logSession->setUploadToFTP(false);

	if(userPermissions->canWriteOpticalDisk()!=false) logSession->setWriteOpticalDisk(true);
	else logSession->setWriteOpticalDisk(false);

	if(userPermissions->canWriteRemovableDisk()!=false) logSession->setWriteRemovableDisk(true);
	else logSession->setWriteRemovableDisk(false);

	delete userPermissions;
}
void LoginUser::loadUserLocations()
{
	try
	{
		UserLocationUtil *ulUtil=new UserLocationUtil(this);

		QList<int> ulList=ulUtil->loadUserLocationIDs(logSession->getUserID(),logSession->getDriverName());
		logSession->setUserLocationIDs(ulList);

		delete ulUtil;

		UserRoomUtil *urUtil=new UserRoomUtil(this);
		
		QList<int> urList=urUtil->loadUserRoomIDs(logSession->getUserID(),logSession->getDriverName());
		logSession->setUserRoomIDs(urList);

		delete urUtil;

		UserRoomEventTypeUtil *ureUtil=new UserRoomEventTypeUtil(this);

		QList<int> ureList=ureUtil->loadUserRoomEventTypeIDs(logSession->getUserID(),logSession->getDriverName());
		logSession->setUserRoomEventTypeIDs(ureList);

		delete ureUtil;

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
}
void LoginUser::loadPCLocation()
{
	PCUtil *pUtil=new PCUtil(this);

	try
	{
		SystemFetch *fetch=new SystemFetch();

		PCInfo info=pUtil->loadPCDetails(fetch->getSystemID(),logSession->getDriverName());

		logSession->setPCID(info.getPCID());
		logSession->setPCNo(info.getPCNo());
		logSession->setPCName(info.getPCName());
		logSession->setPCRoomID(info.getRoomID());
		logSession->setPCCreatedDate(info.getCreatedDate());

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
}
void LoginUser::readSettings()
{
	QSettings settings("WConfig.ini",QSettings::IniFormat);
	
	HashManager *generate = new HashManager(this);

	logSession->setDBServerName(settings.value("SERVER").toString());
	logSession->setDBName(settings.value("DATABASE").toString());
	logSession->setDBAdminName(settings.value("UID").toString());
	logSession->setDBAdminPassWord(generate->decrypts(settings.value("PWD").toString()));
	logSession->setDomainControllerName(settings.value("DomainController").toString());
	logSession->setDriverName(settings.value("DRIVER").toString());

	delete generate;
}
void LoginUser::readLastLogin()
{
	QString settingsPath=QDir::homePath()+"/Application Data/Evidencer/Settings/UserConfig.ini";

	QSettings settings(settingsPath,QSettings::IniFormat);
	
	ui.cmbLanguages->setCurrentIndex(settings.value("LanguageIndex").toInt());
	ui.txtUserName->blockSignals(true);	
	ui.txtUserName->setText(settings.value("LastLogin").toString());
	ui.txtUserName->blockSignals(false);	
}
void LoginUser::writeSettings()
{
	QString settingsPath=QDir::homePath()+"/Application Data/Evidencer/Settings/UserConfig.ini";

	QSettings settings(settingsPath,QSettings::IniFormat);
	
	settings.setValue("LanguageIndex",ui.cmbLanguages->currentIndex());
	settings.setValue("LastLogin",ui.txtUserName->text());	
	
	try
	{
		PCSettings *settingsSaved=new PCSettings(logSession->getPCID(),UserScope);
		HashManager *generate=new HashManager(this);

		QString key=logSession->getPCNo()+"_"+ui.txtUserName->text().trimmed()+"_PWD";

		if(ui.chkSaveUser->isChecked()==true)
		{			
			settingsSaved->setValue(key,generate->encrypts(ui.txtUserPassword->text()));
		}
		else
		{
			settingsSaved->clear(key);
		}

		delete generate;
		delete settingsSaved;
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

}
bool LoginUser::connectDB()
{
	QSqlDatabase db;

	db=QSqlDatabase::database("Inst_SQLServer"); 
	if(NOT db.isOpen())
	{
		db=QSqlDatabase::addDatabase("QODBC","Inst_SQLServer");

		QString dsn=QString("DRIVER={SQL Native Client};SERVER=%1;DATABASE=%2;UID=%3;PWD=%4;").arg(logSession->getDBServerName()).arg(logSession->getDBName()).arg(logSession->getDBAdminName()).arg(logSession->getDBAdminPassWord());
		
		db.setDatabaseName(dsn);	

		if(!db.open())
		{
			PRINT_ERROR(db.lastError().text());
			return false;
		}
		return true;
	}
	else
	{
		return true;		
	}	
}
int LoginUser::languageIndex()
{
	return ui.cmbLanguages->currentIndex();
}
bool LoginUser::isValidUser()
{
	return loginStatus;
}
LoginUser::~LoginUser()
{

}
