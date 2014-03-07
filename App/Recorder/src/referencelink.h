/*Owner & Copyrights: Vance King Saxbe. A.*/#ifndef REFERENCELINK_H
#define REFERENCELINK_H

#include <QDialog>
#include "ui_referencelink.h"

class ReferenceLink : public QDialog
{
	Q_OBJECT

public:
	ReferenceLink(QWidget *parent = 0);
	~ReferenceLink();
	void setTabIndex(int);
	void setRowTime(QString);

	private slots:
		void btnOk_Clicked();
		void btnTest_Clicked();
private:
	Ui::ReferenceLinkClass ui;
	int tabIndex,currentRow;
	QString currentRowTime;
};

#endif // REFERENCELINK_H
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/