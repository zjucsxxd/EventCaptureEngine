#include "actionmenu.h"

ActionMenu::ActionMenu(QObject *parent)
	: QAction(parent)
{

	QAction *action=new QAction(this);
	connect(action, SIGNAL(triggered()), this, SLOT(actionMenu_Clicked()));
}

void ActionMenu::actionMenu_Clicked()
{
	emit Action(this);
}
ActionMenu::~ActionMenu()
{

}
