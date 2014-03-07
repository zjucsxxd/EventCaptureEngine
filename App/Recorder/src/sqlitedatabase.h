/*Owner & Copyrights: Vance King Saxbe. A.*/#ifndef SQLITEDATABASE_H
#define SQLITEDATABASE_H

#include <QObject>
#include <QDateTime>
#include <QSqlDatabase>
#include "loginusersession.h"
#include "dlcrshared.h"

struct tempDetails
{
	int eventid;
	int pcid; 
	int eventtypeid; 
	int sessionid; 
	int userid; 
	int roomid; 	
};

struct tempNotes
{
	QString eventTime;
	QString richEventSpeaker;
	QString richEventNotes;
	QString internalReference;
	QString externalReference;
	//QString filePath;
	int visibility;
};
struct tempReferenceFile
{
	QString attachedPath;
	QString attachedHash;
	QString attachedSize;
	QString attachedType;
	int  uploadStatus;
	QString currentTime;
};
struct tempReferenceBook
{
	QString bookname;
	QString author;
	QString bookdescription;
	QString bcurrentTime;
	QString edition;
};
struct tempReferenceLink
{
	QString rlink;
	QString rdescription;
	QString rcurrentTime;
};
class SQLiteDataBase : public QObject
{
	Q_OBJECT	
	
public:

	static SQLiteDataBase* dataBase()
	{
		static SQLiteDataBase* _instance = new SQLiteDataBase();
		return _instance;
	}
	~SQLiteDataBase();

	bool connectDatabase();
	bool checkDataExistence();
	bool createTempTables();
	bool startCase(tempDetails,tempNotes,QString,int);
	bool insertNotes(tempNotes,QString,int);
	bool insertReferenceFile(tempReferenceFile);
	bool insertReferenceBook(tempReferenceBook);
	bool insertReferenceLink(tempReferenceLink);
	bool updateNotes(tempNotes);
	bool commitData();
	bool commitAllData();		
	bool clearTempTables();
	bool deleteEventNotes(QString);
	bool deleteReferenceFile(QString,QString,QString);
	bool deleteBook(QString,QString,QString,QString);
	bool deleteLink(QString,QString);
	QStringList loadReferenceFile(QString,QString);
	QList<QStringList> loadReferenceFile();
	QList<QStringList> loadReferenceBook();
	QList<QStringList> loadReferenceLink();

private:
	SQLiteDataBase();
	LoginUserSession *logSession;
	QString PlainText(QString);
	bool detailsTableExistence();
	bool notesTableExistence();
	bool refFileTableExistence();
	bool refBookTableExistence();
	bool refLinkTableExistence();

};

#endif // SQLITEDATABASE_H
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/