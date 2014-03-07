/*Owner & Copyrights: Vance King Saxbe. A.*/#ifndef REPLACE_H
#define REPLACE_H

#include <QDialog>
#include "ui_replace.h"

class Replace : public QDialog
{
	Q_OBJECT

public:
	Replace(QWidget *parent = 0);
	~Replace();

	void loadSelectedText(QString);
	QString replaceTxt();
	QString searchTxt();
	private slots:
		void btnReplace_Clicked();
private:
	Ui::ReplaceClass ui;
	QString txtToReplace;
	QString searchStr;
};

#endif // REPLACE_H
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/