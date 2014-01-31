#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QDialog>
#include "ui_dictionary.h"
#include "loginusersession.h"

class Dictionary : public QDialog
{
	Q_OBJECT

public:
	Dictionary(QWidget *parent = 0);
	~Dictionary();

	void loadDictionary(QString,QString,QString,QString);
	void readDictionary();

	private slots:
		void btnPlus_Clicked();
		void btnMinus_Clicked();
		void btnOK_Clicked();
		void tblDictionary_indexChange(int,int);

private:
	Ui::DictionaryClass ui;
	LoginUserSession *logSession;
	int DicRow;
};

#endif // DICTIONARY_H
