#ifndef EXPLORESESSION_H
#define EXPLORESESSION_H

#include <QDialog>
#include <QDateTime>
#include "ui_exploresession.h"

#include "eventutil.h"
#include "dlcrshared.h"
#include "loginusersession.h"

class ExploreSession : public QDialog
{
	Q_OBJECT

public:
	ExploreSession(QWidget *parent = 0);
	~ExploreSession();
	void loadUserDetails();

private:
	Ui::ExploreSessionClass ui;
	DLCRShared *sharedObj;
	LoginUserSession *logSession;

	QIcon icoDown,icoTop,icoOpen,icoClosed;
	QList<EventInfo> allEvents;

	void loadLocations();
	void loadEventTypes();
	bool isAssigned(int,int,int);
	void loadAllEvents();
	void loadRescentEvents();
	void listEvents(QString);	

	private slots:
		void cmbLocationName_indexChanged(int);
		void cmbRoomName_indexChanged(int);
		void cmbSearchCategory_indexChanged(int);
		void txtSearch_textChanged(QString);
		void eventDetails_cellClicked(int,int);
		void tblEventDetails_cellChanged(int,int);
		void btnStartEvent_clicked();
		void btnCloseEvent_clicked();
		void btnReopenEvent_clicked();
		void btnResumeEvent_clicked();
		void btnViewNotes_clicked();
		void btnAutoFill_clicked();
		void validateDescription();
};

#endif // EXPLORESESSION_H
