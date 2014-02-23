/* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "eventeditor.h"

#include <QDir>
#include <QFile>
#include <QTime>
#include <QTableWidgetItem>
#include <QClipboard>
#include <QFont>


#include "customdeligate.h"
#include "customprogressbar.h"
#include "backupserverconnection.h"
#include "attachedfileutil.h"

#define TIME_COUNT 0
#define SPEAKER_COUNT 1
#define NOTE_COUNT 2
#define INTERNAL 3
#define EXTERNAL 4


EventEditor::EventEditor(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	sharedObj=new DLCRShared();
	tempDB=SQLiteDataBase::dataBase();
	logSession=LoginUserSession::session();

	fixedIco=QIcon(QPixmap(":/Recorder/Resources/attach.png"));
	currentIco=QIcon(QPixmap(":/Recorder/Resources/edit-normal.png"));

	edit=NULL;
	previousClickedColumn=0;
	previousClickedRow=0;

	connect(ui.btnUpdate,		SIGNAL(clicked()),					 this,SLOT(updateAllNotes()));
	connect(ui.tblEventEditor,	SIGNAL(cellChanged(int,int)),		 this,SLOT(eventEditor_cellChanged(int,int)));
	connect(ui.tblEventEditor,	SIGNAL(cellClicked(int,int)),		 this,SLOT(eventEditor_cellClicked(int,int)));
	connect(ui.tblEventEditor,	SIGNAL(cellDoubleClicked (int,int)), this,SLOT(eventEditor_cellDoubledClicked(int,int)));
	connect(ui.txtSearch,		SIGNAL(textChanged(const QString &)),this,SLOT(enableSearchGo()));
	connect(ui.btnGo,			SIGNAL(clicked()),					 this,SLOT(search()));
	connect(ui.btnCloseSearch,	SIGNAL(clicked()),					 this,SLOT(closeSearch()));
	connect(ui.btnPlusRow,		SIGNAL(clicked()),					 this,SLOT(btnPlus_Clicked()));
	connect(ui.btnMinusRow,		SIGNAL(clicked()),					 this,SLOT(btnMinus_Clicked()));
	connect(ui.tblEventEditor,	SIGNAL(cellPressed(int,int)),		 this,SLOT(eventEditor_cellPressed(int,int)));

	ui.tblEventEditor->setColumnWidth(0,150);
	ui.tblEventEditor->setColumnWidth(1,150);
	ui.tblEventEditor->setColumnWidth(2,300);
	ui.tblEventEditor->setColumnWidth(3,100);

	//ui.btnPlusRow->setEnabled(false);
	ui.btnMinusRow->setEnabled(false);
	ui.btnBold->setEnabled(false);
	ui.btnItalic->setEnabled(false);
	ui.btnUnderline->setEnabled(false);
	QApplication::clipboard()->clear();
}
void EventEditor::eventEditor_cellPressed(int rw_,int col_)
{
// 	if (previousClickedColumn==INTERNAL)
// 	{
// 		if (edit==NULL)
// 		{
// 			return;
// 		}
// 		else
// 		{
// 			QString linetxt=edit->text();
// 			if (linetxt=="")
// 			{
// 				delete edit;
// 				edit=NULL;
// 				return;
// 			}
// 			QTableWidgetItem *ireference;
// 			ireference = new QTableWidgetItem();
// 			ireference->setText(linetxt);
// 			delete edit;
// 			edit=NULL;
// 			TextEdit *texteditor=new TextEdit(this);
// 			ui.tblEventEditor->setItem(previousClickedRow,INTERNAL,ireference);
// 			texteditor->setText(linetxt);
// 			ui.tblEventEditor->item(previousClickedRow,INTERNAL)->setText(texteditor->toHtml());
// 		}
// 	}
}
void EventEditor::btnPlus_Clicked()
{

// 	ui.tblEventEditor->insertRow(activeRow+1);
// 	ui.tblEventEditor->activate(activeRow+1);
// 	ui.btnPlusRow->setEnabled(false);

	if (ui.tblEventEditor->rowCount()==0)
	{
		ui.tblEventEditor->insertRow(0);
		ui.tblEventEditor->activate(0);
	}
	else
	{
		int row=ui.tblEventEditor->rowCount();
		ui.tblEventEditor->insertRow(row);
		ui.tblEventEditor->activate(row);
	}	
 	ui.btnMinusRow->setEnabled(false);
	emit editorRowPress();
}

void EventEditor::btnMinus_Clicked()
{
	deleteNotes(activeRow);
	ui.tblEventEditor->removeRow(activeRow);
	//ui.btnPlusRow->setEnabled(false);
	ui.btnMinusRow->setEnabled(false);
	emit editorRowPress();
}

void EventEditor::createEditor()
{	
	CustomDeligate *myDelegate=new CustomDeligate(this);
	ui.tblEventEditor->setItemDelegate(myDelegate);	
	
	connect(ui.btnBold,		SIGNAL(clicked(bool)),			myDelegate,SLOT(setBold(bool)));
	connect(ui.btnItalic,	SIGNAL(clicked(bool)),			myDelegate,SLOT(setItalic(bool)));
	connect(ui.btnUnderline,SIGNAL(clicked(bool)),			myDelegate,SLOT(setUnderline(bool)));

	connect(myDelegate,		SIGNAL(currentBold(bool)),		this, SLOT(checkBold(bool)));
	connect(myDelegate,		SIGNAL(currentItalic(bool)),	this, SLOT(checkItalic(bool)));
	connect(myDelegate,		SIGNAL(currentUnderline(bool)), this, SLOT(checkUnderline(bool)));

// 	connect(this,			SIGNAL(sendString(QString)),	myDelegate,SLOT(toReplace(QString)));
// 	connect(this,			SIGNAL(ToFindString(QString)),	myDelegate,SLOT(toFind(QString)));
// 	connect(this,			SIGNAL(FindString()),			myDelegate,SLOT(setReplace()));
// 	connect(myDelegate,		SIGNAL(FoundStr(QString)),		this,SLOT(gotReplace(QString)));

	connect(this,			SIGNAL(findselection()),		myDelegate,SLOT(sendSelection()));
	connect(myDelegate,		SIGNAL(selectionStatus(bool)),  this,SLOT(getSelection_Status(bool)));

	connect(this,			SIGNAL(FindBold(bool)),			myDelegate,SLOT(setBold(bool)));
	connect(this,			SIGNAL(FindItalics(bool)),		myDelegate,SLOT(setItalic(bool)));
	connect(this,			SIGNAL(FindUnderLine(bool)),	myDelegate,SLOT(setUnderline(bool)));

	connect(this,			SIGNAL(textColor()),			myDelegate,SLOT(setColorText()));
	
	ui.btnUpdate->setEnabled(false);	
	copyStringStatus=false;
}
void EventEditor::action_Text_Color_selection()
{
	emit textColor();
}
void EventEditor::getSelection_Status(bool staus_)
{
	copyStringStatus=staus_;
}
void EventEditor::actionBold_Status(bool stats)
{
	emit FindBold(stats);
}

void EventEditor::actionItalics_Status(bool stats)
{
	emit FindItalics(stats);
}

void EventEditor::actionUnderLine_Status(bool stats)
{
	emit FindUnderLine(stats);
}

void EventEditor::insertRow(int row_)
{
	int n=ui.tblEventEditor->rowCount();
	
	if(n==row_ || n==row_+1) 
	{
		ui.tblEventEditor->insertRow(n);
		ui.tblEventEditor->activate(n);
	}
	mode=0;
}

void EventEditor::setCaseDetails(caseDetails details_)
{
	ui.txtNo->setText(details_.no);
	ui.txtNo->setToolTip(details_.no);
	ui.txtTitle->setText(details_.title);
	ui.txtTitle->setToolTip(details_.title);
	ui.txtDescription->setText(details_.description);
	ui.txtDescription->setToolTip(details_.description);
	ui.txtDate->setText(details_.dt.toString("dd/MM/yyyy"));
	ui.txtDate->setToolTip(details_.dt.toString("dd/MM/yyyy"));
	ui.txtLocation->setText(details_.location);
	ui.txtLocation->setToolTip(details_.location);
	ui.txtRoom->setText(details_.room);
	ui.txtRoom->setToolTip(details_.room);
	ui.txtEventType->setText(details_.eventType);
	ui.txtEventType->setToolTip(details_.eventType);
}

caseDetails EventEditor::getCaseDetails()
{
	caseDetails details;
	details.no=ui.txtNo->text();
	details.title=ui.txtTitle->text();
	details.description=ui.txtDescription->text();
	details.dt=QDateTime::fromString(ui.txtDate->text(),"dd/MM/yyyy");
	details.location=ui.txtLocation->text();
	details.room=ui.txtRoom->text();
	details.eventType=ui.txtEventType->text();

	return details;
}

void EventEditor::attachFile(QString fileName_,QString Size_,QString Type_,QString timeStr)
{
	BackupServerConnection *conn=BackupServerConnection::connection();

 	conn->createConnection(logSession->getServerIP(),logSession->getServerAdminName(),logSession->getServerAdminPassword());
 	if (!conn->isOpen())
 	{
 		SHOW_MESSAGE(tr("Server Connection Failed!!!"),tr("Please create connection to attach files"),tr("Ok"));
 		return;
 	}

	//QString homePath=sharedObj->getEventHomePath(ui.txtLocation->text(),ui.txtRoom->text(),ui.txtEventType->text(),ui.txtNo->text());
	QString homePath=logSession->getEventHomePath();
	QStringList tmpList=fileName_.split("/");
	QString destinationPath=homePath+"/"+tmpList.last();
	destinationPath=sharedObj->filterToServerPath(destinationPath,logSession->getSharedPath());

	QString absolutePath=destinationPath.left(destinationPath.lastIndexOf("\\"));
	QDir path=QDir(absolutePath);
	if (!path.exists())
	{
		path.mkpath(absolutePath);
	}

	QFile src(fileName_);
	QFile dst(destinationPath);

	if(exists(destinationPath.split("\\").last(),logSession->getEventID()))
	{
		QMessageBox msgBox;
		msgBox.setText("<b>Same file already attached to the event.</b>");
		msgBox.setInformativeText("Do you want to replace your existed file?");
		msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Cancel);
		if(msgBox.exec()==QMessageBox::Cancel)
		{
			return;
		}
	}

	CopyFileProgress *progress=new CopyFileProgress(this);

	progress->copyFile(fileName_,destinationPath);

	delete progress;

	if(src.size()!=dst.size())
	{
		SHOW_MESSAGE(tr("Error"),tr("Attaching file failed."),tr("Ok"));
		return;
	}

	tempReferenceFile refFile;
	refFile.attachedPath=destinationPath;
	refFile.attachedHash="--";
	refFile.attachedSize=Size_;
	refFile.attachedType=Type_;
	refFile.uploadStatus=1;
	refFile.currentTime=timeStr;

	tempDB->insertReferenceFile(refFile);

}
bool EventEditor::exists(QString fileName_,int eventID_)
{
	bool status=false;

	AttachedFileUtil *util=new AttachedFileUtil(this);

	try
	{
		QList<AttachedFileInfo> allFiles=util->selectAllAttachedFiles(eventID_,logSession->getDriverName());
		foreach(AttachedFileInfo info,allFiles)
		{
			if (info.getAttachedPath().split("\\").last()==fileName_)
			{
				status=true;
				break;
			}
		}
	}
	catch(DBExeption e)//catching db exceptions
	{
		if(e==ExecutionFailed)
		{
			PRINT_ERROR("insertion failed");	
		}
		if(e==UnableToConnect)
		{
			PRINT_ERROR("Unable to connect to db");	
		}	
		if(e==EmptyResultSet)
		{
			PRINT_WARNING("Empty result set");
		}
	}
	catch(...)//catching unknown exceptions
	{
		PRINT_ERROR("UNKnown ERROR");
	}

	delete util;

	return status;
}
void EventEditor::clear()
{
	ui.txtNo->clear();
	ui.txtTitle->clear();
	ui.txtDescription->clear();
	ui.txtDate->clear();
	ui.txtLocation->clear();
	ui.txtRoom->clear();
	ui.txtEventType->clear();
	ui.tblEventEditor->clear();
}
void EventEditor::keyPressEvent(QKeyEvent *e_)
{
	int row=ui.tblEventEditor->currentRow();

	ctRow=ui.tblEventEditor->currentRow();
	QTableWidgetItem *ireference;
	

	//emit keyPress();
	if(ui.tblEventEditor->currentColumn()==SPEAKER_COUNT && (e_->key()==  Qt::Key_Enter ||  e_->key()== Qt::Key_Return ||  e_->key()== Qt::Key_Tab))
	{
		ui.tblEventEditor->setCurrentCell(row,NOTE_COUNT);
		ui.tblEventEditor->editItem(ui.tblEventEditor->currentItem());	

		QString plainString;
		plainString=sharedObj->plainText(QApplication::clipboard()->text());
		QApplication::clipboard()->setText(plainString);

		return;
	}
	if(ui.tblEventEditor->currentColumn()==NOTE_COUNT && (e_->key()==  Qt::Key_Enter ||  e_->key()== Qt::Key_Return ||  e_->key()== Qt::Key_Tab))
	{

		//QTableWidgetItem *ireference;
		recordTime=getRecorderTime();
		ireference = new QTableWidgetItem();
		ireference->setText(recorderTime);
		
		edit = new QLineEdit(this);
		edit->setValidator(new QRegExpValidator(QRegExp("^([0-9]{2}\:{1}\[0-9]{2}\:{1}\[0-9]{2})$"),edit));

		ui.tblEventEditor->setCellWidget(row,INTERNAL, edit);
		edit->setFont(QFont("Arial",11));
		edit->setText(recordTime);
		
		//ui.tblEventEditor->setItem(row,INTERNAL,ireference);
		ui.tblEventEditor->setCurrentCell(row,INTERNAL);
		ui.tblEventEditor->editItem(ui.tblEventEditor->currentItem());

		QString plainString;
		plainString=sharedObj->plainText(QApplication::clipboard()->text());
		QApplication::clipboard()->setText(plainString);

		return;
	}
	if(ui.tblEventEditor->currentColumn()==INTERNAL && (e_->key()==  Qt::Key_Enter ||  e_->key()== Qt::Key_Return ||  e_->key()== Qt::Key_Tab))
	{
		recordTime=edit->text();
		delete edit;
		edit=NULL;
		TextEdit *texteditor=new TextEdit(this);
		ui.tblEventEditor->setItem(row,INTERNAL,ireference);
		texteditor->setText(recordTime);
		ui.tblEventEditor->item(row,INTERNAL)->setText(texteditor->toHtml());
		delete texteditor;

		ui.tblEventEditor->setCurrentCell(row,EXTERNAL);
		ui.tblEventEditor->editItem(ui.tblEventEditor->currentItem());	

		mode=1;

		QString plainString;
		plainString=sharedObj->plainText(QApplication::clipboard()->text());
		QApplication::clipboard()->setText(plainString);

		return;
	}
	if (mode==0 && ui.tblEventEditor->currentColumn()==EXTERNAL)
	{
		mode=1;
	}

	if(mode==1 &&( e_->key()== Qt::Key_Enter ||  e_->key()== Qt::Key_Return ||  e_->key()== Qt::Key_Tab ))
	{
		if (row!=0)
		{
			QString tmpFormatedSpeaker=ui.tblEventEditor->item(row,SPEAKER_COUNT)->text();
			QTextDocument tmpDoc;
			tmpDoc.setHtml(tmpFormatedSpeaker);
			QString tmpSpeaker=tmpDoc.toPlainText();

			if (tmpSpeaker=="")
			{
				QTableWidgetItem *speaker  = new QTableWidgetItem();
				speaker->setText(ui.tblEventEditor->item(row-1,SPEAKER_COUNT)->text());
				ui.tblEventEditor->setItem(row,SPEAKER_COUNT,speaker);
			}
		}

		QString formatedSpeaker = ui.tblEventEditor->item(row,SPEAKER_COUNT)->text();
		QString formatedNote = ui.tblEventEditor->item(row,NOTE_COUNT)->text();
		QString formatedInternal = ui.tblEventEditor->item(row,INTERNAL)->text();
// 		QString formatedInternal;
// 		if (txteditList.count()>ctRow)
// 		{
// 			formatedInternal = txteditList.at(ctRow)->text();
// 		}
		
		QString formatedExternal = ui.tblEventEditor->item(row,EXTERNAL)->text();
		QTextDocument t;
		t.setHtml(formatedSpeaker);
		QString actualSpeaker = t.toPlainText();
		t.setHtml(formatedNote);
		QString actualNote =t.toPlainText();
		t.setHtml(formatedInternal);
		QString actualInternal =t.toPlainText();
		t.setHtml(formatedExternal);
		QString actualExternal = t.toPlainText();

		if(actualSpeaker.trimmed()!="" && actualNote.trimmed()!="" && actualInternal.trimmed()!="")
		{
			ui.tblEventEditor->updateRow(row);
			updateToDatabase(row);
			insertRow(row);
		}

		QString plainString;
		plainString=sharedObj->plainText(QApplication::clipboard()->text());
		QApplication::clipboard()->setText(plainString);
	}
	if (e_->modifiers ()== Qt::ControlModifier)
	{
		if (e_->key()==Qt::Key_X)
		{
			
			copyStringStatus=false;
			int keyrow=getActiveRow();
			int keycolumn=getCurrentColumn();
			if (keycolumn==0)
			{
				return;
			}
			QString copystr=ui.tblEventEditor->item(keyrow,keycolumn)->text();
			//QApplication::clipboard()->setText(copystr);
			QApplication::clipboard()->setText(ui.tblEventEditor->currentIndex().data().toString());
			ui.tblEventEditor->item(keyrow,keycolumn)->setText("");

			clipboardString=QApplication::clipboard()->text();	
		}
	}

	if (e_->modifiers ()== Qt::ControlModifier)
	{
		if (e_->key()==Qt::Key_C)
		{
			copyStringStatus=false;
			int keyrow=getActiveRow();
			int keycolumn=getCurrentColumn();

			if (keycolumn==0)
			{
				return;
			}

			QString copystr=ui.tblEventEditor->item(keyrow,keycolumn)->text();
			QApplication::clipboard()->setText(copystr);

			clipboardString=QApplication::clipboard()->text();
		}
	}

	if (e_->modifiers ()== Qt::ControlModifier)
	{
		if (e_->key()==Qt::Key_V)
		{
			int keyrow=getActiveRow();
			int keycolumn=getCurrentColumn();
			if (keycolumn==0)
			{
				return;
			}
			TextEdit *texteditor=new TextEdit(this);
			texteditor->setText(QApplication::clipboard()->text());
 			ui.tblEventEditor->item(keyrow,keycolumn)->setText(texteditor->toHtml());
		}
	}
}

void EventEditor::setClipBoardText(QString clipboardText_)
{
	clipboardString=clipboardText_;
}
void EventEditor::setRecorderTime(QString time_)
{
	recorderTime=time_;
}

QString EventEditor::getRecorderTime()
{
	return recorderTime;
}

QString EventEditor::TimeNow()
{
	QTime currentTime= QTime::currentTime();
	QString timeStamp="<font size=\"4\" color=\"red\">" + currentTime.toString("hh:mm:ss AP") + "</font>";
	return timeStamp;
}

void EventEditor::updateToDatabase(int row_)
{
	QString timeStr;
	timeStr=sharedObj->plainText(ui.tblEventEditor->item(row_,TIME_COUNT)->text());

	QString strDateTime=ui.txtDate->text()+" "+timeStr;

	tempNotes notes;
	notes.eventTime=strDateTime;
//	notes.filePath=sharedObj->filterToServerPath(logSession->getCurrentFilePath(),logSession->getSharedPath());
	notes.richEventSpeaker=ui.tblEventEditor->item(row_,SPEAKER_COUNT)->text();
	notes.richEventNotes=ui.tblEventEditor->item(row_,NOTE_COUNT)->text();
	notes.internalReference=ui.tblEventEditor->item(row_,INTERNAL)->text();
//	notes.internalReference=txteditList.at(row_)->text();
	notes.externalReference=ui.tblEventEditor->item(row_,EXTERNAL)->text();
	notes.visibility=1;
	QStringList toServerpath;
	toServerpath=sharedObj->filterToServerPath(logSession->getCurrentFilePath(),logSession->getSharedPath());

	for (int i=0;i<toServerpath.count();i++)
	{
		tempDB->insertNotes(notes,toServerpath[i],0);
	}		
}
void EventEditor::updateAllNotes()
{
 	for (int i=0;i<ui.tblEventEditor->rowCount();i++)
 	{
		QString timeStr;
		timeStr=sharedObj->plainText(ui.tblEventEditor->item(i,TIME_COUNT)->text());

		QString strDateTime=ui.txtDate->text()+" "+timeStr;

		tempNotes notes;
		notes.eventTime=strDateTime;	
		notes.richEventSpeaker=ui.tblEventEditor->item(i,SPEAKER_COUNT)->text();
		notes.richEventNotes=ui.tblEventEditor->item(i,NOTE_COUNT)->text();
		notes.internalReference=ui.tblEventEditor->item(i,INTERNAL)->text();
		//notes.internalReference=txteditList.at(i)->text();
		notes.externalReference=ui.tblEventEditor->item(i,EXTERNAL)->text();
		
		tempDB->updateNotes(notes);
 	}

	ui.btnUpdate->setEnabled(false);
}
void EventEditor::enableSearchGo()
{
	if (ui.txtSearch->text().trimmed()!="")
	{
		ui.btnGo->setEnabled(true);
	}
	else
	{
		closeSearch();
		ui.btnGo->setEnabled(false);
	}
}
void EventEditor::closeSearch()
{
	QList<QTableWidgetItem *> items=ui.tblEventEditor->findItems("<!--searched text-->",Qt::MatchContains);

	foreach(QTableWidgetItem *item,items)
	{
		QString tempStr=item->text();
		tempStr=tempStr.replace("<!--searched text-->","");
		item->setText(tempStr);
	}

	ui.txtSearch->blockSignals(true);
	ui.txtSearch->clear();
	ui.txtSearch->blockSignals(false);
	
}
void EventEditor::search()
{
	QString searchText=ui.txtSearch->text().trimmed();

	if(searchText!="")
	{ 
		for(int row=0;row<ui.tblEventEditor->rowCount();row++)
		{
			{
				QString defaultString = ui.tblEventEditor->item(row,SPEAKER_COUNT)->text();
				defaultString=defaultString.replace("<!--searched text-->","");

				QTextDocument t;
				t.setHtml(defaultString);
				QString actualString = t.toPlainText();

				if(actualString.trimmed().size()==0) continue;

				if(actualString.contains(searchText,Qt::CaseInsensitive))
				{
					QString tempStr="<!--searched text-->"+defaultString;
					ui.tblEventEditor->item(row,SPEAKER_COUNT)->setText(tempStr);
				}
				else
				{
					ui.tblEventEditor->item(row,SPEAKER_COUNT)->setText(defaultString);
				}
			}

			{
				QString defaultString = ui.tblEventEditor->item(row,NOTE_COUNT)->text();
				defaultString=defaultString.replace("<!--searched text-->","");

				QTextDocument t;
				t.setHtml(defaultString);
				QString actualString = t.toPlainText();

				if(actualString.trimmed().size()==0) continue;

				if(actualString.contains(searchText,Qt::CaseInsensitive))
				{
					QString tempStr="<!--searched text-->"+defaultString;
					ui.tblEventEditor->item(row,NOTE_COUNT)->setText(tempStr);
				}
				else
				{
					ui.tblEventEditor->item(row,NOTE_COUNT)->setText(defaultString);
				}
			}

			{
				QString defaultString = ui.tblEventEditor->item(row,INTERNAL)->text();
				//QString defaultString = txteditList.at(row)->text();;
				defaultString=defaultString.replace("<!--searched text-->","");

				QTextDocument t;
				t.setHtml(defaultString);
				QString actualString = t.toPlainText();

				if(actualString.trimmed().size()==0) continue;

				if(actualString.contains(searchText,Qt::CaseInsensitive))
				{
					QString tempStr="<!--searched text-->"+defaultString;
					ui.tblEventEditor->item(row,INTERNAL)->setText(tempStr);
					//txteditList.at(row)->setText(tempStr);
				}
				else
				{
					ui.tblEventEditor->item(row,INTERNAL)->setText(defaultString);
					//txteditList.at(row)->setText(defaultString);
				}
			} 

			{
				QString defaultString = ui.tblEventEditor->item(row,EXTERNAL)->text();
				defaultString=defaultString.replace("<!--searched text-->","");

				QTextDocument t;
				t.setHtml(defaultString);
				QString actualString = t.toPlainText();

				if(actualString.trimmed().size()==0) continue;

				if(actualString.contains(searchText,Qt::CaseInsensitive))
				{
					QString tempStr="<!--searched text-->"+defaultString;
					ui.tblEventEditor->item(row,EXTERNAL)->setText(tempStr);
				}
				else
				{
					ui.tblEventEditor->item(row,EXTERNAL)->setText(defaultString);
				}
			} 
		}
	}
}
void EventEditor::eventEditor_cellChanged(int row_,int column_)
{
	if(row_==-1) return;
	int lastRow=ui.tblEventEditor->rowCount()-1;
	if(row_!=lastRow) ui.btnUpdate->setEnabled(true);	

// 	if (previousClickedColumn==INTERNAL)
// 	{
// 	if (edit==NULL)
// 	{
// 		return;
// 	}
// 	else
// 	{
// 		QString linetxt=edit->text();
// 		if (linetxt=="")
// 		{
// 			delete edit;
// 			edit=NULL;
// 			return;
// 		}
// 		QTableWidgetItem *ireference;
// 		ireference = new QTableWidgetItem();
// 		ireference->setText(linetxt);
// 		delete edit;
// 		edit=NULL;
// 		TextEdit *texteditor=new TextEdit(this);
// 		ui.tblEventEditor->setItem(previousClickedRow,INTERNAL,ireference);
// 		texteditor->setText(linetxt);
// 		ui.tblEventEditor->item(previousClickedRow,INTERNAL)->setText(texteditor->toHtml());
// 	}
// 	}

}
void EventEditor::eventEditor_cellDoubledClicked(int row_,int column_)
{
	activeRow=row_;
	ctColumn=column_;

	ui.btnBold->setEnabled(true);
	ui.btnItalic->setEnabled(true);
	ui.btnUnderline->setEnabled(true);

	if (previousClickedColumn==INTERNAL)
	{
		if (edit==NULL)
		{

		}
		else
		{

			//QTableWidgetItem *ireference;
			//ireference = new QTableWidgetItem();
			QString editStr=edit->text();
			if (editStr=="")
			{
				delete edit;
				edit=NULL;
			}
			else
			{
				delete edit;
				edit=NULL;
				TextEdit *texteditor=new TextEdit(this);
				//ui.tblEventEditor->setItem(previousClickedRow,INTERNAL,ireference);
				texteditor->setText(editStr);
				ui.tblEventEditor->item(previousClickedRow,INTERNAL)->setText(texteditor->toHtml());
				delete texteditor;
			}

		}
	}
	if (ctColumn==INTERNAL)
	{
		QString str=sharedObj->plainText(ui.tblEventEditor->item(row_,INTERNAL)->text());
		if (str=="")
		{
			edit = new QLineEdit(this);
			/*edit1->setValidator(new QRegExpValidator(QRegExp("^([0-9]{2}\:{1}\[0-9]{2}\:{1}\[0-9]{2})$"),edit));*/
			edit->setReadOnly(true);
			ui.tblEventEditor->setCellWidget(activeRow,INTERNAL, edit);
		}
		else
		{
			edit = new QLineEdit(this);
			edit->setValidator(new QRegExpValidator(QRegExp("^([0-9]{2}\:{1}\[0-9]{2}\:{1}\[0-9]{2})$"),edit));
			edit->setFont(QFont("Arial",11));
			edit->setText(str);
			ui.tblEventEditor->setCellWidget(activeRow,INTERNAL, edit);	
		}
		
	}

	else
	{
		if (edit==NULL)
		{
			
		}
		else
		{

			//QTableWidgetItem *ireference;
			//ireference = new QTableWidgetItem();
			QString editStr=edit->text();
			if (editStr=="")
			{
				delete edit;
				edit=NULL;
			}
			else
			{
				delete edit;
				edit=NULL;
				TextEdit *texteditor=new TextEdit(this);
				//ui.tblEventEditor->setItem(previousClickedRow,INTERNAL,ireference);
				texteditor->setText(editStr);
				ui.tblEventEditor->item(previousClickedRow,INTERNAL)->setText(texteditor->toHtml());
				delete texteditor;
			}

		}
	}

	if (copyStringStatus)
	{

	}
	else
	{
		QApplication::clipboard()->clear();
// 		QString plainString;
// 		plainString=sharedObj->plainText(QApplication::clipboard()->text());
// 		QApplication::clipboard()->setText(plainString);
	}
	emit editorRowDoublePress();

	previousClickedRow=row_;
	previousClickedColumn=column_;
}
void EventEditor::eventEditor_cellClicked(int row_,int column_)
{
	ctRow=row_;
	activeRow=row_;
	ctColumn=column_;
	emit keyPress();
	
	ui.btnPlusRow->setEnabled(true);
	ui.btnMinusRow->setEnabled(true);
	ui.btnBold->setEnabled(false);
	ui.btnItalic->setEnabled(false);
	ui.btnUnderline->setEnabled(false);

// 	if (previousClickedColumn==INTERNAL)
// 	{
// 
// 		if (edit==NULL)
// 		{
// 
// 		}
// 		else
// 		{
// 
// 			QTableWidgetItem *ireference;
// 			ireference = new QTableWidgetItem();
// 			QString editStr=edit->text();
// 			if (editStr=="")
// 			{
// 				delete edit;
// 				edit=NULL;
// 			}
// 			else
// 			{
// 				delete edit;
// 				edit=NULL;
// 				TextEdit *texteditor=new TextEdit(this);
// 				ui.tblEventEditor->setItem(previousClickedRow,INTERNAL,ireference);
// 				texteditor->setText(editStr);
// 				ui.tblEventEditor->item(previousClickedRow,INTERNAL)->setText(texteditor->toHtml());
// 			}
// 
// 		}
// 	}

// 	for(int i=0;i<ui.tblEventEditor->rowCount();i++)
// 	{
// 		ui.tblEventEditor->verticalHeaderItem(i)->setIcon(fixedIco);
// 	}
// 	ui.tblEventEditor->verticalHeaderItem(row_)->setIcon(currentIco);
	
}
void EventEditor::checkBold(bool status_)
{
	ui.btnBold->setChecked(status_);
	emit chkBStatus(status_);
}
void EventEditor::checkItalic(bool status_)
{
	ui.btnItalic->setChecked(status_);
	emit chkIStatus(status_);
}
void EventEditor::checkUnderline(bool status_)
{
	ui.btnUnderline->setChecked(status_);
	emit chkUStatus(status_);
}

