#ifndef CUSTOMDELIGATE_H
#define CUSTOMDELIGATE_H

#include <QObject>
#include <QPainter>
#include <QItemDelegate>
#include <QStyledItemDelegate>
#include <QLocale>

#include "eventeditor.h"
#include "loginusersession.h"
#include "textedit.h"

class CustomDeligate : public QStyledItemDelegate
{
	Q_OBJECT

public:

	CustomDeligate(QObject *parent);
	~CustomDeligate();

	void paint(QPainter *, const QStyleOptionViewItem &, const QModelIndex &) const;
	QWidget *createEditor(QWidget *, const QStyleOptionViewItem &, const QModelIndex &)const;
	void setEditorData(QWidget *editor, const QModelIndex &index)const ;
	bool eventFilter(QObject *, QEvent *);

signals:

	void makeBold(bool);
	void makeItalic(bool);
	void makeUnderline(bool);

	void currentBold(bool);
	void currentItalic(bool);
	void currentUnderline(bool);

	void selectionTxt();
	void selectionStatus(bool);
 	void makeTxtColor();

// 	void FoundStr(QString); 
// 	void ReturnReplace(QString);
// 	void FindDelegate(QString);

public slots:

	void setBold(bool);
	void setItalic(bool);
	void setUnderline(bool);
	void checkBold(bool);
	void checkItalic(bool);
	void checkUnderline(bool);

	void sendSelection();
	void getselectionStatus(bool);
	void setColorText();

// 	void setReplace();
// 	void getReplaceString(QString);
// 	void toReplace(QString);
// 	void toFind(QString);
	

private:

	EventEditor *e;
	LoginUserSession *logSession;
	QLocale *lan;
	QString dictionary1,dictionary2;
	void closeEditor(QWidget *);	
};

#endif // CUSTOMDELIGATE_H
