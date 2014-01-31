#include "loginusersession.h"

//default constructor
LoginUserSession::LoginUserSession()
{
	dbServerName="";
	dbName="";
	dbAdminName="";
	dbAdminPassWord="";
	serverDBStatus=false;
}
//setting data source name
//<param name="dataSourceName_">Used as the name of data source</param>
void LoginUserSession::setDBServerName(QString dbServerName_)
{
	dbServerName=dbServerName_;
}
//getting data source name
//<returns>returns a string value which defines the data source name</returns>
QString LoginUserSession::getDBServerName()
{
	return dbServerName;
}
//setting data source name
//<param name="dataSourceName_">Used as the name of data source</param>
void LoginUserSession::setDBName(QString dbName_)
{
	dbName=dbName_;
}
//getting data source name
//<returns>returns a string value which defines the data source name</returns>
QString LoginUserSession::getDBName()
{
	return dbName;
}
//setting database administrator name
//<param name="dataSourceName_">Used as the name of db administrator</param>
void LoginUserSession::setDBAdminName(QString dbAdminName_)
{
	dbAdminName=dbAdminName_;
}
//getting database administrator name
//<returns>returns a string value which defines the name of db administrator</returns>
QString LoginUserSession::getDBAdminName()
{
	return dbAdminName;
}
//setting database administrator password
//<param name="dataSourceName_">Used as the password of db administrator</param>
void LoginUserSession::setDBAdminPassWord(QString dbAdminPassWord_)
{
	dbAdminPassWord=dbAdminPassWord_;
}
//getting database administrator password
//<returns>returns a string value which defines the password of db administrator</returns>
QString LoginUserSession::getDBAdminPassWord()
{
	return dbAdminPassWord;
}
void LoginUserSession::setFTPServer(QString ftpServer_)
{
	ftpServerName=ftpServer_;
}
QString LoginUserSession::getFTPServer()
{
	return ftpServerName;
}

void LoginUserSession::setFTPPort(int ftpPort_)
{
	ftpPortNo=ftpPort_;
}
int LoginUserSession::getFTPPort()
{
	return ftpPortNo;
}

void LoginUserSession::setFTPAdmin(QString admin_)
{
	ftpUserName=admin_;
}
QString LoginUserSession::getFTPAdmin()
{
	return ftpUserName;
}

