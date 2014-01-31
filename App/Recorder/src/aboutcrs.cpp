#include "aboutcrs.h"

#include <QDate>
#include <QTime>

AboutCRS::AboutCRS(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	ui.lblBuildID->setText(getBuildID());
}

QString AboutCRS::getBuildID()
{
	QString buildID="";

	QString Time=QString(__TIME__);
	QString Date=QString(__DATE__).replace("  ",":").replace(" ",":")
		.replace("Jan","1")
		.replace("Feb","2")
		.replace("Mar","3")
		.replace("Apr","4")
		.replace("May","5")
		.replace("Jun","6")
		.replace("Jul","7")
		.replace("Aug","8")
		.replace("Sep","9")
		.replace("Oct","10")
		.replace("Nov","11")
		.replace("Dec","12")
		.replace("\"","");

	QDate d = QDate::fromString(Date,"M:d:yyyy");
	QTime t = QTime::fromString(Time,"h:m:S");

	int year = d.year();
	int day = d.day();
	int month = d.month();
	int hour = t.hour();

	QString version=QString::number((year%100),16).toUpper()+"."+QString::number(month,16).toUpper()+"."+QString::number(day,16).toUpper()+"."+QString::number(hour);
	buildID="("+tr("Build ")+version+")";

	return buildID;
}

AboutCRS::~AboutCRS()
{

}
