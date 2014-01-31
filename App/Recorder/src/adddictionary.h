#ifndef ADDDICTIONARY_H
#define ADDDICTIONARY_H

#include <QDialog>
#include "ui_adddictionary.h"

class AddDictionary : public QDialog
{
	Q_OBJECT

public:
	AddDictionary(QWidget *parent = 0);
	~AddDictionary();

	private slots:
		void cmbDictionary_Changed(int);
		void btnBrowse_Clicked();
		void btnOk_Clicked();

private:
	Ui::AddDictionaryClass ui;
};

#endif // ADDDICTIONARY_H
