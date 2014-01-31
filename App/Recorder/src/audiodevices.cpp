#include "audiodevices.h"
#include "deviceloader.h"

AudioDevices::AudioDevices(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.btnCancel,	SIGNAL(clicked()), this,SLOT(close()));
	connect(ui.btnOk,	SIGNAL(clicked()), this,SLOT(btn_Ok()));
	loadDevices();
}

void AudioDevices::loadDevices()
{
	DeviceLoader *availableDevs=new DeviceLoader();

	QStringList devList;

	try
	{
		devList=availableDevs->getAudioDevices();
	}
	catch(QString ex_)
	{
		printf("\nLoading available videoDevList failed with exception : %s",ex_.toAscii().data());
	}
	catch(...)
	{
		printf("\nUnknown error while loading available videoDevList");
	}

	delete availableDevs;

	ui.cmbDevices->addItems(devList);
}
void AudioDevices::btn_Ok()
{
	Wizard *e=static_cast<Wizard*>(this->parent());
	e->getAudioDevices(ui.cmbDevices->currentText());
	e->getAudioType(ui.cmbAudioTypes->currentText());
	e->loadAudioDevices();
}


AudioDevices::~AudioDevices()
{

}
