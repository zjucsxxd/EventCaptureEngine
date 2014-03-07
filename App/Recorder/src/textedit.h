/*Owner & Copyrights: Vance King Saxbe. A.*/#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QObject>
#include "SpellTextEdit.h"
#include "loginusersession.h"

class TextEdit : public SpellTextEdit
{
	Q_OBJECT

public:
	TextEdit(QWidget *parent);
	~TextEdit();

 	QString loadSelectedText();
// 	QString replaceSelectedText(QString);

signals:

	void currentBold(bool);
	void currentItalic(bool);
	void currentUnderline(bool);

	void cursorStatus(bool);

// 	void Found(QString);

public slots:
	void setBold(bool);
	void setItalic(bool);
	void setUnderline(bool);

	void getSelectionTxtStatus();
	void getTxt();
	void setColorText();

// 	void sendReplaceStr(QString);
// 	void sendDelegateStr(QString);
	
protected:

	void keyPressEvent(QKeyEvent *);

private:

	void fontChanged(const QFont &);	
	void mergeFormatOnWordOrSelection(const QTextCharFormat &);
	QString filterText(QString);
	void insertText(QString);
	LoginUserSession *logSession;
	bool selectionTxtStatus;

private slots:

	void currentCharFormatChanged(const QTextCharFormat &);
	void currentSelectionTextStatus(bool);

};

#endif // TEXTEDIT_H
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/