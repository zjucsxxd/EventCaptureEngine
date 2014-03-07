/*Owner & Copyrights: Vance King Saxbe. A.*//* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "customdeligate.h"

#include <QKeyEvent>
// #include <QRegExpValidator>
// #include <QRegExp>
// #include <QLineEdit>

CustomDeligate::CustomDeligate(QObject *parent)
	: QStyledItemDelegate(parent)
{
	e=static_cast<EventEditor*>(parent);
	logSession=LoginUserSession::session();
	lan=new QLocale();

	dictionary1="en_GB.dic";
	dictionary2="ar_AE.dic";
}
void CustomDeligate::checkBold(bool status_)
{
	emit currentBold(status_);
}
void CustomDeligate::checkItalic(bool status_)
{
	emit currentItalic(status_);
}
void CustomDeligate::checkUnderline(bool status_)
{
	emit currentUnderline(status_);
}
void CustomDeligate::setBold(bool status_)
{
	emit makeBold(status_);
}
void CustomDeligate::setItalic(bool status_)
{
	emit makeItalic(status_);
}
void CustomDeligate::setUnderline(bool status_)
{
	emit makeUnderline(status_);
}

void CustomDeligate::sendSelection()
{
	emit selectionTxt();
}
void CustomDeligate::getselectionStatus(bool status)
{
	emit selectionStatus(status);
}

void CustomDeligate::setColorText()
{
	emit makeTxtColor();
}

// void CustomDeligate::getReplaceString(QString str)
// {
// 	emit FoundStr(str);
// }

// void CustomDeligate::toReplace(QString s)
// {
// 	emit ReturnReplace(s);
// }

// void CustomDeligate::toFind(QString s)
// {
// 	emit FindDelegate(s);
// }

QWidget *CustomDeligate::createEditor(QWidget *parent_, const QStyleOptionViewItem &option_,const QModelIndex &index_)const
{
	TextEdit *textBox = new TextEdit(parent_);
	
 	if(logSession->getLanguageIndex()==0)
 	{
 		textBox->setDict(dictionary1);
 	}
 	else
 	{
 		textBox->setDict(dictionary2);
	}
 	

// 	QLineEdit *edit = new QLineEdit(parent_);
// 	edit->setValidator(new QRegExpValidator(QRegExp("^[0-9]+$"),edit));

	connect(this   ,	SIGNAL(makeBold(bool))        ,	textBox, SLOT(setBold(bool)));
	connect(this   ,	SIGNAL(makeItalic(bool))      ,	textBox, SLOT(setItalic(bool)));
	connect(this   ,	SIGNAL(makeUnderline(bool))   ,	textBox, SLOT(setUnderline(bool)));

	connect(textBox,	SIGNAL(currentBold(bool))     ,	this   , SLOT(checkBold(bool)));
	connect(textBox,	SIGNAL(currentItalic(bool))   ,	this   , SLOT(checkItalic(bool)));
	connect(textBox,	SIGNAL(currentUnderline(bool)), this   , SLOT(checkUnderline(bool)));

	connect(this   ,	SIGNAL(selectionTxt())        ,	textBox, SLOT(getSelectionTxtStatus()));
	connect(textBox,	SIGNAL(cursorStatus(bool))    ,	this   , SLOT(getselectionStatus(bool)));

 	connect(this   ,	SIGNAL(makeTxtColor())        ,	textBox, SLOT(setColorText()));

// 	connect(textBox,	SIGNAL(Found(QString))        ,	this   , SLOT(getReplaceString(QString)));
// 	connect(this   ,	SIGNAL(ReturnReplace(QString)), textBox, SLOT(sendReplaceStr(QString)));
// 	connect(this   ,	SIGNAL(FindDelegate(QString)) ,	textBox, SLOT(sendDelegateStr(QString)));

	return textBox;
}

void CustomDeligate::paint(QPainter *painter, const QStyleOptionViewItem &option,
							  const QModelIndex &index) const
{
	painter->save();

	QString val = index.model()->data(index, Qt::EditRole).toString();
	QTextDocument d;
	d.setHtml(val);	

	if (option.state & QStyle::State_Selected){
		QBrush brush=option.palette.highlight();
		brush.setColor(Qt::lightGray);
		painter->fillRect(option.rect,brush );
	}
	else
	{
		QBrush brush=option.palette.highlight();
		if(val.left(20)=="<!--searched text-->") brush.setColor(QColor(173,216,230));
		else brush.setColor(Qt::white);	
		painter->fillRect(option.rect,brush );
	}	

	painter->translate(option.rect.topLeft());

	d.drawContents(painter);

	painter->translate(-option.rect.topLeft());

	painter->restore();

}
bool CustomDeligate::eventFilter(QObject *obj_, QEvent *event_)
{
	if (event_->type() == QEvent::KeyPress) 
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event_);

		if(keyEvent->key()==Qt::Key_Enter)
		{
			EventEditor *deligateparent = static_cast<EventEditor*>(obj_);
			commitData(deligateparent);
			closeEditor(deligateparent);
			e->keyPressEvent(keyEvent);
			return true;
		}
		else if(keyEvent->key()==Qt::Key_Return )
		{

			EventEditor *deligateparent = static_cast<EventEditor*>(obj_);
			commitData(deligateparent);
			closeEditor(deligateparent);
			e->keyPressEvent(keyEvent);
			return true;
		}
		else if(keyEvent->key()==Qt::Key_Tab)
		{
			EventEditor *deligateparent = static_cast<EventEditor*>(obj_);	
			commitData(deligateparent);
			closeEditor(deligateparent);
			e->keyPressEvent(keyEvent);
			return true;
		}
		else
		{
			return false;
		}
	} 
	else 
	{
		return false;
	}
}
void CustomDeligate::closeEditor(QWidget *widget_)
{	
	QStyledItemDelegate::closeEditor(widget_);
	delete widget_;	
}
void CustomDeligate::setEditorData(QWidget *editor,const QModelIndex &index) const
{
	QString strVal = index.model()->data(index, Qt::EditRole).toString();

	TextEdit *textBox = static_cast<TextEdit*>(editor);
	textBox->setHtml(strVal);

	QTextCursor tc = textBox->textCursor();
	tc.movePosition(QTextCursor::Left);
	tc.movePosition(QTextCursor::EndOfLine);

	textBox->setTextCursor(tc);
}
CustomDeligate::~CustomDeligate()
{

}
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/