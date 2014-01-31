#ifndef IMPORTNOTES_H
#define IMPORTNOTES_H

#include <QDialog>
#include "ui_importnotes.h"
#include <QDateTime>
#include "eventutil.h"
#include "dlcrshared.h"
#include "loginusersession.h"
class ImportNotes : public QDialog
{
	Q_OBJECT

public:
	ImportNotes(QWidget *parent = 0);
	~ImportNotes();
	
	void setTabIndex(int);

private:
	Ui::ImportNotesClass ui;
	DLCRShared *sharedObj;
	LoginUserSession *logSession;

	QIcon icoDown,icoTop,icoOpen,icoClosed;
	QList<EventInfo> allEvents;
	int tabIndex;

	void loadLocations();
	void loadAllEvents();
	void loadRescentEvents();
	void listEvents(QString);
	void loadDetails();
	

	private slots:
		void cmbLocationName_indexChanged(int);
		void cmbRoomName_indexChanged(int);
		void cmbSearchCategory_indexChanged(int);
		void txtSearch_textChanged(QString);
		void eventDetails_cellClicked(int,int);
		void btnImportNotes_clicked();
		
};

#endif // IMPORTNOTES_H
