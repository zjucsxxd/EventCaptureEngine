#include "textedit.h"

#include <QKeyEvent>
#include <QDebug>
#include <QTextDocument>
#include <QClipboard>
#include <QApplication>
#define MAX_LENGTH 150

TextEdit::TextEdit(QWidget *parent)
	: SpellTextEdit(parent)
{
	logSession=LoginUserSession::session();
	
	connect (this, SIGNAL(currentCharFormatChanged(const QTextCharFormat &)),this, SLOT(currentCharFormatChanged(const QTextCharFormat &)));
	connect (this, SIGNAL(copyAvailable(bool)),this, SLOT(currentSelectionTextStatus(bool)));
}

void TextEdit::currentSelectionTextStatus(bool status_)
{
	selectionTxtStatus=status_;
}
void TextEdit::setBold(bool status_)
{
	QTextCharFormat format;
	format.setFontWeight(status_!=false ? QFont::Bold : QFont::Normal);
	mergeFormatOnWordOrSelection(format);
}
void TextEdit::setItalic(bool status_)
{
	QTextCharFormat format;
	(status_!=false)? format.setFontItalic(true) : format.setFontItalic(false);
	mergeFormatOnWordOrSelection(format);	
}
void TextEdit::setUnderline(bool status_)
{
	QTextCharFormat format;
	(status_!=false)? format.setFontUnderline(true) : format.setFontUnderline(false);
	mergeFormatOnWordOrSelection(format);
}

void TextEdit::setColorText()
{
	QTextCharFormat format_;
		
	QTextCursor cursor = textCursor();

	if (cursor.hasSelection())
	{
		format_.setForeground(Qt::red);
		cursor.select(QTextCursor::WordUnderCursor);
	}
	else
	{
		format_.setForeground(Qt::black);
	}
	cursor.mergeCharFormat(format_);
	mergeCurrentCharFormat(format_);		
}

QString TextEdit::loadSelectedText()
{
	QString selectStr;
	QTextCursor cursor = textCursor(); 
	if (!cursor.hasSelection())
	{
		cursor.select(QTextCursor::WordUnderCursor);
	}
	selectStr=cursor.selectedText();
	return selectStr;
}

// void TextEdit::setReplaceStr()
// {
// 		QString replace=loadSelectedText();
// 		emit Found(replace);
// }

// void TextEdit::sendReplaceStr(QString str)
// {
// 		QString replace=replaceSelectedText(str);
//		QTextCursor cursor(textCursor());
//		QString findstr=loadSelectedText();
// 		if (cursor.hasSelection())
// 		{
// 			cursor.select(QTextCursor::Document);
// 			cursor.movePosition(QTextCursor::Start); 
// 			QTextCharFormat plainFormat(cursor.charFormat());
// 			QTextCharFormat colorFormat = plainFormat;
// 			colorFormat.setForeground(Qt::red);
// 			QTextDocument *doc =document();
// 			QTextCursor newCursor(doc);
// 			while (!newCursor.isNull() && !newCursor.atEnd()) 
// 			{
// 				newCursor = doc->find(findstr,newCursor);
//
// 				if (!newCursor.isNull()) 
// 				{
// 					newCursor.movePosition(QTextCursor::EndOfWord,
// 					QTextCursor::KeepAnchor); 			
// 					newCursor.mergeCharFormat(colorFormat);
// 				}
// 				else
// 				{
// 					newCursor.movePosition(QTextCursor::EndOfWord,
// 						QTextCursor::KeepAnchor); 			
// 				}
// 			}
// 			cursor.clearSelection();
// 		}
// }

// QString TextEdit::replaceSelectedText(QString repl)
// {
//		QString selectStr;
// 		QTextCursor cursor = textCursor();
// 		if (!cursor.hasSelection())
//		{
//			cursor.select(QTextCursor::WordUnderCursor);
//		}
//		cursor.removeSelectedText();
// 		cursor.insertText(repl);
// 		return selectStr;
// }

