/*Owner & Copyrights: Vance King Saxbe. A.*/#ifndef LANGUAGES_H
#define LANGUAGES_H

#include <QDialog>
#include "ui_languages.h"
#include "loginusersession.h"
class Languages : public QDialog
{
	Q_OBJECT

public:
	Languages(QWidget *parent = 0);
	~Languages();

	void loadLanguages(QString,QString,QString,QString);
	void readLanguages();

	private slots:
		void btnPlus_Clicked();
		void btnMinus_Clicked();
		void btnOK_Clicked();
		void tbllanguage_indexChange(int,int);
private:
	Ui::LanguagesClass ui;
	LoginUserSession *logSession;
	int languageRow;
};

#endif // LANGUAGES_H
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/