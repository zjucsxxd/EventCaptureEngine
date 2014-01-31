#ifndef UISPELLCHECKER_H
#define UISPELLCHECKER_H

#include <QDialog>
#include "ui_spellchecker.h"
#include "loginusersession.h"

class SpellChecker : public QDialog
{
	Q_OBJECT

public:
	SpellChecker(QWidget *parent = 0);
	~SpellChecker();
	
	void setText(QString);

	Ui::SpellCheckerClass ui;

private:
	LoginUserSession *logSession;

private slots:
	void btnApply_clicked();
	
};

#endif // UISPELLCHECKER_H
