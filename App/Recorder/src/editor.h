/*Owner & Copyrights: Vance King Saxbe. A.*/#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QTableWidget>
#include <QIcon>
#include "eventnoteutil.h"
#include "dlcrshared.h"
#include "loginusersession.h"
#include "sqlitedatabase.h"

class Editor : public QTableWidget
{
	Q_OBJECT

public:
	Editor(QWidget *parent);
	~Editor();
	void deactivate();
	void insertRow(int);
	void activate(int);
	void updateRow(int);
	void clear();
	void insertImportRows(QList<EventNoteInfo> ,bool);
	void updateToDatabase(QList<EventNoteInfo>,QString,bool);

private:
	QString dayTimeNow();

	LoginUserSession *logSession;
	SQLiteDataBase *tempDB;
	DLCRShared *sharedObj;
	QIcon fixedIco,currentIco;

private slots:
	void tblCellChanged();

};

#endif // EDITOR_H
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/