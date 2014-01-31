#ifndef ADDSCHEDULER_H
#define ADDSCHEDULER_H

#include <QDialog>
#include "ui_addscheduler.h"
#include "loginusersession.h"
#include "scheduler.h"
#include "eventutil.h"

class AddScheduler : public QDialog
{
	Q_OBJECT

public:
	AddScheduler(QWidget *parent = 0);
	~AddScheduler();

	void loadEventTypes();
	bool isAssigned(int,int,int);
	void loadLocations();
	void loadAllEvents();
	void listEvents();
	void loadUserDetails();


	private slots:
		void cmbAction_indexChanged(int);
		void cmbRepeat_indexChanged(int);
		void cmbEventNo_indexChanged(int);
		void cmbLocationName_indexChanged(int);
		void cmbRoomName_indexChanged(int);
		void btnOk_Clicked();
	
private:
	Ui::AddSchedulerClass ui;
	LoginUserSession *logSession;
	QList<EventInfo> allEvents;
};

#endif // ADDSCHEDULER_H