void LoginUserSession::setFTPPassword(QString password_)
{
	ftpPassword=password_;
}
QString LoginUserSession::getFTPPassword()
{
return ftpPassword;
}
void LoginUserSession::setDomainControllerName(QString controller_)
{
	domainControllerName=controller_;
}
QString LoginUserSession::getDomainControllerName()
{
	return domainControllerName;
}
void LoginUserSession::setServerDBStatus(bool status_)
{
	serverDBStatus=status_;
}
bool LoginUserSession::getServerDBStatus()
{
	return serverDBStatus;
}
//setting user id
void LoginUserSession::setUserID(int id_)
{
	userID=id_;
}
//getting user id
int LoginUserSession::getUserID()
{
	return userID;
}
//setting user name
void LoginUserSession::setUserName(QString name_)
{
	userName=name_;
}
//getting user name
QString LoginUserSession::getUserName()
{
	return userName;
}
//setting user type
void LoginUserSession::setUserType(int type_)
{
	userType=type_;
}
//getting user type
int LoginUserSession::getUserType()
{
	return userType;
}
void LoginUserSession::setUserCategoryID(int id_)
{
	userCategoryID=id_;
}
//getting category id
int LoginUserSession::getUserCategoryID()
{
	return userCategoryID;
}
//setting created date
void LoginUserSession::setUserCreatedDate(QDateTime dt_)
{
	userCreatedDate=dt_;
}
//getting created date
QDateTime LoginUserSession::getUserCreatedDate()
{
	return userCreatedDate;
}
void LoginUserSession::setStartEvent(bool status_)
{
	startEvent=status_;
}
bool LoginUserSession::canStartEvent()
{
	return startEvent;
}
void LoginUserSession::setResumeEvent(bool status_)
{
	resumeEvent=status_;
}
bool LoginUserSession::canResumeEvent()
{
	return resumeEvent;
}
void LoginUserSession::setCloseEvent(bool status_)
{
	closeEvent=status_;
}
bool LoginUserSession::canCloseEvent()
{
	return closeEvent;
}
void LoginUserSession::setReopenEvent(bool status_)
{
	reopenEvent=status_;
}
bool LoginUserSession::canReopenEvent()
{
	return reopenEvent;
}
void LoginUserSession::setViewClosedEvent(bool status_)
{
	viewClosedEvent=status_;
}
bool LoginUserSession::canViewClosedEvent()
{
	return viewClosedEvent;
}
void LoginUserSession::setRetrieveEvent(bool status_)
{
	retrieveEvent=status_;
}
bool LoginUserSession::canRetrieveEvent()
{
	return retrieveEvent;
}
void LoginUserSession::setEditRetrieved(bool status_)
{
	editRetrieved=status_;
}
bool LoginUserSession::canEditRetrieved()
{
	return editRetrieved;
}
void LoginUserSession::setExportToFile(bool status_)
{
	exportToFile=status_;
}
bool LoginUserSession::canExportToFile()
{
	return exportToFile;
}
void LoginUserSession::setPrintAccess(bool status_)
{
	printAccess=status_;
}
bool LoginUserSession::canPrintAccess()
{
	return printAccess;
}
void LoginUserSession::setAttachFile(bool status_)
{
	attachFile=status_;
}
bool LoginUserSession::canAttachFile()
{
	return attachFile;
}
void LoginUserSession::setDownloadFile(bool status_)
{
	downloadFile=status_;
}
bool LoginUserSession::canDownloadFile()
{
	return downloadFile;
}
void LoginUserSession::setUploadToFTP(bool status_)
{
	uploadToFTP=status_;
}
bool LoginUserSession::canUploadToFTP()
{
	return uploadToFTP;
}
void LoginUserSession::setWriteOpticalDisk(bool status_)
{
	writeOpticalDisk=status_;
}
bool LoginUserSession::canWriteOpticalDisk()
{
	return writeOpticalDisk;
}
void LoginUserSession::setWriteRemovableDisk(bool status_)
{
	writeRemovableDisk=status_;
}
bool LoginUserSession::canWriteRemovableDisk()
{
	return writeRemovableDisk;
}
void LoginUserSession::setUserLocationIDs(QList<int> idList_)
{
	userLocationIDs.clear();
	userLocationIDs=idList_;
}
QList<int> LoginUserSession::getUserLocationIDs()
{
	return userLocationIDs;
}

void LoginUserSession::setUserRoomIDs(QList<int> idList_)
{
	userRoomIDs.clear();
	userRoomIDs=idList_;
}
QList<int> LoginUserSession::getUserRoomIDs()
{
	return userRoomIDs;
}

void LoginUserSession::setUserRoomEventTypeIDs(QList<int> idList_)
{
	userRoomEventTypeIDs.clear();
	userRoomEventTypeIDs=idList_;
}
QList<int> LoginUserSession::getUserRoomEventTypeIDs()
{
	return userRoomEventTypeIDs;
}
//Setting the PCID  
//<param name="PCID_">Used as PCInfo PCID</param>
void LoginUserSession::setPCID(int PCID_) 
{
	pcID=PCID_;
}
//getting the PCID 
//<returns>returns a int value which defines PCID</returns>
int LoginUserSession::getPCID() 
{
	return pcID;
}
//Setting the PCNO 
//<param name="PCNo_">Used as PCInfo PCNo</param>
void LoginUserSession::setPCNo(QString PCNo_)  
{
	pcNo=PCNo_;
}//getting the PCNO
//<returns>returns a int value which defines PCNo</returns>
QString LoginUserSession::getPCNo()   
{
	return pcNo;
}
//Setting the PCName
//<param name="PCName_">Used as PCInfo PCName</param>
void LoginUserSession::setPCName(QString PCName_)   
{
	pcName=PCName_;
}
//getting the PCName
//<returns>returns a String value which defines PCName</returns>
QString LoginUserSession::getPCName()   
{
	return pcName;
}
//Setting the RoomID 
//<param name="roomID_">Used as PCInfo roomID</param>
void LoginUserSession::setPCRoomID(int roomID_)  
{
	pcRoomID=roomID_;
}
//getting the RoomID 
//<returns>returns a int value which defines roomID_</returns>
int LoginUserSession::getPCRoomID()  
{
	return pcRoomID;
}
//Setting the Created Date
//<param name="createdDate_">Used as PCInfo createdDate</param>
void LoginUserSession::setPCCreatedDate(QDateTime createdDate_)   
{
	pcCreatedDate=createdDate_;
}
//getting the Created Date
//<returns>returns a Date which defines createdDate</returns>
QDateTime LoginUserSession::getPCCreatedDate() 
{
	return pcCreatedDate;
}
//Setting the LocationID  
//<param name="locationID_">Used as RoomInfo locationID</param>
void LoginUserSession::setCurrentLocation(int locationID_) 
{
	locationID=locationID_;
}
//getting the LocationID
//<returns>returns a int value which defines locationID</returns>
int LoginUserSession::getCurrentLocation()   
{
	return locationID;
}
//Setting the RoomID 
//<param name="roomID_">Used as RoomInfo roomID</param>
void LoginUserSession::setCurrentRoom(int roomID_) 
{
	roomID=roomID_;
}
//getting the RoomID 
//<returns>returns a int value which defines roomID</returns>
int LoginUserSession::getCurrentRoom()  
{
	return roomID;
}
//Setting the event type Name 
//<param name="eventTypeName_">Used as event type details eventTypeName</param>
void LoginUserSession::setCurrentEventType(int eventTypeID_) 
{
	eventTypeID=eventTypeID_;
}
//getting the event type ID 
//<returns>returns a int value which defines EventTypeID</returns>
int LoginUserSession::getCurrentEventType() 
{
	return eventTypeID;
}