void EventEditor::remove(int removeindex)
{
	ui.tblEventEditor->removeRow(removeindex);
}
int EventEditor::getActiveRow()
{
	return activeRow;
}
int EventEditor::getCurrentRow()
{
	return ctRow;
}
int EventEditor::getCurrentColumn()
{
	return ctColumn;
}
void EventEditor::insertAfterRow(int nrow)
{
	ui.tblEventEditor->insertRow(nrow+1);
	ui.tblEventEditor->activate(nrow+1);
}
void EventEditor::insertBeforeRow(int nrow)
{
	ui.tblEventEditor->insertRow(nrow);
	ui.tblEventEditor->activate(nrow);	
}
void EventEditor::attachBook(QString bName,QString bAuthor,QString bDescription,QString timeStr,QString Edtion_)
{
	tempReferenceBook refBook;
	refBook.bookname=bName;
	refBook.author=bAuthor;
	refBook.bookdescription=bDescription;
	refBook.bcurrentTime=timeStr;
	refBook.edition=Edtion_;

	tempDB->insertReferenceBook(refBook);
}

void EventEditor::attachLink(QString link,QString Description,QString timeStr)
{
	tempReferenceLink reflink;
	reflink.rlink=link;
	reflink.rdescription=Description;
	reflink.rcurrentTime=timeStr;

	tempDB->insertReferenceLink(reflink);
}

