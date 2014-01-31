#ifndef LANGUAGEMENU_H
#define LANGUAGEMENU_H

#include <QAction>

class LanguageMenu : public QAction
{
	Q_OBJECT

public:
	LanguageMenu(QWidget *paren);
	~LanguageMenu();

	private slots:
		void actionLanguage_clicked(bool);
signals:

		void LAction(LanguageMenu*);
private:

	QAction *language;


	
};

#endif // LANGUAGEMENU_H
