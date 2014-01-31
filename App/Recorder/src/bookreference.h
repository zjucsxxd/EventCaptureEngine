#ifndef BOOKREFERENCE_H
#define BOOKREFERENCE_H

#include <QDialog>
#include "ui_bookreference.h"
#include "loginusersession.h"

class BookReference : public QDialog
{
	Q_OBJECT

public:
	BookReference(QWidget *parent = 0);
	~BookReference();
	void setTabIndex(int);
	void setRowTime(QString);
	void completer();

	private slots:
	void btnAdd_Clicked();

private:
	Ui::BookReferenceClass ui;
	LoginUserSession *logSession;
	int tabIndex;
	QString currentRowTime;
};

#endif // BOOKREFERENCE_H
