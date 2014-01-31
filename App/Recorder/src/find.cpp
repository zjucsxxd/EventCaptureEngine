#include "find.h"
#include "eventeditor.h"
Find::Find(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.btnFind, SIGNAL(clicked()),this,SLOT(btnFind_Clicked()));
}

void Find::btnFind_Clicked()
{
	 findstr=ui.txtFind->text();
	 close();	
}

QString Find::findText()
{
	return findstr;
}
Find::~Find()
{

}
