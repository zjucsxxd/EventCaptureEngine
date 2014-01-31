#ifndef ABOUTCRS_H
#define ABOUTCRS_H

#include <QDialog>
#include "ui_aboutcrs.h"

class AboutCRS : public QDialog
{
	Q_OBJECT

public:
	AboutCRS(QWidget *parent = 0);
	~AboutCRS();

private:
	Ui::AboutCRSClass ui;

	QString getBuildID();
};

#endif // ABOUTCRS_H
