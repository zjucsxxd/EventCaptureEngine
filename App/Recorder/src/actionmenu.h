#ifndef ACTIONMENU_H
#define ACTIONMENU_H

#include <QAction>

class ActionMenu : public QAction
{
	Q_OBJECT

public:
	ActionMenu(QObject *parent);
	~ActionMenu();
	private slots:
		void actionMenu_Clicked();
signals:

	void Action(ActionMenu*);

private:
	
};

#endif // ACTIONMENU_H
