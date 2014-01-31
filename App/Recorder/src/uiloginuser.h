#ifndef UILOGINUSER_H
#define UILOGINUSER_H

#include <QtGui/QDialog>
#include <QTranslator>

#include "ui_loginuser.h"
#include "dbserverconnection.h"
#include "loginusersession.h"

//<author>Jojin</author>
//<date>22/4/2011</date>
//<purpose>creating Evidencer login module</purpose>

class LoginUser : public QDialog
{
	Q_OBJECT

public:

	LoginUser(QWidget *parent = 0, Qt::WFlags flags = 0);
	~LoginUser();
	
	bool checkDBConnection();	
	bool isValidUser();
	int languageIndex();

private:

	Ui::LoginUserClass ui;

	QIcon icoDown;
	QIcon icoTop;

	bool loginStatus;

	QTranslator *translator; 
	LoginUserSession *logSession;

	void refreshDialog();
	void loadLocations();
	void setupCompleter();
	void readSettings();
	void readLastLogin();
	void writeSettings();
	bool connectDB();

	bool checkPCRegistration();

	void updateDomainPassword(QString,QString);
	
	void setupRecorder();
	
	void loadUserPermissions(QString);
	void loadUserLocations();
	void loadPCLocation();	

private slots:

	void btnLogin_click();	
	void btnCancel_click();	
	void btnMore_click();
	void cmbLocation_indexChange(int);
	//void cmbLanguages_indexChange(int);

	void readSavedPassword();
	void clearUserPswrdEdit(QString);

};

#endif // UILOGINUSER_H
//<history></history>