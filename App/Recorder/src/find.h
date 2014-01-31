#ifndef FIND_H
#define FIND_H

#include <QDialog>
#include "ui_find.h"

class Find : public QDialog
{
	Q_OBJECT

public:
	Find(QWidget *parent = 0);
	~Find();

	QString findText();

private slots:
	void btnFind_Clicked();

private:
	Ui::FindClass ui;
	QString findstr;
};

#endif // FIND_H
