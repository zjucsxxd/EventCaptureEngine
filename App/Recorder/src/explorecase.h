#ifndef EXPLORECASE_H
#define EXPLORECASE_H

#include <QDialog>
#include <QDateTime>
#include "ui_explorecase.h"

#include "eventutil.h"
#include "dlcrshared.h"
#include "loginusersession.h"

class ExploreCase : public QDialog
{
	Q_OBJECT

public:
	ExploreCase(QWidget *parent = 0);
	~ExploreCase();

	void loadUserDetails();

private:
	Ui::ExploreCaseClass ui;

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
	void btnMore_clicked();
	void btnStartEvent_clicked();
	void btnCloseEvent_clicked();
	void btnReopenEvent_clicked();
	void btnResumeEvent_clicked();
	void btnViewNotes_clicked();
	void validateDescription();
};

#endif // EXPLORECASE_H
