#ifndef BOOKDETAILS_H
#define BOOKDETAILS_H

#include <QDialog>
#include "ui_bookdetails.h"
#include "bookreference.h"
#include "loginusersession.h"
#include "sqlitedatabase.h"

class BookDetails : public QDialog
{
	Q_OBJECT

public:
	BookDetails(QWidget *parent = 0);
	~BookDetails();

	void readFile();
	void setTab(int);
	void setRowTime(QString);
	void loadBookDetails(QString,QString,QString,QString);

	private slots:
		void btnPlus_Clicked();
		void btnMinus_Clicked();
		void btnOK_Clicked();
		void tblbook_indexChange(int,int);


private:
	Ui::BookDetailsClass ui;
	LoginUserSession *logSession;
	SQLiteDataBase *tempDB;
	int tabIndx;
	int remRow;
	QString Rowtime;
};

#endif // BOOKDETAILS_H
