/*Owner & Copyrights: Vance King Saxbe. A.*/#ifndef ADVANCEDSEARCH_H
#define ADVANCEDSEARCH_H

#include <QDialog>
#include "ui_advancedsearch.h"

#include <QStringList>
#include <QDateTime>
#include <QIcon>
#include "dbserverconnection.h"
#include "loginusersession.h"
#include "dlcrshared.h"

class AdvancedSearch : public QDialog
{
	Q_OBJECT

public:
	AdvancedSearch(QWidget *parent = 0);
	~AdvancedSearch();
	void  searchByCatagory(QString ,int,QDateTime,QDateTime,QString,QString,QString,QString,QString,QString,int,QString);

private:
	QIcon icoOpen,icoClosed;
	LoginUserSession *logSession;
	DLCRShared *sharedObj;

	bool isAssigned(int,int,int);//check wheather location is assigned

	Ui::AdvancedSearchClass ui;	
	
private slots:
	void btnNewsearch_click();
	void btnSearch_click();
	void tblCellClicked(int,int);
	void tblCellChanged(int,int);
	void cmbo_click(int);
	void btnCloseEvent_clicked();
	void btnReopenEvent_clicked();
	void btnResumeEvent_clicked();
	void btnViewNotes_clicked();
};

#endif // ADVANCEDSEARCH_H
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/