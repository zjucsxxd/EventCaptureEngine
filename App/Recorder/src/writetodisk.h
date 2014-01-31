#ifndef WRITETODISK_H
#define WRITETODISK_H

#include <QDialog>
#include "ui_writetodisk.h"

#include "eventutil.h"
#include "dlcrshared.h"
#include "loginusersession.h"
#include "backupserverconnection.h"
#include "copyengine.h"
#include "diskanalizer.h"

#include <QCloseEvent>
#include <QProcess>

class WriteToDisk : public QDialog
{
	Q_OBJECT

public:
	WriteToDisk(QWidget *parent = 0);
	~WriteToDisk();

	void loadUserDetails();

private:
	Ui::WriteToDiskClass ui;

	DLCRShared *sharedObj;
	LoginUserSession *logSession;
	BackupServerConnection *srvr;
	CopyEngine *copyThread;
	DiskAnalizer *driveAnalizer;
	bool isRunning;

	int avalableSpace;
	int totalAdded;

	QString backupServer,serverAdminName,serverPassword;
	QString eventNo,eventTitle,eventDescription,eventType;
	QString location,room,createdDate;

	QIcon icoSession,icoOpen,icoClosed;
	QList<EventInfo> allEvents;

	QProcess burn;

	void loadLocations();
	bool isAssigned(int,int,int);
	void loadAllEvents();
	void loadRescentEvents();
	void listEvents(QString);
	QString getSessionString(QDateTime,QDateTime);
	void prepareBurning();
	void writeRemovableDisk();
	void readSrvrSettings();
	bool copyFiles(QStringList,QString);
	bool removeDirectory(QString);
	void copyPlayer(QString);
	QString convertToHtml(QList<QStringList>);
	void createPdfFile(QString,QString);
	QList<QStringList> readSessionNotes(int);
	void closeEvent(QCloseEvent *);
	void copyFolder(QString,QString);

private slots:
	void cmbLocationName_indexChanged(int);
	void cmbRoomName_indexChanged(int);
	void cmbSearchCategory_indexChanged(int);
	void txtSearch_textChanged(QString);
	void eventDetails_cellClicked(int,int);
	void tblEventDetails_cellChanged();
	void btnAddClicked();
	void btnRemoveClicked();
	void listSessionItemClicked();
	void copySessionListItemClicked();
	void cbxOpticalChanged(int);
	void cbxRemovableChanged(int);
	void btnBrowsClicked();
	void btnWriteClicked();
	void showAvailability(int);
};

#endif // WRITETODISK_H
