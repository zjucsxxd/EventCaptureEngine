/*Owner & Copyrights: Vance King Saxbe. A.*/#ifndef REFERENCEFILE_H
#define REFERENCEFILE_H

#include <QDialog>
#include "ui_referencefile.h"

class ReferenceFile : public QDialog
{
	Q_OBJECT

public:
	ReferenceFile(QWidget *parent = 0);
	~ReferenceFile();

	void setTabIndex(int);
	void setCurrentTime(QString);

	private slots:
		void btnOk_Clicked();
		void btnBrowse_Clicked();

private:
	Ui::ReferenceFileClass ui;
	int tabIndex;
	QString currentRowTime;
};

#endif // REFERENCEFILE_H
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/