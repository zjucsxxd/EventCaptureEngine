#include "replace.h"
#include "eventeditor.h"
#include "textedit.h"
Replace::Replace(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.btnReplace, SIGNAL(clicked()),this,SLOT(btnReplace_Clicked()));
}

void Replace::loadSelectedText(QString selString)
{
	ui.txtSelected->setText(selString);
	//exec();	
}
void Replace::btnReplace_Clicked()
{
	txtToReplace=ui.txtReplace->text();
	searchStr=ui.txtSelected->text();
	close();	

}
QString Replace::replaceTxt()
{
	return txtToReplace;
	
}
QString Replace::searchTxt()
{
	return searchStr;

}
Replace::~Replace()
{

}