void LoginUserSession::setEventID(int id_)
{
	eventID=id_;
}
int LoginUserSession::getEventID()
{
	return eventID;
}
void LoginUserSession::setEventNo(QString no_)
{
	eventNo=no_;
}
QString LoginUserSession::getEventNo()
{
	return eventNo;
}
void LoginUserSession::setEventName(QString name_)
{
	eventName=name_;
}
QString LoginUserSession::getEventName()
{
	return eventName;
}
void LoginUserSession::setEventDescription(QString description_)
{
	eventDescription=description_;
}
QString LoginUserSession::getEventDescription()
{
	return eventDescription;
}
void LoginUserSession::setRetrievedID(int id_)
{
	retrievedID=id_;
}
int LoginUserSession::getRetrievedID()
{
	return retrievedID;
}
void LoginUserSession::setRetrievedLocation(QString location_)
{
	retrievedLocation=location_;
}
QString LoginUserSession::getRetrievedLocation()
{
	return retrievedLocation;
}
void LoginUserSession::setRetrievedRoom(QString room_)
{
	retrievedRoom=room_;
}
QString LoginUserSession::getRetrievedRoom()
{
	return retrievedRoom;
}
void LoginUserSession::setSessionID(int id_)
{
	sessionID=id_;
}
int LoginUserSession::getSessionID()
{
	return sessionID;
}
void LoginUserSession::setCurrentFilePath(QStringList filePath_)
{
	fileName=filePath_;
}
QStringList LoginUserSession::getCurrentFilePath()
{
	return fileName;
}
void LoginUserSession::setEventHomePath(QString path_)
{
	eventHomePath=path_;
}
QString LoginUserSession::getEventHomePath()
{
	return eventHomePath;
}
void LoginUserSession::setShortcutList(QStringList wordList_)
{
	shortcutWords=wordList_;
}
QStringList LoginUserSession::getShortcutList()
{
	return shortcutWords;
}
void LoginUserSession::setSharedPath(QString path_)
{
	uploadPath=path_;
}
QString LoginUserSession::getSharedPath()
{
	return uploadPath;
}
void LoginUserSession::setServerIP(QString ip_)
{
	serverIP=ip_;
}
QString LoginUserSession::getServerIP()
{
	return serverIP;
}

void LoginUserSession::setServerAdminName(QString admin_)
{
	serverAdminName=admin_;
}
QString LoginUserSession::getServerAdminName()
{
	return serverAdminName;
}

void LoginUserSession::setServerAdminPassword(QString password_)
{
	serverAdminPassword=password_;
}
QString LoginUserSession::getServerAdminPassword()
{
	return serverAdminPassword;
}
void LoginUserSession::setLanguageIndex(int index_)
{
	langIndex=index_;
}
int LoginUserSession::getLanguageIndex()
{
	return langIndex;
}
void LoginUserSession::setDriverName(QString driverName_)
{
	driverName=driverName_;
}
QString LoginUserSession::getDriverName()
{
	return driverName;
}
void LoginUserSession::setEventNoteID(int id_)
{
	eventNoteid=id_;
}
int LoginUserSession::getEventNoteID()
{
	return eventNoteid;
}
//destructor
LoginUserSession::~LoginUserSession()
{

}