void TextEdit::mergeFormatOnWordOrSelection(const QTextCharFormat &format_)
{
	QTextCursor cursor = textCursor();
	
	if (!cursor.hasSelection())
	{
		cursor.select(QTextCursor::WordUnderCursor);
	}
	cursor.mergeCharFormat(format_);
	mergeCurrentCharFormat(format_);
}

void TextEdit::getSelectionTxtStatus()
{
	QTextCursor cursor = textCursor();

	if (cursor.hasSelection())
	{
		emit cursorStatus(true);
	}
	else
	{
		emit cursorStatus(false);
	}
}

void TextEdit::getTxt()
{
	QString originalText=QApplication::clipboard()->text();

	if(originalText!="")
	{
		emit cursorStatus(true);
	}
	else
	{
		emit cursorStatus(false);
	}
}

void TextEdit::currentCharFormatChanged(const QTextCharFormat &format)
{
	fontChanged(format.font());
}
void TextEdit::fontChanged(const QFont &f)
{
	if(f.bold()!=true) emit currentBold(false);
	else emit currentBold(true);
	if(f.italic()!=true) emit currentItalic(false);
	else emit currentItalic(true);
	if(f.underline()!=true) emit currentUnderline(false);
	else emit currentUnderline(true);
}

QString TextEdit::filterText(QString str_)
{
 	if(this->toPlainText().length() + str_.length() > MAX_LENGTH)
 	{
 		return str_.left((this->toPlainText().length() + str_.length()) - MAX_LENGTH);
 	}
 	else
 	{
 		return str_;
 	}
}

void TextEdit::insertText(QString text_)
{
 	int a= this->textCursor().anchor();
 	setText(this->toPlainText().left(a)  + filterText(text_) + this->toPlainText().right(this->toPlainText().length()-a));
 	this->textCursor().setPosition(this->textCursor().position()+text_.length());
} 

