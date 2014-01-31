#ifndef IMAGEDETAILS_H
#define IMAGEDETAILS_H

#include <QDialog>
#include "ui_imagedetails.h"
#include "referenceimage.h"
#include "loginusersession.h"
#include "sqlitedatabase.h"
#include <QToolTip>
class ImageDetails : public QDialog
{
	Q_OBJECT

public:
	ImageDetails(QWidget *parent = 0);
	~ImageDetails();

	void readFile();
	void setTab(int);
	void loadFileDetails(QString,QString,QString);
	void setActiveRowTime(QString);

	private slots:
		void btnPlus_Clicked();
		void btnMinus_Clicked();
		void btnOK_Clicked();
		void tblImages_indexChange(int,int);


private:
	Ui::ImageDetailsClass ui;
	LoginUserSession *logSession;
	SQLiteDataBase *tempDB;
	int tabIndx;
	int remRow;
	QString activeRowTime;
	QToolTip *toolTip;
};

#endif // IMAGEDETAILS_H
