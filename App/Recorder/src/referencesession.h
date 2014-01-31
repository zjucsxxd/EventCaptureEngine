#ifndef REFERENCESESSION_H
#define REFERENCESESSION_H

#include <QDialog>
#include "ui_referencesession.h"

class ReferenceSession : public QDialog
{
	Q_OBJECT

public:
	ReferenceSession(QWidget *parent = 0);
	~ReferenceSession();

private:
	Ui::ReferenceSessionClass ui;
};

#endif // REFERENCESESSION_H
