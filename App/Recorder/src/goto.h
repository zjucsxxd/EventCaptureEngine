/*Owner & Copyrights: Vance King Saxbe. A.*/#ifndef GOTO_H
#define GOTO_H

#include <QDialog>
#include "ui_goto.h"

class Goto : public QDialog
{
	Q_OBJECT

public:
	Goto(QWidget *parent = 0);
	~Goto();

	void setTabIndex(int);

private slots:
	void btnGoto_Clicked();

private:
	Ui::GotoClass ui;
	int tabIndex;
};

#endif // GOTO_H
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/