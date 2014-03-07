/*Owner & Copyrights: Vance King Saxbe. A.*/#ifndef WIZARD_H
#define WIZARD_H

#include <QDialog>
#include <QProcess>

#include "ui_wizard.h"
#include "dbserverconnection.h"

class Wizard : public QDialog
{
	Q_OBJECT

public:

	Wizard(QWidget *parent = 0);
	~Wizard();

	void resizeEvent (QResizeEvent *);
	void refershWizard();
	void writeAppSettings();
	void setDriverName(QString);
	void loadPCRegDetails();
	void writePCDetails();
	void writeVideoSettings();
	void writeAudioSettings();
	void readPCDetails();
	void readAppSettings();
	void readAudioSettings();
	void readVideoSettings();
	void loadVideoDevices();
	void loadAudioDevices();
	void getAudioDevices(QString);
	void getAudioType(QString);
	void getvideoDevices(QString);
	void getResolution(QString);
	bool validateFields();
	void readSettings();
	void writeSettings();
	void setChannelCount(int);
	int getChannelCount();
	void checkDBConnection();

private:

	Ui::WizardClass ui;
	QProcess *process;
	QSqlDatabase db;
	QString driverName;
	QString VideoDeviceName;
	QString Resolution;
	QString AudioDeviceName;
	QString AudioType;
	int videoColumn;
	int audioColumn;
	int channelsCount;
	private slots:

		void btnRegisterPC_click();
		void btnVideoPlus_click();
		void btnVideoMinus_click();
		void btnAudioPlus_click();
		void btnAudioMinus_click();
		void btnConnect_click();
		void btnOk_click();
		void btnRestore_click();
		void cmbLocationName_indexChange(int);
		void videotable_indexChange(int row_,int column_);
		void audiotable_indexChange(int row_,int column_);
		void wizardTabChanged(int);
		void text_PCChanged();
		void cmbRoomName_indexChange(int);


};

#endif // WIZARD_H
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/