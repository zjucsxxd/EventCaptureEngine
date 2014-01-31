#include "uispellchecker.h"

#include "SpellTextEdit.h"
#include "dlcrmain.h"

#include <QTableWidgetItem>

SpellTextEdit *editor;

SpellChecker::SpellChecker(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
	editor=new SpellTextEdit(ui.frameEditor);
	logSession=LoginUserSession::session();
	ui.gridLayout_2->addWidget(editor, 0, 0, 1, 1);

	connect(ui.btnApply, SIGNAL(clicked()), this,SLOT(btnApply_clicked()));
}

void SpellChecker::setText(QString str_)
{	
	editor->setText(str_);	
	if(logSession->getLanguageIndex()==0) editor->setDict("en_GB.dic");
	else editor->setDict("ar_AE.dic");
}
void SpellChecker::btnApply_clicked()
{
	DLCRMain *dlcrs=static_cast<DLCRMain*>(this->parent());
	
	if(dlcrs->ui.tabEvents->currentIndex()==0)
	{
		dlcrs->ui.activeEditor->ui.tblEventEditor->currentItem()->setText(editor->toHtml());
	}
	else
	{		
		dlcrs->ui.activeRetriever->ui.tblEventRetriever->currentItem()->setText(editor->toHtml());		
	}

	this->close();
}
SpellChecker::~SpellChecker()
{

}