QString EventEditor::getCurrentRowTime(int trow)
{
	QString timeStr;
	timeStr=sharedObj->plainText(ui.tblEventEditor->item(trow,TIME_COUNT)->text());
	QString strDateTime=ui.txtDate->text()+" "+timeStr;

	return strDateTime;
}

// void EventEditor::actionReplace_clicked()
// {
// 	emit FindString();
// 	Replace *rep=new Replace(this);
// 	rep->loadSelectedText(findStr);
// 	QString str_=rep->replaceTxt();
// 	QString searchtext=rep->searchTxt();
// 	searchTxtReplace(str_,searchtext);
// 	emit sendString(str_);
// 	delete rep;	
//  }

// void EventEditor::actionFindNotes_clicked()
// {
// 	Find *fin=new Find(this);
// 	fin->exec();
// 	findStr=fin->findText();
//  searchTxt(findStr);
// 	emit ToFindString(fStr);
// 	delete fin; 
// }

void EventEditor::gotReplace(QString replace_)
{
  replaceStr=replace_;
}

void EventEditor::searchTxt(QString str)
{
	QString searchText=str;

	if(searchText!="")
	{ 
		for(int row=0;row<ui.tblEventEditor->rowCount();row++)
		{
			{
				QString defaultString = ui.tblEventEditor->item(row,SPEAKER_COUNT)->text();
				defaultString=defaultString.replace("<!--searched text-->","");

				QTextDocument t;
				t.setHtml(defaultString);
				QString actualString = t.toPlainText();

				if(actualString.trimmed().size()==0) continue;

				if(actualString.contains(searchText,Qt::CaseInsensitive))
				{
					QString tempStr="<!--searched text-->"+defaultString;
					ui.tblEventEditor->item(row,SPEAKER_COUNT)->setText(tempStr);
				}
				else
				{
					ui.tblEventEditor->item(row,SPEAKER_COUNT)->setText(defaultString);
				}
			}

			{
				QString defaultString = ui.tblEventEditor->item(row,NOTE_COUNT)->text();
				defaultString=defaultString.replace("<!--searched text-->","");

				QTextDocument t;
				t.setHtml(defaultString);
				QString actualString = t.toPlainText();

				if(actualString.trimmed().size()==0) continue;

				if(actualString.contains(searchText,Qt::CaseInsensitive))
				{
					QString tempStr="<!--searched text-->"+defaultString;
					ui.tblEventEditor->item(row,NOTE_COUNT)->setText(tempStr);
				}
				else
				{
					ui.tblEventEditor->item(row,NOTE_COUNT)->setText(defaultString);
				}
			} 
			{
				QString defaultString = ui.tblEventEditor->item(row,INTERNAL)->text();
				//QString defaultString =txteditList.at(row)->text();
				defaultString=defaultString.replace("<!--searched text-->","");

				QTextDocument t;
				t.setHtml(defaultString);
				QString actualString = t.toPlainText();

				if(actualString.trimmed().size()==0) continue;

				if(actualString.contains(searchText,Qt::CaseInsensitive))
				{
					QString tempStr="<!--searched text-->"+defaultString;
					ui.tblEventEditor->item(row,INTERNAL)->setText(tempStr);
					//txteditList.at(row)->setText(tempStr);
				}
				else
				{
					ui.tblEventEditor->item(row,INTERNAL)->setText(defaultString);
					//txteditList.at(row)->setText(defaultString);
				}
			} 

			{
				QString defaultString = ui.tblEventEditor->item(row,EXTERNAL)->text();
				defaultString=defaultString.replace("<!--searched text-->","");

				QTextDocument t;
				t.setHtml(defaultString);
				QString actualString = t.toPlainText();

				if(actualString.trimmed().size()==0) continue;

				if(actualString.contains(searchText,Qt::CaseInsensitive))
				{
					QString tempStr="<!--searched text-->"+defaultString;
					ui.tblEventEditor->item(row,EXTERNAL)->setText(tempStr);
				}
				else
				{
					ui.tblEventEditor->item(row,EXTERNAL)->setText(defaultString);
				}
			} 
		}
	}
}

