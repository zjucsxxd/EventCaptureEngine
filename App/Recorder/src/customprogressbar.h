/*Owner & Copyrights: Vance King Saxbe. A.*/#ifndef CUSTOMPROGRESSBAR_H
#define CUSTOMPROGRESSBAR_H

#include <QDialog>
#include "ui_customprogressbar.h"

class CopyFileProgress : public QDialog
{
	Q_OBJECT

public:
	CopyFileProgress(QWidget *parent = 0);
	~CopyFileProgress();

	bool copyFile(QString,QString);
	bool copyTranslationFile(QString,QString);

private:
	Ui::CustomProgressBarClass ui;
};

#endif // CUSTOMPROGRESSBAR_H
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/