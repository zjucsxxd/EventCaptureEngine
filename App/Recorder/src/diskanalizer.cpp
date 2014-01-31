#include "diskanalizer.h"

#include <windows.h>
#include <lmcons.h>
#include <shellapi.h>
#include <QDir>

#include "nightcharts.h"

DiskAnalizer::DiskAnalizer(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	QString homePath =QDir::homePath();
	drive=homePath.left(3);

	ui.lblDrive->setText(drive);
 	
	double bytesTotal = getTotalSpace(drive);
	ui.lblBytesTotal->setText(QString::number(bytesTotal,'f',0));
	double gbTotal=(bytesTotal/1048576)/1024;
	ui.lblGBTotal->setText(QString::number(gbTotal,'f',2));

	this->setFixedSize(this->width(),this->height());

	ui.toolButton->setEnabled(false);
	ui.toolButton_2->setEnabled(false);
}

float DiskAnalizer::getPercentageUsed()
{
	double bytesUsed = getUsedSpace(drive);
	double bytesTotal = getTotalSpace(drive);
	float p =((bytesUsed*100)/bytesTotal);
	return p;
}

void DiskAnalizer::paintEvent(QPaintEvent *e)
{
 	QWidget::paintEvent(e);
 	QPainter painter;

 	double bytesFree = getFreeSpace(drive);
	ui.lblBytesFree->setText(QString::number(bytesFree,'f',0));
	double gbFree=(bytesFree/1048576)/1024;
	ui.lblGBFree->setText(QString::number(gbFree,'f',2));
 	double bytesUsed = getUsedSpace(drive);
	ui.lblBytesUsed->setText(QString::number(bytesUsed,'f',0));
	double gbUsed=(bytesUsed/1048576)/1024;
	ui.lblGBUsed->setText(QString::number(gbUsed,'f',2));
	double bytesTotal = getTotalSpace(drive);
 	
	float p =((bytesFree*100)/bytesTotal);

  	painter.begin(this);
  	Nightcharts PieChart;
  	PieChart.setType(Nightcharts::Pie);//{Histogramm,Pie,Dpie};
  	PieChart.setLegendType(Nightcharts::Vertical);//{Round,Vertical}
  	PieChart.setCords(ui.widget->geometry().x(),ui.widget->geometry().y(),ui.widget->width(),ui.widget->height());
  
  	PieChart.addPiece("Free Space",Qt::green,p);
  	PieChart.addPiece("Used Space",QColor(200,10,50),100);
  
  	PieChart.draw(&painter);
}

double DiskAnalizer::getFreeSpace(QString drive_)
{	
	QByteArray ba = drive_.toLatin1();
	char *c_str2 = ba.data(); 
	LPCSTR DirecName =c_str2;
	ULARGE_INTEGER free,total;
	bool bRes = ::GetDiskFreeSpaceExA(DirecName, &free , &total , NULL );

	if ( !bRes ) 
	{
		return 0;
	}
	double fFree = static_cast<double>( static_cast<__int64>(free.QuadPart) ) ;

	return fFree;
}

double DiskAnalizer::getTotalSpace(QString drive_)
{
	QByteArray ba = drive_.toLatin1();
	char *c_str2 = ba.data(); 
	LPCSTR DirecName =c_str2;
	ULARGE_INTEGER free,total;
	bool bRes = ::GetDiskFreeSpaceExA(DirecName, &free , &total , NULL );

	if ( !bRes ) 
	{
		return 0;
	}
	double fTotal = static_cast<double>( static_cast<__int64>(total.QuadPart) ) ;

	return fTotal;
}

double DiskAnalizer::getUsedSpace(QString drive_)
{
	QByteArray ba = drive_.toLatin1();
	char *c_str2 = ba.data(); 
	LPCSTR DirecName =c_str2;
	ULARGE_INTEGER free,total;
	bool bRes = ::GetDiskFreeSpaceExA(DirecName, &free , &total , NULL );

	if ( !bRes ) 
	{
		return 0;
	}
	double fFree = static_cast<double>( static_cast<__int64>(free.QuadPart) ) ;
	double fTotal = static_cast<double>( static_cast<__int64>(total.QuadPart) ) ;
	double FUsed=fTotal-fFree;

	return FUsed;
}

DiskAnalizer::~DiskAnalizer()
{

}
