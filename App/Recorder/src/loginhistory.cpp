#include "loginhistory.h"

LoginHistory::LoginHistory(QWidget *parent)
: QDialog(parent, QFlag(Qt::WA_StaticContents |Qt::WA_DeleteOnClose))
{
	ui.setupUi(this);

	ui.tblHistory->setColumnWidth(0,150);
	ui.tblHistory->setColumnWidth(1,200);
}

void LoginHistory::setUser(QString user_)
{
	ui.lblUser->setText(user_);
}

void LoginHistory::insertHistory(QDateTime time_,QString caseNo_,QString process_)
{
	int row=ui.tblHistory->rowCount();

	QTableWidgetItem *itemTime=new QTableWidgetItem();
	itemTime->setForeground(Qt::blue);
	itemTime->setText(time_.toString("dd-MM-yyyy hh:mm:ss AP"));	
	QTableWidgetItem *itemCaseNo=new QTableWidgetItem();
/*	itemCaseNo->setForeground(Qt::blue);*/
	itemCaseNo->setText(caseNo_);	
	QTableWidgetItem *itemProcess=new QTableWidgetItem();
/*	itemProcess->setForeground(Qt::blue);*/
	itemProcess->setText(process_);	

	ui.tblHistory->insertRow(row);

	ui.tblHistory->setItem(row,0,itemTime);
	ui.tblHistory->setItem(row,1,itemCaseNo);
	ui.tblHistory->setItem(row,2,itemProcess);
}

LoginHistory::~LoginHistory()
{

}