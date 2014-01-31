#ifndef VIDEODEVICES_H
#define VIDEODEVICES_H

#include <QDialog>
#include "ui_videodevices.h"
#include "wizard.h"

class VideoDevices : public QDialog
{
	Q_OBJECT

public:
	VideoDevices(QWidget *parent = 0);
	~VideoDevices();

	private slots:
		void btn_ok();

private:

	Ui::VideoDevicesClass ui;
	void loadDevices();
};

#endif // VIDEODEVICES_H
