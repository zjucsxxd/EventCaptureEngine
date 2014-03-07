/*Owner & Copyrights: Vance King Saxbe. A.*/#ifndef AUDIODEVICES_H
#define AUDIODEVICES_H

#include <QDialog>
#include "ui_audiodevices.h"
#include "wizard.h"

class AudioDevices : public QDialog
{
	Q_OBJECT

public:
	AudioDevices(QWidget *parent = 0);
	~AudioDevices();

	private slots:
		void btn_Ok();
private:

	Ui::AudioDevicesClass ui;
	void loadDevices();
};

#endif // AUDIODEVICES_H
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/