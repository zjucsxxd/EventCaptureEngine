#ifndef ADDLANGUAGE_H
#define ADDLANGUAGE_H

#include <QDialog>
#include "ui_addlanguage.h"
class AddLanguage : public QDialog
{
	Q_OBJECT

public:
	AddLanguage(QWidget *parent = 0);
	~AddLanguage();

	private slots:
		void cmbLanguage_Changed(int);
		void btnBrowse_Clicked();
		void btnOk_Clicked();
private:
	Ui::AddLanguageClass ui;
	
};

#endif // ADDLANGUAGE_H
