/*Owner & Copyrights: Vance King Saxbe. A.*/#ifndef FILESDETAILS_H
#define FILESDETAILS_H

#include <QDialog>
#include "ui_filesdetails.h"
#include "referencefile.h"
#include "loginusersession.h"
#include "sqlitedatabase.h"

class FilesDetails : public QDialog
{
	Q_OBJECT

public:
	FilesDetails(QWidget *parent = 0);
	~FilesDetails();

	void readFile();
	void setTab(int);
	void loadFileDetails(QString,QString,QString);
	void setRowTime(QString);

	private slots:
		void btnPlus_Clicked();
		void btnMinus_Clicked();
		void btnOK_Clicked();
		void tblFiles_indexChange(int,int);

private:
	Ui::FilesDetailsClass ui;
	LoginUserSession *logSession;
	SQLiteDataBase *tempDB;
	int tabIndx;
	int remRow;
	QString RowTime;
};

#endif // FILESDETAILS_H
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/