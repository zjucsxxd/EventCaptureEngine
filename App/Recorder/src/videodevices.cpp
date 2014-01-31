#include "videodevices.h"
#include "deviceloader.h"
#include "videodeviceutil.h"


VideoDevices::VideoDevices(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.btnCancel,	SIGNAL(clicked()), this,SLOT(close()));
	connect(ui.btnOk,		SIGNAL(clicked()), this,SLOT(btn_ok()));

	loadDevices();
}

void VideoDevices::loadDevices()
{
 	DeviceLoader *availableDevs=new DeviceLoader();
 	
 	QStringList devList;
 	
 	try
 	{
 		devList=availableDevs->getVideoDevices();
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

void VideoDevices::btn_ok()
{

	Wizard *e=static_cast<Wizard*>(this->parent());
	e->getvideoDevices(ui.cmbDevices->currentText());
	e->getResolution(ui.cmbResolutions->currentText());
	e->loadVideoDevices();
}

VideoDevices::~VideoDevices()
{

}
 