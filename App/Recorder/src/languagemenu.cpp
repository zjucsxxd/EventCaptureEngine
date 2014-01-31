#include "languagemenu.h"

LanguageMenu::LanguageMenu(QWidget *parent)
	: QAction(parent)
{
	connect(this, SIGNAL(triggered(bool)),this,SLOT(actionLanguage_clicked(bool)));
}
void LanguageMenu::actionLanguage_clicked(bool staus)
{
	emit LAction(this);
}
LanguageMenu::~LanguageMenu()
{

}
