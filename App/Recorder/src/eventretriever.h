#ifndef EVENTRETRIEVER_H
#define EVENTRETRIEVER_H

#include <QWidget>
#include "ui_eventretriever.h"
#include "eventnoteinfo.h"

#include "dlcrshared.h"
#include "loginusersession.h"

struct retrieveDetails
{
	QString no;
	QString title;
	QString description;
	QDateTime dt;
	QString eventType;
};

class EventRetriever : public QWidget
{
	Q_OBJECT

public:
	EventRetriever(QWidget *parent = 0);
	~EventRetriever();
	
	Ui::EventRetrieverClass ui;

	void insertAfterRow(int);
	void insertBeforeRow(int);
	int getRetrieveRow();
	QString getCurrentRetrieveTime(int);
	void createEditor();
	void insertRows(QList<EventNoteInfo>);
	void setEditable(bool);
	void setEventDetails(retrieveDetails);	
	void attachFile(QString,QString,QString,QString);
	void attachLink(QString,QString,QString);
	void attachBook(QString,QString,QString,QString,QString);
	QList<QTableWidgetSelectionRange> getSelectedColumn() const;
	void actionBold_Status(bool);
	void actionUnderLine_Status(bool);
	void actionItalics_Status(bool);
	void searchTxt(QString);
	void searchTxtReplace(QString,QString);
	void insertImportRows(QList<EventNoteInfo>);
	void deleteFromRow(int);
	void keyPressEvent(QKeyEvent *);	
	void actionText_Highlight_Clicked();

signals:

	void FindBold(bool);
	void FindItalics(bool);
	void FindUnderLine(bool);
	void chkRBStatus(bool);
	void chkRIStatus(bool);
	void chkRUStatus(bool);

	void retrieveCellClicked();
	void retrieveCellDoubleClicked();
	void retreiveRowPress();
	void textHighlightColor();
	
public slots:
	void exportPdf();
	void exportWordDoc();
	void printPreview();
	void printDocument();
	void checkBold(bool);
	void checkItalic(bool);
	void checkUnderline(bool);
	void btnPlus_Clicked();
	void btnMinus_Clicked();
	void getSelection_Status(bool);

protected:
	void contextMenuEvent( QContextMenuEvent *);

private:
	DLCRShared *sharedObj;
	LoginUserSession *logSession;

	QAction *exportToPdf;
	QAction *exportToWordDoc;
	QAction *exportToHtml;
	QAction *exportToCsv;

	int cRow;
	QList<QTableWidgetSelectionRange> selectedColumn;
	int selectedRowsCount;
	bool exists(QString,int);
	bool copyStringStatus;

	QString convertToHtml(QList<QStringList>);	
	QString convertToWordHtml(QList<QStringList>);

	void createPdfFile(QString,QString);
	void createWordDocument(QString,QString);
	void createHtmlFile(QString,QString);
	void createCsvFile(QString,QString);

private slots:
	void updateAllNotes();
	void eventRetriever_cellChanged(int,int);
	void eventRetriver_cellClicked(int,int);
	void exportToPdf_click();
	void exportToWordDoc_click();
	void exportToHtml_click();
	void exportToCsv_click();
	void print(QPrinter *);
	void enableSearchGo();
	void closeSearch();
	void search();
	void eventRetriver_cellDoubleClicked(int,int);
		
};

#endif // EVENTRETRIEVER_H
