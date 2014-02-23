/* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "actionmenu.h"

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
