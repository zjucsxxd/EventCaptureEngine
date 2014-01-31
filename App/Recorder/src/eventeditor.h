#ifndef EVENTEDITOR_H
#define EVENTEDITOR_H

#include <QWidget>
#include "ui_eventeditor.h"
#include <QLineEdit>

#include <QKeyEvent>
#include <QDateTime>
#include <QIcon>
#include "eventnoteutil.h"
#include "dlcrshared.h"
#include "loginusersession.h"
#include "sqlitedatabase.h"

struct caseDetails
{
	QString no;
	QString title;
	QString description;
	QDateTime dt;
	QString location;
	QString room;
	QString eventType;
};

class EventEditor : public QWidget
{
	Q_OBJECT

public:
	EventEditor(QWidget *parent = 0);
	~EventEditor();

	Ui::EventEditorClass ui;

	void createEditor();
	void insertRow(int);
	void insertAfterRow(int);
	void insertBeforeRow(int);

	void setCaseDetails(caseDetails);
	caseDetails getCaseDetails();

	void attachFile(QString,QString,QString,QString);
	void attachBook(QString,QString,QString,QString,QString);
	void attachLink(QString,QString,QString);

	void remove(int);
	void clear();

	int getActiveRow();
	int getCurrentRow();
	int getCurrentColumn();
	
	void searchTxt(QString);
	void searchTxtReplace(QString,QString);
	void setClipBoardText(QString);

	void keyPressEvent(QKeyEvent *);	

	void actionBold_Status(bool);
	void actionUnderLine_Status(bool);
	void actionItalics_Status(bool);
	
	void insertImportRows(QList<EventNoteInfo>,bool);
	void deleteNotes(int);

	QString TimeNow();
	QString getCurrentRowTime(int);
	void setRecorderTime(QString);
	QString getRecorderTime();

signals:

	void FindBold(bool);
	void FindItalics(bool);
	void FindUnderLine(bool);

	void chkBStatus(bool);
	void chkIStatus(bool);
	void chkUStatus(bool);

	void keyPress();
	void editorRowPress();
	void editorRowDoublePress();

	void findselection();
 	void textColor();

// 	void FindString();
//	void ToFindString(QString);

public slots:

	void checkBold(bool);
	void checkItalic(bool);
	void checkUnderline(bool);
	void gotReplace(QString);

	void getSelection_Status(bool);
	void action_Text_Color_selection();

//	void actionFindNotes_clicked();
//	void actionReplace_clicked();
	
private:
	
	LoginUserSession *logSession;
	SQLiteDataBase *tempDB;
	DLCRShared *sharedObj;

	int mode,ctRow,ctColumn,activeRow;
	QIcon fixedIco,currentIco;
	QString replaceStr,recorderTime,clipboardString;
	bool chkBold,chkItalics,chkUnderLine,copyStringStatus;

	bool exists(QString,int);
	void updateToDatabase(int);

	QLineEdit *edit;

	QString recordTime;
	int previousClickedRow,previousClickedColumn;

	
private slots:

	void updateAllNotes();
	void eventEditor_cellChanged(int,int);
	void eventEditor_cellClicked(int,int);
	void enableSearchGo();
	void closeSearch();
	void search();
	void btnPlus_Clicked();
	void btnMinus_Clicked();
	void eventEditor_cellDoubledClicked(int,int);
	void eventEditor_cellPressed(int,int);

}; 

#endif // EVENTEDITOR_H
