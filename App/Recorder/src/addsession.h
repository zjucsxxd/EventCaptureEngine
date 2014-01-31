#ifndef ADDSESSION_H
#define ADDSESSION_H

#include <QDialog>
#include "ui_addsession.h"

class AddSession : public QDialog
{
	Q_OBJECT

public:
	AddSession(QWidget *parent = 0);
	~AddSession();

private:
	Ui::AddSessionClass ui;
};

#endif // ADDSESSION_H
