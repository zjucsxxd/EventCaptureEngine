#ifndef EASYNOTES_H
#define EASYNOTES_H

#include <QDialog>
#include "ui_easynotes.h"

#include "dlcrshared.h"
#include "loginusersession.h"

class EasyNotes : public QDialog
{
	Q_OBJECT

public:
	EasyNotes(QWidget *parent = 0);
	~EasyNotes();
	
	void refreshDialog();
	void applyShortcuts();

private slots:
	void btnSave_click();
	void btnApply_click();
	void cmbBasedOn_indexChange(int);
	void cmbCaseList_indexChange(int);
	void tblCellChanged();

private:
	Ui::EasyNotesClass ui;
	LoginUserSession *logSession;

	void loadUserEvents();
	void loadEasyNotes(int,int);
};

#endif // EASYNOTES_H
