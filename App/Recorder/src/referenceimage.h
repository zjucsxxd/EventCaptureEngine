#ifndef REFERENCEIMAGE_H
#define REFERENCEIMAGE_H

#include <QDialog>
#include "ui_referenceimage.h"

class ReferenceImage : public QDialog
{
	Q_OBJECT

public:
	ReferenceImage(QWidget *parent = 0);
	~ReferenceImage();

	void setTabIndex(int);
	void setReferenceTime(QString);

	private slots:
		void btnOk_Clicked();
		void btnBrowse_Clicked();

private:
	Ui::ReferenceImageClass ui;
	int tabIndex;
	QString referenceTime;
};

#endif // REFERENCEIMAGE_H
