/*Owner & Copyrights: Vance King Saxbe. A.*/#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QDialog>
#include "ui_scheduler.h"
#include "loginusersession.h"

class Scheduler : public QDialog
{
	Q_OBJECT

public:
	Scheduler(QWidget *parent = 0);
	~Scheduler();

	void loadschedulersinfo(QString,QString,QString,QString,QString ,QString,QString);
	void readScheduler();

	private slots:
		void btnPlus_Clicked();
		void btnMinus_Clicked();
		void btnEdit_Clicked();
		void tblscheduler_indexChange(int,int);
private:
	Ui::SchedulerClass ui;
	int deleteRow;
	LoginUserSession *logSession;
};

#endif // SCHEDULER_H
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/