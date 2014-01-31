#ifndef DISKANALIZER_H
#define DISKANALIZER_H

#include <QDialog>
#include "ui_diskanalizer.h"

class DiskAnalizer : public QDialog
{
	Q_OBJECT

public:
	DiskAnalizer(QWidget *parent = 0);
	~DiskAnalizer();

	float getPercentageUsed();

	double getFreeSpace(QString);
	double getTotalSpace(QString);
	double getUsedSpace(QString);

protected:
	void paintEvent(QPaintEvent *e);

private:
	Ui::DiskAnalizerClass ui;
	QString drive;
};

#endif // DISKANALIZER_H
