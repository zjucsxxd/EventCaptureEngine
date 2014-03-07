/*Owner & Copyrights: Vance King Saxbe. A.*/#ifndef LINKDETAILS_H
#define LINKDETAILS_H

#include <QDialog>
#include "ui_linkdetails.h"
#include "referencelink.h"
#include "loginusersession.h"
#include "sqlitedatabase.h"

class LinkDetails : public QDialog
{
	Q_OBJECT

public:
	LinkDetails(QWidget *parent = 0);
	~LinkDetails();

	void readFile();
	void setTab(int);
	void setRowTime(QString);
	void loadLinkDetails(QString,QString);

	private slots:
		void btnPlus_Clicked();
		void btnMinus_Clicked();
		void btnOK_Clicked();
		void tblURL_indexChange(int,int);

private:
	Ui::LinkDetailsClass ui;
	LoginUserSession *logSession;
	SQLiteDataBase *tempDB;
	int tabIndx;
	int remRow,Row;
	QString Rowtime;
};

#endif // LINKDETAILS_H
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/