void EventEditor::searchTxtReplace(QString replaceTxt,QString search_)
{

	QString searchText=search_;

	if(searchText!="")
	{ 
		for(int row=0;row<ui.tblEventEditor->rowCount();row++)
		{
			{
				QString speakerString = ui.tblEventEditor->item(row,SPEAKER_COUNT)->text();
				QString defaultString = ui.tblEventEditor->item(row,SPEAKER_COUNT)->text();
				defaultString=defaultString.replace("<!--searched text-->","");

				QTextDocument t;
				t.setHtml(defaultString);
				QString actualString = t.toPlainText();

				if(actualString.trimmed().size()==0) continue;

				if(actualString.contains(searchText,Qt::CaseInsensitive))
				{
					QString tempStr=speakerString.replace(searchText,replaceTxt);
					ui.tblEventEditor->item(row,SPEAKER_COUNT)->setText(tempStr);
				}
				else
				{
					ui.tblEventEditor->item(row,SPEAKER_COUNT)->setText(defaultString);
				}
			}

			{
				QString NoteString = ui.tblEventEditor->item(row,NOTE_COUNT)->text();
				QString defaultString = ui.tblEventEditor->item(row,NOTE_COUNT)->text();
				defaultString=defaultString.replace("<!--searched text-->","");

				QTextDocument t;
				t.setHtml(defaultString);
				QString actualString = t.toPlainText();

				if(actualString.trimmed().size()==0) continue;

				if(actualString.contains(searchText,Qt::CaseInsensitive))
				{
					QString tempStr=NoteString.replace(searchText,replaceTxt);
					ui.tblEventEditor->item(row,NOTE_COUNT)->setText(tempStr);
				}
				else
				{
					ui.tblEventEditor->item(row,NOTE_COUNT)->setText(defaultString);
				}
			}

			{
				QString InternalString = ui.tblEventEditor->item(row,INTERNAL)->text();
				QString defaultString = ui.tblEventEditor->item(row,INTERNAL)->text();
				//QString InternalString = txteditList.at(row)->text();
				//QString defaultString = txteditList.at(row)->text();
				defaultString=defaultString.replace("<!--searched text-->","");

				QTextDocument t;
				t.setHtml(defaultString);
				QString actualString = t.toPlainText();

				if(actualString.trimmed().size()==0) continue;

				if(actualString.contains(searchText,Qt::CaseInsensitive))
				{
					QString tempStr=InternalString.replace(searchText,replaceTxt);
					ui.tblEventEditor->item(row,INTERNAL)->setText(tempStr);
					//txteditList.at(row)->setText(tempStr);
				}
				else
				{
					ui.tblEventEditor->item(row,INTERNAL)->setText(defaultString);
					//txteditList.at(row)->setText(defaultString);
				}
			} 

			{
				QString ExternalString = ui.tblEventEditor->item(row,EXTERNAL)->text();
				QString defaultString = ui.tblEventEditor->item(row,EXTERNAL)->text();
				defaultString=defaultString.replace("<!--searched text-->","");

				QTextDocument t;
				t.setHtml(defaultString);
				QString actualString = t.toPlainText();

				if(actualString.trimmed().size()==0) continue;

				if(actualString.contains(searchText,Qt::CaseInsensitive))
				{
					QString tempStr=ExternalString.replace(searchText,replaceTxt);
					ui.tblEventEditor->item(row,EXTERNAL)->setText(tempStr);
				}
				else
				{
					ui.tblEventEditor->item(row,EXTERNAL)->setText(defaultString);
				}
			} 	
		}
	}
}

void EventEditor::insertImportRows(QList<EventNoteInfo> allEventNotes_,bool chkState_)
{
	ui.tblEventEditor->insertImportRows(allEventNotes_,chkState_);
}

void EventEditor::deleteNotes(int row_)
{
	QString timeStr;
	timeStr=sharedObj->plainText(ui.tblEventEditor->item(row_,TIME_COUNT)->text());

	QString strDateTime=ui.txtDate->text()+" "+timeStr;

	tempDB->deleteEventNotes(strDateTime);

}

EventEditor::~EventEditor()
{

}
