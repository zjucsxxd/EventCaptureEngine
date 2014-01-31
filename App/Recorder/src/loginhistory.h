#ifndef LOGINHISTORY_H
#define LOGINHISTORY_H

#include <QDialog>
#include "ui_loginhistory.h"

#include <QDateTime>

class LoginHistory : public QDialog
{
	Q_OBJECT

public:

	LoginHistory(QWidget *parent = 0);
	~LoginHistory();


	void setUser(QString);
	void insertHistory(QDateTime,QString,QString);

private:

 	Ui::LoginHistoryClass ui;

};

#endif // LOGINHISTORY_H
