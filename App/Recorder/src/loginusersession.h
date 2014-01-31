#ifndef LOGINUSERSESSION_H
#define LOGINUSERSESSION_H

#include <QString>
#include <QStringList>
#include <QDateTime>

//<author>Jojin</author>
//<date>6/4/2011</date>
//<purpose>creating login user session class</purpose>

class LoginUserSession
{
public:
	//static method for getting single instance
	//<returns>returns static instance of the object</returns>
	static LoginUserSession* session()
	{
		static LoginUserSession* _instance = new LoginUserSession();
		return _instance;
	}

	void setDBServerName(QString);//setting data source name
	QString getDBServerName();//getting data source name

	void setDBName(QString);//setting data source name
	QString getDBName();//getting data source name

	void setDBAdminName(QString);//setting database administrator name
	QString getDBAdminName();//getting database administrator name

	void setDBAdminPassWord(QString);//setting database administrator password
	QString getDBAdminPassWord();//getting database administrator password

	void setFTPServer(QString);
	QString getFTPServer();

	void setFTPPort(int);
	int getFTPPort();

	void setFTPAdmin(QString);
	QString getFTPAdmin();

	void setFTPPassword(QString);
	QString getFTPPassword();

	void setDomainControllerName(QString);//setting database administrator password
	QString getDomainControllerName();//getting database administrator password

	void setServerDBStatus(bool);//setting a status which defines db is on remote server or not
	bool getServerDBStatus();//getting a status which defines db is on remote server or not

	void setUserID(int);//setting user id
	int getUserID();//getting user id

	void setUserName(QString);//setting user name
	QString getUserName();//getting user name

	void setUserType(int);//setting user type
	int getUserType();//getting user type

	void setUserCategoryID(int);//setting category id
	int getUserCategoryID();//getting category id

	void setUserCreatedDate(QDateTime);//setting created date
	QDateTime getUserCreatedDate();//getting created date

	void setStartEvent(bool);
	bool canStartEvent();

	void setResumeEvent(bool);
	bool canResumeEvent();

	void setCloseEvent(bool);
	bool canCloseEvent();

	void setReopenEvent(bool);
	bool canReopenEvent();

	void setViewClosedEvent(bool);
	bool canViewClosedEvent();

	void setRetrieveEvent(bool);
	bool canRetrieveEvent();

	void setEditRetrieved(bool);
	bool canEditRetrieved();

	void setExportToFile(bool);
	bool canExportToFile();

	void setPrintAccess(bool);
	bool canPrintAccess();

	void setAttachFile(bool);
	bool canAttachFile();

	void setDownloadFile(bool);
	bool canDownloadFile();

	void setUploadToFTP(bool);
	bool canUploadToFTP();

	void setWriteOpticalDisk(bool);
	bool canWriteOpticalDisk();

	void setWriteRemovableDisk(bool);
	bool canWriteRemovableDisk();

	void setUserLocationIDs(QList<int>);
	QList<int> getUserLocationIDs();

	void setUserRoomIDs(QList<int>);
	QList<int> getUserRoomIDs();

	void setUserRoomEventTypeIDs(QList<int>);
	QList<int> getUserRoomEventTypeIDs();

	int getPCID();//getting PCID from PC Details
	void setPCID(int);//Setting PCID for PC Details
	QString getPCNo();//getting PCNO from PC Details
	void setPCNo(QString);//Setting PCNO for PC Details
	QString getPCName();//getting PCName from PC Details
	void setPCName(QString);//Setting PCName for PC Details
	int getPCRoomID();//getting RoomID from PC Details
	void setPCRoomID(int);//Setting RoomID for PC Details
	QDateTime getPCCreatedDate();//getting Created Date from PC Details
	void setPCCreatedDate(QDateTime);//Setting Created Date for PC Details
	
	int getCurrentLocation();//getting LocationID from Room Details
	void setCurrentLocation(int);//Setting LocationID for Room Details
	int getCurrentRoom();//getting RoomID from Room Details
	void setCurrentRoom(int);//Setting RoomID for Room Details
	int getCurrentEventType();//getting eventtype ID from eventtype details
	void setCurrentEventType(int);//Setting eventtype ID for eventtype details

	void setEventID(int);
	int getEventID();
	void setEventNo(QString);
	QString getEventNo();
	void setEventName(QString);
	QString getEventName();
	void setEventDescription(QString);
	QString getEventDescription();

	void setRetrievedID(int);
	int getRetrievedID();
	void setRetrievedLocation(QString);
	QString getRetrievedLocation();
	void setRetrievedRoom(QString);
	QString getRetrievedRoom();

	void setSessionID(int);
	int getSessionID();

	void setCurrentFilePath(QStringList);
	QStringList getCurrentFilePath();

	void setEventHomePath(QString);
	QString getEventHomePath();

	void setShortcutList(QStringList);
	QStringList getShortcutList();

	void setSharedPath(QString);
	QString getSharedPath();

	void setServerIP(QString);
	QString getServerIP();

	void setServerAdminName(QString);
	QString getServerAdminName();

	void setServerAdminPassword(QString);
	QString getServerAdminPassword();

	void setLanguageIndex(int);
	int getLanguageIndex();

	void setDriverName(QString);
	QString getDriverName();

	void setEventNoteID(int);
	int getEventNoteID();

	~LoginUserSession();//destructor

private:

	LoginUserSession();//default constructor

	//Database connection settings
	QString dbServerName;
	QString dbName;
	QString dbAdminName;
	QString dbAdminPassWord;

	//ftp connection settings
	QString ftpServerName;
	int ftpPortNo;
	QString ftpUserName;
	QString ftpPassword;

	//domain controller
	QString domainControllerName;

	//server db status
	bool serverDBStatus;

	//login user details
	int userID;
	QString userName;
	int userType;
	int userCategoryID;
	QDateTime userCreatedDate;

	bool startEvent;
	bool resumeEvent;
	bool closeEvent;
	bool reopenEvent;
	bool viewClosedEvent;
	bool retrieveEvent;
	bool editRetrieved;
	bool exportToFile;
	bool printAccess;
	bool attachFile;
	bool downloadFile;
	bool uploadToFTP;
	bool writeOpticalDisk;
	bool writeRemovableDisk;

	//login user location details
	QList<int> userLocationIDs;
	QList<int> userRoomIDs;
	QList<int> userRoomEventTypeIDs;

	//pc details
	int pcID;
	QString pcNo;
	QString pcName;
	int pcRoomID;
	QDateTime pcCreatedDate;

	//Current Login details
	int locationID;
	int roomID;
	int eventTypeID;

	//Current event details
	int eventID;
	QString eventNo;
	QString eventName;
	QString eventDescription;
	int sessionID;
	QStringList fileName;

	int retrievedID;
	QString retrievedLocation;
	QString retrievedRoom;

	QString eventHomePath;

	QStringList shortcutWords;

	QString uploadPath;
	QString serverIP;
	QString serverAdminName;
	QString serverAdminPassword;

	int langIndex;
	int eventNoteid;

	QString driverName;
};

#endif // LOGINUSERSESSION_H
//<history></history>