void TextEdit::keyPressEvent(QKeyEvent *e_)
{
	QStringList shortcutWords=logSession->getShortcutList();

	if (e_->key()== Qt::Key_Enter ||  e_->key()== Qt::Key_Return)
	{
		e_->ignore();
		return;
	}
	else
	{
		if(e_->modifiers () == (Qt::ControlModifier | Qt::AltModifier)) //check for the modifier of Alt + Control....
		{
			QTextCursor cursor = this->textCursor();

			switch (e_->key()) 
			{
				
				case Qt::Key_A: // Item at Position 0
 					insertText(filterText(shortcutWords[0]));
 					this->setTextCursor(cursor) ;
 					e_->ignore();
 					e_->setAccepted (true);
 					break;

				case Qt::Key_B: // Item at Position 1
					
 					insertText(filterText(shortcutWords[1]));
 					this->setTextCursor(cursor) ;
 					e_->ignore();
 					e_->setAccepted (true);
 					break;

				case Qt::Key_C: // Item at Position 2

 					insertText(filterText(shortcutWords[2]));
 					this->setTextCursor(cursor) ;
 					e_->ignore();
 					e_->setAccepted(true);
 					break;

				case Qt::Key_D: // Item at Position 3
					
 					insertText(filterText(shortcutWords[3]));
 					this->setTextCursor(cursor) ;
 					e_->ignore();
 					e_->setAccepted (true);
					break;

				case Qt::Key_E: // Item at Position 4
					
 					insertText(filterText(shortcutWords[4]));
 					this->setTextCursor(cursor) ;
 					e_->ignore();
 					e_->setAccepted (true);
					break;

				case Qt::Key_F: // Item at Position 5
					
 					insertText(filterText(shortcutWords[5]));
 					this->setTextCursor(cursor) ;
 					e_->ignore();
 					e_->setAccepted (true);
					break;

				case Qt::Key_G: // Item at Position 6
					
 					insertText(filterText(shortcutWords[6]));
 					this->setTextCursor(cursor) ;
 					e_->ignore();
 					e_->setAccepted (true);
					break;

				case Qt::Key_H: // Item at Position 7
					
 					insertText(filterText(shortcutWords[7]));
 					this->setTextCursor(cursor) ;
 					e_->ignore();
 					e_->setAccepted (true);
					break;

				case Qt::Key_I: // Item at Position 8
					
 					insertText(filterText(shortcutWords[8]));
 					this->setTextCursor(cursor) ;
 					e_->ignore();
 					e_->setAccepted (true);
					break;

				case Qt::Key_J: // Item at Position 9
					
 					insertText(filterText(shortcutWords[9]));
 					this->setTextCursor(cursor) ;
 					e_->ignore();
 					e_->setAccepted (true);
					break;

				case Qt::Key_K: // Item at Position 10
					
 					insertText(filterText(shortcutWords[10]));
 					this->setTextCursor(cursor) ;
 					e_->ignore();
 					e_->setAccepted (true);
					break;

				case Qt::Key_L: // Item at Position 11
					
 					insertText(filterText(shortcutWords[11]));
 					this->setTextCursor(cursor) ;
 					e_->ignore();
 					e_->setAccepted (true);
					break;

				case Qt::Key_M: // Item at Position 12
					
 					insertText(filterText(shortcutWords[12]));
 					this->setTextCursor(cursor) ;
 					e_->ignore();
 					e_->setAccepted (true);
					break;

				case Qt::Key_N: // Item at Position 13
					
 					insertText(filterText(shortcutWords[13]));
 					this->setTextCursor(cursor) ;
 					e_->ignore();
 					e_->setAccepted (true);
					break;

				case Qt::Key_O: // Item at Position 14
					
 					insertText(filterText(shortcutWords[14]));
 					this->setTextCursor(cursor) ;
 					e_->ignore();
 					e_->setAccepted (true);
					break;

				case Qt::Key_P: // Item at Position 15
					
 					insertText(filterText(shortcutWords[15]));
 					this->setTextCursor(cursor) ;
 					e_->ignore();
 					e_->setAccepted (true);
					break;

				case Qt::Key_Q: // Item at Position 16
					
 					insertText(filterText(shortcutWords[16]));
 					this->setTextCursor(cursor) ;
 					e_->ignore();
 					e_->setAccepted (true);
					break;

				case Qt::Key_R: // Item at Position 17
					
 					insertText(filterText(shortcutWords[17]));
 					this->setTextCursor(cursor) ;
 					e_->ignore();
 					e_->setAccepted (true);
					break;

				case Qt::Key_S: // Item at Position 18
					
 					insertText(filterText(shortcutWords[18]));
 					this->setTextCursor(cursor) ;
 					e_->ignore();
 					e_->setAccepted (true);
					break;

				case Qt::Key_T: // Item at Position 19
					
 					insertText(filterText(shortcutWords[19]));
 					this->setTextCursor(cursor) ;
 					e_->ignore();
 					e_->setAccepted (true);
					break;
			}
			return;
		}
		else
		{
			QTextEdit::keyPressEvent(e_);
		}		
	}

	if (e_->modifiers ()== Qt::ControlModifier)
	{
		switch (e_->key()) 
		{
			case Qt::Key_C:
				 getSelectionTxtStatus();
				 break;

			case Qt::Key_X:
				 getTxt();
				 break;
		}
		return;
	}

	if (e_->key()==Qt::Key_Space)
	{
		QTextCharFormat format_;
		
		QTextCursor cursor = textCursor();

		if (cursor.hasSelection())
		{
			cursor.select(QTextCursor::WordUnderCursor);
			cursor.movePosition(QTextCursor::EndOfWord,
			QTextCursor::KeepAnchor); 
			format_.setForeground(Qt::red);
		}
		else
		{
			format_.setForeground(Qt::black);
		}
		cursor.mergeCharFormat(format_);
		mergeCurrentCharFormat(format_);
	}
}
TextEdit::~TextEdit()
{

}
