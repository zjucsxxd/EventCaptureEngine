/*Owner & Copyrights: Vance King Saxbe. A.*/#ifndef EDITSCHEDULERS_H
#define EDITSCHEDULERS_H

#include <QDialog>
#include "ui_editschedulers.h"
#include "loginusersession.h"
#include "scheduler.h"
#include "eventutil.h"

class EditSchedulers : public QDialog
{
	Q_OBJECT

public:
	EditSchedulers(QWidget *parent = 0);
	~EditSchedulers();
	void loadEventTypes();
	bool isAssigned(int,int,int);
	void loadLocations();
	void loadAllEvents();
	void listEvents();
	void loadUserDetails();
	void setEditSchduleID(int);

	private slots:
		void cmbAction_indexChanged(int);
		void cmbRepeat_indexChanged(int);
		void cmbEventNo_indexChanged(int);
		void cmbLocationName_indexChanged(int);
		void cmbRoomName_indexChanged(int);
		void btnOk_Clicked();
private:
	Ui::EditSchedulersClass ui;
	LoginUserSession *logSession;
	QList<EventInfo> allEvents;
	int scheduleID;
};

#endif // EDITSCHEDULERS_H
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/