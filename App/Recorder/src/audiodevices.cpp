/*Owner & Copyrights: Vance King Saxbe. A.*//* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "audiodevices.h"
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
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/