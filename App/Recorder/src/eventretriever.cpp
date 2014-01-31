#include "eventretriever.h"

#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QPrinter>
#include <QTextCodec>
#include <QTextStream>
#include <QTextDocument>
#include <QMenu>
#include <QTableWidgetItem>
#include <QDateTime>
#include <QClipboard>
#include <QLineEdit>
#include "customdeligate.h"
#include "eventnoteutil.h"
#include "customprogressbar.h"
#include "backupserverconnection.h"
#include "attachedfileutil.h"
#include "referencebookutil.h"
#include "referencelinkutil.h"

#define TIME_COUNT 0
#define SPEAKER_COUNT 1
#define NOTE_COUNT 2
#define INTERNAL 3
#define EXTERNAL 4

EventRetriever::EventRetriever(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	sharedObj=new DLCRShared();
	logSession=LoginUserSession::session();

	exportToPdf = new QAction(tr("&Export To pdf"), this);
	exportToPdf->setStatusTip(tr("Export To pdf"));
	exportToWordDoc = new QAction(tr("&Export to doc"), this);
	exportToWordDoc->setStatusTip(tr("Export to doc"));
	exportToHtml = new QAction(tr("&Export To html"), this);
	exportToHtml->setStatusTip(tr("Export To Html"));
	exportToCsv = new QAction(tr("&Export to csv"), this);
	exportToCsv->setStatusTip(tr("Export to csv"));

	connect(ui.btnUpdate,			SIGNAL(clicked()),					 this,SLOT(updateAllNotes()));
	connect(ui.tblEventRetriever,	SIGNAL(cellChanged(int,int)),		 this,SLOT(eventRetriever_cellChanged(int,int)));
 	connect(exportToPdf,			SIGNAL(triggered()),				 this,SLOT(exportToPdf_click()));
 	connect(exportToWordDoc,		SIGNAL(triggered()),				 this,SLOT(exportToWordDoc_click()));
 	connect(exportToHtml,			SIGNAL(triggered()),				 this,SLOT(exportToHtml_click()));
 	connect(exportToCsv,			SIGNAL(triggered()),				 this,SLOT(exportToCsv_click()));
	connect(ui.txtSearch,			SIGNAL(textChanged(const QString &)),this,SLOT(enableSearchGo()));
	connect(ui.btnGo,				SIGNAL(clicked()),					 this,SLOT(search()));
	connect(ui.btnCloseSearch,		SIGNAL(clicked()),					 this,SLOT(closeSearch()));
	connect(ui.tblEventRetriever,	SIGNAL(cellClicked(int,int)),		 this,SLOT(eventRetriver_cellClicked(int,int)));
	connect(ui.tblEventRetriever,	SIGNAL(cellDoubleClicked (int,int)), this,SLOT(eventRetriver_cellDoubleClicked(int,int)));
	connect(ui.btnPlusRow,			SIGNAL(clicked()),					 this,SLOT(btnPlus_Clicked()));
	connect(ui.btnMinusRow,			SIGNAL(clicked()),					 this,SLOT(btnMinus_Clicked()));

	ui.tblEventRetriever->setColumnWidth(0,175);
	ui.tblEventRetriever->setColumnWidth(1,150);
	ui.tblEventRetriever->setColumnWidth(2,300);
	ui.tblEventRetriever->setColumnWidth(3,100);

	ui.btnPlusRow->setEnabled(false);
	ui.btnMinusRow->setEnabled(false);
	ui.btnBold->setEnabled(false);
	ui.btnItalic->setEnabled(false);
	ui.btnUnderline->setEnabled(false);
}
void EventRetriever::btnPlus_Clicked()
{
	ui.tblEventRetriever->insertRow(cRow+1);
	ui.tblEventRetriever->activate(cRow);
	ui.btnPlusRow->setEnabled(false);
	ui.btnMinusRow->setEnabled(false);
	emit retreiveRowPress();
}
void EventRetriever::btnMinus_Clicked()
{
	int eventnoteID=ui.tblEventRetriever->item(ui.tblEventRetriever->currentRow(),0)->data(Qt::UserRole).toInt();
	deleteFromRow(eventnoteID);
	ui.tblEventRetriever->removeRow(cRow);
	ui.btnPlusRow->setEnabled(false);
	ui.btnMinusRow->setEnabled(false);
	emit retreiveRowPress();
}
void EventRetriever::eventRetriver_cellClicked(int row,int col)
{
	ui.btnPlusRow->setEnabled(false);
	ui.btnMinusRow->setEnabled(false);
	cRow=row;
	selectedColumn=ui.tblEventRetriever->selectedRanges();
	emit retrieveCellClicked();

	ui.btnBold->setEnabled(false);
	ui.btnItalic->setEnabled(false);
	ui.btnUnderline->setEnabled(false);
	
}
void EventRetriever::eventRetriver_cellDoubleClicked(int row_,int column_)
{
	ui.btnBold->setEnabled(true);
	ui.btnItalic->setEnabled(true);
	ui.btnUnderline->setEnabled(true);

// 	if (column_==INTERNAL)
// 	{
// 		QString txt=sharedObj->plainText(ui.tblEventRetriever->item(row_,column_)->text());
// 
// 		QLineEdit *edit= new QLineEdit(this);
// 		edit->setValidator(new QRegExpValidator(QRegExp("^([0-9]{2}\:{1}\[0-9]{2}\:{1}\[0-9]{2})$"),edit));
// 		ui.tblEventRetriever->setCellWidget(row_,INTERNAL, edit);
// 		edit->setText(txt);
// 	}

	emit retrieveCellDoubleClicked();

	if (copyStringStatus)
	{

	}
	else
	{
		QApplication::clipboard()->clear();
	}

}
int EventRetriever::getRetrieveRow()
{
	return cRow;
}
QList<QTableWidgetSelectionRange> EventRetriever::getSelectedColumn()const
{
	return selectedColumn;
}

void EventRetriever::createEditor()
{	
	CustomDeligate *myDelegate=new CustomDeligate(this);
	ui.tblEventRetriever->setItemDelegate(myDelegate);	

	connect(ui.btnBold,		SIGNAL(clicked(bool)),			myDelegate,SLOT(setBold(bool)));
	connect(ui.btnItalic,	SIGNAL(clicked(bool)),			myDelegate,SLOT(setItalic(bool)));
	connect(ui.btnUnderline,SIGNAL(clicked(bool)),			myDelegate,SLOT(setUnderline(bool)));

	connect(myDelegate,		SIGNAL(currentBold(bool)),		this, SLOT(checkBold(bool)));
	connect(myDelegate,		SIGNAL(currentItalic(bool)),	this, SLOT(checkItalic(bool)));
	connect(myDelegate,		SIGNAL(currentUnderline(bool)), this, SLOT(checkUnderline(bool)));

	connect(myDelegate,		SIGNAL(selectionStatus(bool)),  this,SLOT(getSelection_Status(bool)));

	connect(this,			SIGNAL(FindBold(bool)),			myDelegate,SLOT(setBold(bool)));
	connect(this,			SIGNAL(FindItalics(bool)),		myDelegate,SLOT(setItalic(bool)));
	connect(this,			SIGNAL(FindUnderLine(bool)),	myDelegate,SLOT(setUnderline(bool)));

	connect(this,			SIGNAL(textHighlightColor()),	myDelegate,SLOT(setColorText()));

	ui.btnUpdate->setEnabled(false);
}

void EventRetriever::getSelection_Status(bool staus_)
{
	copyStringStatus=staus_;
}
void EventRetriever::actionBold_Status(bool stats)
{
	emit FindBold(stats);
}
void EventRetriever::actionItalics_Status(bool stats)
{
	emit FindItalics(stats);
}
void EventRetriever::actionUnderLine_Status(bool stats)
{
	emit FindUnderLine(stats);
}
void EventRetriever::insertRows(QList<EventNoteInfo> allEventNotes_)
{
	ui.tblEventRetriever->clear();
	ui.tblEventRetriever->insertRows(allEventNotes_);
	ui.btnUpdate->setEnabled(false);
}
void EventRetriever::insertImportRows(QList<EventNoteInfo> allEventNotes_)
{
	//ui.tblEventRetriever->clear();
	ui.tblEventRetriever->insertImportRows(allEventNotes_);
	ui.btnUpdate->setEnabled(true);
}
void  EventRetriever::setEditable(bool status_)
{
	if(status_!=false) ui.tblEventRetriever->setEditTriggers(QAbstractItemView::DoubleClicked);
	else ui.tblEventRetriever->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
void EventRetriever::setEventDetails(retrieveDetails details_)
{
	ui.txtEventNo->setText(details_.no);
	ui.txtEventNo->setToolTip(details_.no);
	ui.txtTitle->setText(details_.title);
	ui.txtTitle->setToolTip(details_.title);
	ui.txtDescription->setText(details_.description);
	ui.txtDescription->setToolTip(details_.description);
	ui.txtDate->setText(details_.dt.toString("dd/MM/yyyy"));
	ui.txtDate->setToolTip(details_.dt.toString("dd/MM/yyyy"));
	ui.txtEventType->setText(details_.eventType);
	ui.txtEventType->setToolTip(details_.eventType);
}
void EventRetriever::attachFile(QString fileName_,QString Size_,QString Type_,QString timeStr)
{
 	BackupServerConnection *conn=BackupServerConnection::connection();
 
 	conn->createConnection(logSession->getServerIP(),logSession->getServerAdminName(),logSession->getServerAdminPassword());
 	if (!conn->isOpen())
 	{
 		SHOW_MESSAGE(tr("Server Connection Failed!!!"),tr("Please create connection to attach files"),tr("Ok"));
 		return;
 	}

	QString homePath=sharedObj->getEventHomePath(logSession->getRetrievedLocation(),logSession->getRetrievedRoom(),ui.txtEventType->text(),ui.txtEventNo->text());
	
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

	if(exists(destinationPath.split("\\").last(),logSession->getRetrievedID()))
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
		PRINT_ERROR("Error occurred while attaching file. Source or destination file is not match.");
		PRINT_WARNING("Source size : "+QString::number(src.size()));
		PRINT_WARNING("Destination size : "+QString::number(dst.size()));
		SHOW_MESSAGE(tr("Error"),tr("Attaching file failed."),tr("Ok"));
		return;
	}


	int eventNoteID;

	EventNoteUtil *eutil=new EventNoteUtil(this);
	
	AttachedFileUtil *util=new AttachedFileUtil(this);

	try
	{
		QList<EventNoteInfo> eInfoAll;

		eInfoAll=eutil->selectEventNotesByEvent(logSession->getRetrievedID(),logSession->getDriverName());

		foreach(EventNoteInfo eInfo,eInfoAll)
		{	
			QString str;
			QDateTime eventTime;
			eventTime=eInfo.getEventTime();
			str=eventTime.toString("dd-MM-yyyy hh:mm:ss AP");

			if (str==timeStr)
			{
				eventNoteID=eInfo.getEventNoteID();
			}
		}
		//eventNoteID=ui.tblEventRetriever->item(ui.tblEventRetriever->currentRow(),0)->data(Qt::UserRole).toInt();
		AttachedFileInfo info;
		info.setAttachedPath(destinationPath);
		info.setAttachedhash("--");
		info.setEventNoteID(eventNoteID);
		info.setAttachedSize(Size_);
		info.setAttachedType(Type_);
		util->insertOrUpdateAttachedFile(info,logSession->getDriverName());
	}
	catch(DBExeption e)//catching db exeptions
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
	catch(...)//catching unknown wxwptions
	{
		PRINT_ERROR("UNKnown ERROR");
	}

	delete eutil;

	delete util;
}
void EventRetriever::attachBook(QString bName,QString bAuthor,QString bDescription,QString timeStr,QString edition_)
{
	
	EventNoteUtil *eutil=new EventNoteUtil(this);
	ReferenceBookUtil *bUtil=new ReferenceBookUtil(this);
	int eventNoteID;
	try
	{
		QList<EventNoteInfo> eInfoAll;

		eInfoAll=eutil->selectEventNotesByEvent(logSession->getRetrievedID(),logSession->getDriverName());

		foreach(EventNoteInfo eInfo,eInfoAll)
		{
			QString str;
			QDateTime eventTime;
			eventTime=eInfo.getEventTime();
			str=eventTime.toString("dd-MM-yyyy hh:mm:ss AP");

			if (str==timeStr)
			{
				eventNoteID=eInfo.getEventNoteID();
			}
		}

		ReferenceBookinfo bInfo;
		bInfo.setBookName(bName);
		bInfo.setAuthor(bAuthor);
		bInfo.setEdition(edition_);
		bInfo.setBookDecription(bDescription);
		bInfo.setEventNoteID(eventNoteID);

		bUtil->insertOrUpdateReferenceBook(bInfo,logSession->getDriverName());
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

	delete eutil;

}
void EventRetriever::attachLink(QString link,QString Description,QString timeStr)
{
	
	EventNoteUtil *eutil=new EventNoteUtil(this);
	ReferenceLinkutil *lUtil=new ReferenceLinkutil(this);
	int eventNoteID;
	try
	{
		QList<EventNoteInfo> eInfoAll;

		eInfoAll=eutil->selectEventNotesByEvent(logSession->getRetrievedID(),logSession->getDriverName());

		foreach(EventNoteInfo eInfo,eInfoAll)
		{	
			QString str;
			QDateTime eventTime;
			eventTime=eInfo.getEventTime();
			str=eventTime.toString("dd-MM-yyyy hh:mm:ss AP");

			if (str==timeStr)
			{
				eventNoteID=eInfo.getEventNoteID();
			}
		}

		ReferenceLinkInfo lInfo;

		lInfo.setURL(link);
		lInfo.setURLDescription(Description);
		lInfo.setEventNoteID(eventNoteID);

		lUtil->insertOrUpdateReferenceLink(lInfo,logSession->getDriverName());
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

	delete eutil;

}
bool EventRetriever::exists(QString fileName_,int eventID_)
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
void EventRetriever::updateAllNotes()
{
	EventNoteUtil *util=new EventNoteUtil(this);
	try
	{
		for(int i=0;i<ui.tblEventRetriever->rowCount();i++)
		{
			EventNoteInfo info,einfo;

			int noteID=ui.tblEventRetriever->item(i,TIME_COUNT)->data(Qt::UserRole).toInt();
			QDateTime eventTime;
			QString strTime;
			strTime=sharedObj->plainText(ui.tblEventRetriever->item(i,TIME_COUNT)->text());
			eventTime= QDateTime::fromString(strTime,"dd/MM/yyyy hh:mm:ss AP");
			einfo=util->loadEventNote(noteID,logSession->getDriverName());
		
			info.setEventNoteID(noteID);
			info.setEventTime(einfo.getEventTime());
			info.setEventSpeaker(sharedObj->plainText(ui.tblEventRetriever->item(i,SPEAKER_COUNT)->text()));
			info.setEventNote(sharedObj->plainText(ui.tblEventRetriever->item(i,NOTE_COUNT)->text()));
			info.setRichSpeaker(ui.tblEventRetriever->item(i,SPEAKER_COUNT)->text().replace("<!--searched text-->",""));
			info.setRichNote(ui.tblEventRetriever->item(i,NOTE_COUNT)->text().replace("<!--searched text-->",""));
			info.setElapsedTime(sharedObj->plainText(ui.tblEventRetriever->item(i,INTERNAL)->text()));
			info.setSessionURL(sharedObj->plainText(ui.tblEventRetriever->item(i,EXTERNAL)->text()));
			info.setVisibility(1);
			info.setFileID(einfo.getFileID());
			info.setRoomID(einfo.getRoomID());
			info.setPCID(einfo.getPCID());
			info.setUserID(einfo.getUserID());
			info.setSessionID(einfo.getSessionID());
			if (einfo.getEventID()!=logSession->getRetrievedID())
			{
				info.setEventID(logSession->getRetrievedID());
				util->updateEventNoteRetriever(info,logSession->getDriverName());
			}
			else
			{
				info.setEventID(einfo.getEventID());
				util->editEventNote(info,logSession->getDriverName());
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

	ui.btnUpdate->setEnabled(false);	
}
void EventRetriever::contextMenuEvent(QContextMenuEvent * event_)
{
	if(logSession->canExportToFile())
	{
		QMenu menu(this);
		menu.addAction(exportToPdf);
		menu.addAction(exportToWordDoc);
		menu.addAction(exportToCsv);
		menu.exec(event_->globalPos());
	}
}
void EventRetriever::exportPdf()
{
	QList<QTableWidgetItem *> SelectedItems=ui.tblEventRetriever->selectedItems();
	if(SelectedItems.count()!=0)
	{
		exportToPdf_click();
	}
	else
	{
		QList<QStringList> selectedRows;

		for(int i=0;i<ui.tblEventRetriever->rowCount();i++)
		{
			QStringList tmpList;
			tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,0)->text());
			tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,1)->text());
			tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,2)->text());
			tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,3)->text());
			tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,4)->text());
			selectedRows<<tmpList;
		}

		QString html=convertToHtml(selectedRows);

		QString fileName = QFileDialog::getSaveFileName(this, tr("Export PDF"), QString(), "*.pdf");
		if (!fileName.isEmpty())
		{
			if (QFileInfo(fileName).suffix().isEmpty())
			{
				fileName.append(".pdf");
			}
			createPdfFile(fileName,html);
		}
	}
}
void EventRetriever::exportToPdf_click()
{
	QList<QStringList> selectedRows;

	for(int i=0;i<ui.tblEventRetriever->rowCount();i++)
	{
		if(ui.tblEventRetriever->item(i,1)->isSelected())
		{
			QStringList tmpList;
			tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,0)->text());
			tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,1)->text());
			tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,2)->text());
			tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,3)->text());
			tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,4)->text());

			selectedRows<<tmpList;
		}
	}

	QString html=convertToHtml(selectedRows);

	QString fileName = QFileDialog::getSaveFileName(this, tr("Export PDF"), QString(), "*.pdf");
	if (!fileName.isEmpty())
	{
		if (QFileInfo(fileName).suffix().isEmpty())
		{
			fileName.append(".pdf");
		}
		createPdfFile(fileName,html);
	}
}
void EventRetriever::exportWordDoc()
{
	QList<QTableWidgetItem *> SelectedItems=ui.tblEventRetriever->selectedItems();
	if(SelectedItems.count()!=0)
	{
		exportToWordDoc_click();
	}
	else
	{
		QList<QStringList> selectedRows;

		for(int i=0;i<ui.tblEventRetriever->rowCount();i++)
		{
			QStringList tmpList;
			tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,0)->text());
			tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,1)->text());
			tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,2)->text());
			tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,3)->text());
			tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,4)->text());

			selectedRows<<tmpList;	
		}

		QString html=convertToWordHtml(selectedRows);
/*		html=html.replace(":/Recorder/Resources/AdjdLogo.png","logo.jpg");*/

		QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
			QString(), tr("Document (*.doc)"));

		if (!fileName.isEmpty())
		{
			createWordDocument(fileName,html);

// 			QString destination;
// 			QStringList tempList=fileName.split("/");
// 			destination=tempList[0];
// 
// 			for(int i=1;i<tempList.size()-1;i++)
// 			{
// 				destination=destination+"/"+tempList[i];
// 			}	
// 
// 			destination=destination+"/logo.jpg";
// 
// 			QFile *dst=new QFile();
// 
// 			if(!dst->copy("logo.jpg",destination)) PRINT_ERROR(dst->errorString());
		}
	}
}
void EventRetriever::exportToWordDoc_click()
{
	QList<QStringList> selectedRows;

	for(int i=0;i<ui.tblEventRetriever->rowCount();i++)
	{
		if(ui.tblEventRetriever->item(i,1)->isSelected())
		{
			QStringList tmpList;
			tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,0)->text());
			tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,1)->text());
			tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,2)->text());
			tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,3)->text());
			tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,4)->text());

			selectedRows<<tmpList;
		}
	}

	QString html=convertToWordHtml(selectedRows);
	//html=html.replace(":/Recorder/Resources/AdjdLogo.png","logo.jpg");

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
		QString(), tr("Document (*.doc)"));

	if (!fileName.isEmpty())
	{
		createWordDocument(fileName,html);

// 			QString destination;
// 			QStringList tempList=fileName.split("/");
// 			destination=tempList[0];
// 
// 			for(int i=1;i<tempList.size()-1;i++)
// 			{
// 				destination=destination+"/"+tempList[i];
// 			}	
// 
// 			destination=destination+"/logo.jpg";
// 
// 			QFile *dst=new QFile();
// 
// 			if(!dst->copy("logo.jpg",destination)) PRINT_ERROR(dst->errorString());		

	}
}
void EventRetriever::exportToHtml_click()
{
	QList<QStringList> selectedRows;

	for(int i=0;i<ui.tblEventRetriever->rowCount();i++)
	{
		if(ui.tblEventRetriever->item(i,1)->isSelected())
		{
			QStringList tmpList;
			tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,0)->text());
			tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,1)->text());
			tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,2)->text());
			tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,3)->text());
			tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,4)->text());

			selectedRows<<tmpList;
		}
	}

	QString html=convertToHtml(selectedRows);

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
		QString(), tr("Document (*.html)"));

	if (!fileName.isEmpty())
	{
		createHtmlFile(fileName,html);

		QString destination;
		QStringList tempList=fileName.split("/");
		destination=tempList[0];

		for(int i=1;i<tempList.size()-1;i++)
		{
			destination=destination+"/"+tempList[i];
		}	

		destination=destination+"/logo.jpg";

		QFile *dst=new QFile();

		if(!dst->copy("logo.jpg",destination)) PRINT_ERROR(dst->errorString());
	}
}
void EventRetriever::exportToCsv_click()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Export CSV"), QString(), "*.csv");
	if (!fileName.isEmpty())
	{
		if (QFileInfo(fileName).suffix().isEmpty())
		{
			fileName.append(".csv");
		}
		QFile f(fileName);

		if (f.open(QFile::WriteOnly | QFile::Truncate))
		{
			QTextStream data( &f );
			QStringList strList;

			QStringList header;
			QList<QTableWidgetItem *> SelectedItems=ui.tblEventRetriever->selectedItems();
			if(SelectedItems.count()!=0)
			{
				
				header<< ui.tblEventRetriever->horizontalHeaderItem(0)->data(Qt::DisplayRole).toString();
				header<< ui.tblEventRetriever->horizontalHeaderItem(1)->data(Qt::DisplayRole).toString();
				header<< ui.tblEventRetriever->horizontalHeaderItem(2)->data(Qt::DisplayRole).toString();
				header<< ui.tblEventRetriever->horizontalHeaderItem(3)->data(Qt::DisplayRole).toString();
				header<< ui.tblEventRetriever->horizontalHeaderItem(4)->data(Qt::DisplayRole).toString();
				data <<header.join( "," )+"\n";
				for( int r = 0; r < ui.tblEventRetriever->rowCount(); ++r )
				{
					QStringList tmpList;
					tmpList.clear();
					if(ui.tblEventRetriever->item(r,1)->isSelected())
					{
						tmpList<<"\" "+sharedObj->plainText(ui.tblEventRetriever->item(r,0)->text())+"\" ";
						tmpList<<"\" "+sharedObj->plainText(ui.tblEventRetriever->item(r,1)->text())+"\" ";
						tmpList<<"\" "+sharedObj->plainText(ui.tblEventRetriever->item(r,2)->text())+"\" ";
						tmpList<<"\" "+sharedObj->plainText(ui.tblEventRetriever->item(r,3)->text())+"\" ";
						tmpList<<"\" "+sharedObj->plainText(ui.tblEventRetriever->item(r,4)->text())+"\" ";
						data<< tmpList.join( "," )+"\n";
					}
						
				}
			}
			else
			{

				header<< ui.tblEventRetriever->horizontalHeaderItem(0)->data(Qt::DisplayRole).toString();
				header<< ui.tblEventRetriever->horizontalHeaderItem(1)->data(Qt::DisplayRole).toString();
				header<< ui.tblEventRetriever->horizontalHeaderItem(2)->data(Qt::DisplayRole).toString();
				header<< ui.tblEventRetriever->horizontalHeaderItem(3)->data(Qt::DisplayRole).toString();
				header<< ui.tblEventRetriever->horizontalHeaderItem(4)->data(Qt::DisplayRole).toString();
				data <<header.join( "," )+"\n";
				
				for( int r = 0; r < ui.tblEventRetriever->rowCount(); ++r )
				{
					QStringList tmpList;
					tmpList.clear();
					
						tmpList<<"\" "+sharedObj->plainText(ui.tblEventRetriever->item(r,0)->text())+"\" ";
						tmpList<<"\" "+sharedObj->plainText(ui.tblEventRetriever->item(r,1)->text())+"\" ";
						tmpList<<"\" "+sharedObj->plainText(ui.tblEventRetriever->item(r,2)->text())+"\" ";
						tmpList<<"\" "+sharedObj->plainText(ui.tblEventRetriever->item(r,3)->text())+"\" ";
						tmpList<<"\" "+sharedObj->plainText(ui.tblEventRetriever->item(r,4)->text())+"\" ";
						data<< tmpList.join( "," )+"\n";
				}			
					
			}

			f.close();
		}
	}
}
QString EventRetriever::convertToHtml(QList<QStringList> selectedRows_)
{
	QString htmlString="<html>"
		"<body>"
		"<br>"
		"<table width=\"600\" border=\"0\" bordercolor=\"#EFEFFB\" cellspacing=\"0\" cellpadding=\"0\">"
		"<tr><td colspan=\"10\">&nbsp;</td></tr>"
		"<tr>"
		"<td colspan=\"10\" height=\"213\" valign=\"bottom\" align=\"center\">&nbsp;<img src=\":/Recorder/Resources/AdjdLogo.png\" height=\"140\"></td>"
		"</tr>"
		"<tr><td colspan=\"10\" height=\"100\">"
		"<table width=\"600\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">"
		"<tr><td colspan=\"3\">&nbsp;</td></tr>"
		"<tr>"
		"<th align=\"left\"><font face=\"arial\" size=\"2\" color=\"#003664\">&nbsp;Event No</font></th>"			
		"<th align=\"left\"><font face=\"arial\" size=\"2\" color=\"#003664\">&nbsp;Event Title</font></th>"			
		"<th colspan=\"2\" align=\"left\"><font face=\"arial\" size=\"2\" color=\"#003664\">&nbsp;Description</font></th>"
		"</tr>"	
		"<tr>"
		"<td>&nbsp;<table width=\"147\" border=\"1\" bordercolor=\"#f1f1f1\" cellspacing=\"0\" cellpadding=\"0\"><tr><td><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;"+ui.txtEventNo->text()+"</font></td></tr></table></td>"	
		"<td>&nbsp;<table width=\"147\" border=\"1\" bordercolor=\"#f1f1f1\" cellspacing=\"0\" cellpadding=\"0\"><tr><td><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;"+ui.txtTitle->text()+"</font></td></tr></table></td>"
		"<td colspan=\"2\">&nbsp;<table width=\"300\" border=\"1\" bordercolor=\"#f1f1f1\" cellspacing=\"0\" cellpadding=\"0\"><tr><td><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;"+ui.txtDescription->text()+"</font></td></tr></table></td>"	
		"</tr>"
		"<tr><td colspan=\"3\">&nbsp;</td></tr>"
		"<tr>"
		"<th align=\"left\"><font face=\"arial\" size=\"2\" color=\"#003664\">&nbsp;Location</font></th>"		
		"<th align=\"left\"><font face=\"arial\" size=\"2\" color=\"#003664\">&nbsp;Room</font></th>"
		"<th align=\"left\"><font face=\"arial\" size=\"2\" color=\"#003664\">&nbsp;Event Type</font></th>"	
		"<th align=\"left\"><font face=\"arial\" size=\"2\" color=\"#003664\">&nbsp;Created Date</font></th>"				
		"</tr>"	
		"<tr>"
		"<td>&nbsp;<table width=\"147\" border=\"1\" bordercolor=\"#f1f1f1\" cellspacing=\"0\" cellpadding=\"0\"><tr><td><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;"+logSession->getRetrievedLocation()+"</font></td></tr></table></td>"
		"<td>&nbsp;<table width=\"147\" border=\"1\" bordercolor=\"#f1f1f1\" cellspacing=\"0\" cellpadding=\"0\"><tr><td><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;"+logSession->getRetrievedRoom()+"</font></td></tr></table></td>"	
		"<td>&nbsp;<table width=\"147\" border=\"1\" bordercolor=\"#f1f1f1\" cellspacing=\"0\" cellpadding=\"0\"><tr><td><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;"+ui.txtEventType->text()+"</font></td></tr></table></td>"
		"<td>&nbsp;<table width=\"150\" border=\"1\" bordercolor=\"#f1f1f1\" cellspacing=\"0\" cellpadding=\"0\"><tr><td><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;"+ui.txtDate->text()+"</font></td></tr></table></td>"				
		"</tr>"
		"<tr><td colspan=\"3\">&nbsp;</td></tr>"
		"</table>"
		"</td></tr>"
		"<tr><td colspan=\"10\">"
		"<table width=\"600\" border=\"0\" bordercolor=\"#EFEFFB\" cellspacing=\"0\" cellpadding=\"0\">"
		"<tr>"
		"<th bgcolor=\"#003664\" width=\"120\" height=\"30\" align=\"left\"><font face=\"arial\" size=\"3\" color=\"#FFFFFF\">&nbsp;Date & Time</font></th>"
		"<th bgcolor=\"#003664\" width=\"100\" height=\"30\" align=\"left\"><font face=\"arial\" size=\"3\" color=\"#FFFFFF\">&nbsp;Speaker</font></th>"
		"<th bgcolor=\"#003664\" width=\"170\" height=\"30\" align=\"left\"><font face=\"arial\" size=\"3\" color=\"#FFFFFF\">&nbsp;Notes</font></th>"
		"<th bgcolor=\"#003664\" width=\"100\" height=\"30\" align=\"left\"><font face=\"arial\" size=\"3\" color=\"#FFFFFF\">&nbsp;InternalReference</font></th>"
		"<th bgcolor=\"#003664\" width=\"100\" height=\"30\" align=\"left\"><font face=\"arial\" size=\"3\" color=\"#FFFFFF\">&nbsp;ExternalReference</font></th>"
		"</tr>";

	int k=0;

	foreach(QStringList itemList,selectedRows_)
	{
		QString rowColor;

		if(k==0){rowColor="#FFFFFF"; k=1;}	
		else{rowColor="#D8D8D8"; k=0;}

		htmlString.append("<tr>");

		htmlString.append("<td bgcolor="+rowColor+" width=\"120\" height=\"25\"><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;");
		htmlString.append(itemList[0]);
		htmlString.append("</font></td>");	

		htmlString.append("<td bgcolor="+rowColor+" width=\"100\" height=\"25\"><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;");
		htmlString.append(itemList[1]);
		htmlString.append("</font></td>");	

		htmlString.append("<td bgcolor="+rowColor+" width=\"170\" height=\"25\"><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;");
		htmlString.append(itemList[2]);
		htmlString.append("</font></td>");

		htmlString.append("<td bgcolor="+rowColor+" width=\"100\" height=\"25\"><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;");
		htmlString.append(itemList[3]);
		htmlString.append("</font></td>");	

		htmlString.append("<td bgcolor="+rowColor+" width=\"100\" height=\"25\"><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;");
		htmlString.append(itemList[4]);
		htmlString.append("</font></td>");	

		htmlString.append("</tr>");
	}

	htmlString=htmlString+"</table>"
		"</td></tr>"
		"<tr>"
		"<td colspan=\"10\">&nbsp;</td>"
		"</tr>"
		"</table>"
		"</body>"
		"</html>";

	return htmlString;
}

QString EventRetriever::convertToWordHtml(QList<QStringList> selectedRows_)
{
	QString htmlString="<html>"
	"<body>"
	"<div align=\"right\" style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">"
	"<br><br><br><br>"		
	"<table  align=\"center\" width=\"700\" border=\"0\" bordercolor=\"#EFEFFB\" cellspacing=\"0\" cellpadding=\"0\">"
	"<tr><td>&nbsp;</td></tr>"
	"<tr><td>&nbsp;</td></tr>"
	"<tr><td>&nbsp;</td></tr>"
		"<tr>"
			"<td bgcolor=\"#003664\" width=\"150\" height=\"25\" align=\"left\"><font face=\"arial\" size=\"3\" color=\"#FFFFFF\">&nbsp;EventNo</font></th>"
			"<th bgcolor=\"#003664\" width=\"150\" height=\"25\" align=\"left\"><font face=\"arial\" size=\"3\" color=\"#FFFFFF\">&nbsp;EventTitle</font></th>"
			"<th colspan=\"2\" bgcolor=\"#003664\" width=\"150\" height=\"30\" align=\"left\"><font face=\"arial\" size=\"3\" color=\"#FFFFFF\">&nbsp;Description</font></th>"
 		"</tr>"
/* 		"<tr><td>&nbsp;</td></tr>"*/
 		"<tr>"
		"<td bgcolor=\"#FFFFFF\" width=\"150\" height=\"25\"><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;"+ui.txtEventNo->text()+"</font></td>"
		"<td bgcolor=\"#FFFFFF\" width=\"150\" height=\"25\"><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;"+ui.txtTitle->text()+"</font></td>"
		"<td colspan=\"2\" bgcolor=\"#FFFFFF\" width=\"150\" height=\"25\"><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;"+ui.txtDescription->text()+"</font></td>"
 		"</tr>"
 		"<tr><td>&nbsp;</td></tr>"
 		"<tr>"
		"<th bgcolor=\"#003664\" width=\"150\" height=\"25\" align=\"left\"><font face=\"arial\" size=\"3\" color=\"#FFFFFF\">&nbsp;Location</font></th>"
		"<th bgcolor=\"#003664\" width=\"150\" height=\"25\" align=\"left\"><font face=\"arial\" size=\"3\" color=\"#FFFFFF\">&nbsp;Room</font></th>"
		"<th bgcolor=\"#003664\" width=\"150\" height=\"25\" align=\"left\"><font face=\"arial\" size=\"3\" color=\"#FFFFFF\">&nbsp;EventType</font></th>"
		"<th bgcolor=\"#003664\" width=\"150\" height=\"25\" align=\"left\"><font face=\"arial\" size=\"3\" color=\"#FFFFFF\">&nbsp;CreatedDate</font></th>"
 		"</tr>"
/* 		"<tr><td>&nbsp;</td></tr>"	*/	
 		"<tr>"
		"<td bgcolor=\"#FFFFFF\" width=\"150\" height=\"25\"><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;"+logSession->getRetrievedLocation()+"</font></td>"
		"<td bgcolor=\"#FFFFFF\" width=\"150\" height=\"25\"><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;"+logSession->getRetrievedRoom()+"</font></td>"
		"<td bgcolor=\"#FFFFFF\" width=\"150\" height=\"25\"><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;"+ui.txtEventType->text()+"</font></td>"
		"<td bgcolor=\"#FFFFFF\" width=\"150\" height=\"25\"><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;"+ui.txtDate->text()+"</font></td>"
		"</tr>"
		"</table>"		
		"<tr><td>&nbsp;</td></tr>"		
		"<table align=\"center\" width=\"700\" border=\"0\" bordercolor=\"#EFEFFB\" cellspacing=\"0\" cellpadding=\"0\">"
		"<tr><td>&nbsp;</td></tr>"
		"<tr>"
		"<th bgcolor=\"#003664\" width=\"150\" height=\"25\" align=\"left\"><font face=\"arial\" size=\"3\" color=\"#FFFFFF\">&nbsp;Date&Time</font></th>"
		"<th bgcolor=\"#003664\" width=\"130\" height=\"25\" align=\"left\"><font face=\"arial\" size=\"3\" color=\"#FFFFFF\">&nbsp;Speaker</font></th>"
		"<th bgcolor=\"#003664\" width=\"150\" height=\"25\" align=\"left\"><font face=\"arial\" size=\"3\" color=\"#FFFFFF\">&nbsp;Notes</font></th>"
		"<th bgcolor=\"#003664\" width=\"130\" height=\"25\" align=\"left\"><font face=\"arial\" size=\"3\" color=\"#FFFFFF\">&nbsp;InternalReference</font></th>"
		"<th bgcolor=\"#003664\" width=\"130\" height=\"25\" align=\"left\"><font face=\"arial\" size=\"3\" color=\"#FFFFFF\">&nbsp;ExternalReference</font></th>"
		"</tr>";

	int k=0;

	foreach(QStringList itemList,selectedRows_)
	{
		QString rowColor;

		if(k==0){rowColor="#FFFFFF"; k=1;}	
		else{rowColor="#D8D8D8"; k=0;}

		htmlString.append("<tr>");

		htmlString.append("<td bgcolor="+rowColor+" width=\"170\" height=\"25\"><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;");
		htmlString.append(itemList[0]);
		htmlString.append("</font></td>");	

		htmlString.append("<td bgcolor="+rowColor+" width=\"130\" height=\"25\"><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;");
		htmlString.append(itemList[1]);
		htmlString.append("</font></td>");	

		htmlString.append("<td bgcolor="+rowColor+" width=\"150\" height=\"25\"><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;");
		htmlString.append(itemList[2]);
		htmlString.append("</font></td>");

		htmlString.append("<td bgcolor="+rowColor+" width=\"100\" height=\"25\"><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;");
		htmlString.append(itemList[3]);
		htmlString.append("</font></td>");

		htmlString.append("<td bgcolor="+rowColor+" width=\"120\" height=\"25\"><font face=\"arial\" size=\"2\" color=\"#000000\">&nbsp;");
		htmlString.append(itemList[4]);
		htmlString.append("</font></td>");

		htmlString.append("</tr>");
	}

	htmlString=htmlString+"</table>"
		"</div>"
		"</body>"
		"</html>";

	return htmlString;
}

void EventRetriever::createPdfFile(QString filePath_,QString html_)
{
	QTextDocument *printtextEdit= new QTextDocument();
	QPrinter printer(QPrinter::HighResolution);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOutputFileName(filePath_);
	printtextEdit->setHtml(html_);
	printtextEdit->print(&printer);
}
void EventRetriever::createWordDocument(QString filePath_,QString html_)
{
 	QFile file(filePath_);
 	if (!file.open(QFile::WriteOnly))
 		return ;
	QTextStream ts(&file);
	ts.setGenerateByteOrderMark(true);
	ts.setCodec(QTextCodec::codecForName("UTF-8"));
 	ts << html_;	
}
void EventRetriever::createHtmlFile(QString filePath_,QString html_)
{
	QFile file(filePath_);
	if (!file.open(QFile::WriteOnly))
		return ;

	QTextStream ts(&file);
	ts.setCodec(QTextCodec::codecForName("UTF-8"));
	ts <<html_;
}
void EventRetriever::createCsvFile(QString filePath_,QString html_)
{

}
void EventRetriever::printPreview()
{
	QPrinter printer(QPrinter::HighResolution);
	QPrintPreviewDialog preview(&printer, this , Qt::Window);
	preview.setWindowTitle(tr("Print Preview"));

	QList<QToolButton *> allPButtons = preview.findChildren<QToolButton *>();

	allPButtons[1]->setToolTip (tr("Fit width"));
	allPButtons[2]->setToolTip(tr("Fit page")); 
	allPButtons[3]->setToolTip(tr("Zoom out")); 
	allPButtons[4]->setToolTip(tr("Zoom in")); 
	allPButtons[5]->setToolTip(tr("Portrait")); 
	allPButtons[6]->setToolTip(tr("Landscape")); 
	allPButtons[7]->setToolTip(tr("First page")); 
	allPButtons[8]->setToolTip(tr("Previous page")); 
	allPButtons[9]->setToolTip(tr("Next page")); 
	allPButtons[10]->setToolTip(tr("Last page")); 
	allPButtons[11]->setToolTip(tr("Show single page")); 
	allPButtons[12]->setToolTip(tr("Show facing pages")); 
	allPButtons[13]->setToolTip(tr("Show overview of all pages")); 
	allPButtons[14]->setToolTip(tr("Page setup")); 
	allPButtons[15]->setToolTip(tr("Print")); 
	

	for(int i=0;i<allPButtons.count();i++)
	{
		printf("\n %d %s", i ,allPButtons[i]->toolTip());
	}
	preview.setParent(this);
	connect(&preview, SIGNAL(paintRequested(QPrinter *)), SLOT(print(QPrinter *)));
	preview.exec();
}
void EventRetriever::print(QPrinter *printer_)
{
	QList<QStringList> selectedRows;

	for(int i=0;i<ui.tblEventRetriever->rowCount();i++)
	{		
		QStringList tmpList;
		tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,0)->text());
		tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,1)->text());
		tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,2)->text());
		tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,3)->text());
		tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,4)->text());

		selectedRows<<tmpList;		
	}

	QString html=convertToHtml(selectedRows);

	QTextDocument *printtextEdit= new QTextDocument();

	printtextEdit->setHtml(html);
	printtextEdit->print(printer_);
}
void EventRetriever::printDocument()
{
	QList<QStringList> selectedRows;

	for(int i=0;i<ui.tblEventRetriever->rowCount();i++)
	{		
		QStringList tmpList;
		tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,0)->text());
		tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,1)->text());
		tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,2)->text());
		tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,3)->text());
		tmpList<<sharedObj->plainText(ui.tblEventRetriever->item(i,4)->text());

		selectedRows<<tmpList;		
	}

	QString html=convertToHtml(selectedRows);

#ifndef QT_NO_PRINTER
	QPrinter printer(QPrinter::HighResolution);
	QPrintDialog *dlg = new QPrintDialog(&printer, this);
	dlg->setWindowTitle(tr("Print Document"));
	QString PrinterName=printer.printerName ();
	if(PrinterName=="")
	{
		return;
	}
	QTextDocument *printtextEdit= new QTextDocument();
	printtextEdit->setHtml(html);
	if (dlg->exec() == QDialog::Accepted) 
	{
		printtextEdit->print(&printer);
	}
	delete dlg;
#endif
}
void EventRetriever::enableSearchGo()
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
void EventRetriever::closeSearch()
{
	QList<QTableWidgetItem *> items=ui.tblEventRetriever->findItems("<!--searched text-->",Qt::MatchContains);

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
void EventRetriever::search()
{

	QString searchText=ui.txtSearch->text().trimmed();

	if(searchText!="")
	{ 
		for(int row=0;row<ui.tblEventRetriever->rowCount();row++)
		{
			{
				QString defaultString = ui.tblEventRetriever->item(row,SPEAKER_COUNT)->text();
				defaultString=defaultString.replace("<!--searched text-->","");

				QTextDocument t;
				t.setHtml(defaultString);
				QString actualString = t.toPlainText();

				if(actualString.trimmed().size()==0) continue;

				if(actualString.contains(searchText,Qt::CaseInsensitive))
				{
					QString tempStr="<!--searched text-->"+defaultString;
					ui.tblEventRetriever->item(row,SPEAKER_COUNT)->setText(tempStr);
				}
				else
				{
					ui.tblEventRetriever->item(row,SPEAKER_COUNT)->setText(defaultString);
				}
			}

			{
				QString defaultString = ui.tblEventRetriever->item(row,NOTE_COUNT)->text();
				defaultString=defaultString.replace("<!--searched text-->","");

				QTextDocument t;
				t.setHtml(defaultString);
				QString actualString = t.toPlainText();

				if(actualString.trimmed().size()==0) continue;

				if(actualString.contains(searchText,Qt::CaseInsensitive))
				{
					QString tempStr="<!--searched text-->"+defaultString;
					ui.tblEventRetriever->item(row,NOTE_COUNT)->setText(tempStr);
				}
				else
				{
					ui.tblEventRetriever->item(row,NOTE_COUNT)->setText(defaultString);
				}
			} 	
		}
	}
}
void EventRetriever::eventRetriever_cellChanged(int row_,int column_)
{
	cRow=row_;
	if(row_==-1) return;
	ui.btnUpdate->setEnabled(true);
}
void EventRetriever::checkBold(bool status_)
{
	ui.btnBold->setChecked(status_);
	emit chkRBStatus(status_);
}
void EventRetriever::checkItalic(bool status_)
{
	ui.btnItalic->setChecked(status_);
	emit chkRIStatus(status_);
}
void EventRetriever::checkUnderline(bool status_)
{
	ui.btnUnderline->setChecked(status_);
	emit chkRUStatus(status_);
}
QString EventRetriever::getCurrentRetrieveTime(int trow)
{
	QString timeStr;
	timeStr=sharedObj->plainText(ui.tblEventRetriever->item(trow,TIME_COUNT)->text());
	QString strDateTime=timeStr;

	return strDateTime;
}
void EventRetriever::searchTxt(QString str)
{

	QString searchText=str;

	if(searchText!="")
	{ 
		for(int row=0;row<ui.tblEventRetriever->rowCount();row++)
		{
			{
				QString defaultString = ui.tblEventRetriever->item(row,SPEAKER_COUNT)->text();
				defaultString=defaultString.replace("<!--searched text-->","");

				QTextDocument t;
				t.setHtml(defaultString);
				QString actualString = t.toPlainText();

				if(actualString.trimmed().size()==0) continue;

				if(actualString.contains(searchText,Qt::CaseInsensitive))
				{
					QString tempStr="<!--searched text-->"+defaultString;
					ui.tblEventRetriever->item(row,SPEAKER_COUNT)->setText(tempStr);
				}
				else
				{
					ui.tblEventRetriever->item(row,SPEAKER_COUNT)->setText(defaultString);
				}
			}

			{
				QString defaultString = ui.tblEventRetriever->item(row,NOTE_COUNT)->text();
				defaultString=defaultString.replace("<!--searched text-->","");

				QTextDocument t;
				t.setHtml(defaultString);
				QString actualString = t.toPlainText();

				if(actualString.trimmed().size()==0) continue;

				if(actualString.contains(searchText,Qt::CaseInsensitive))
				{
					QString tempStr="<!--searched text-->"+defaultString;
					ui.tblEventRetriever->item(row,NOTE_COUNT)->setText(tempStr);
				}
				else
				{
					ui.tblEventRetriever->item(row,NOTE_COUNT)->setText(defaultString);
				}
			} 	
		}
	}
}
void EventRetriever::searchTxtReplace(QString replaceTxt,QString search_)
{

	QString searchText=search_;

	if(searchText!="")
	{ 
		for(int row=0;row<ui.tblEventRetriever->rowCount();row++)
		{
			{
				QString speakerString = ui.tblEventRetriever->item(row,SPEAKER_COUNT)->text();
				QString defaultString = ui.tblEventRetriever->item(row,SPEAKER_COUNT)->text();
				defaultString=defaultString.replace("<!--searched text-->","");

				QTextDocument t;
				t.setHtml(defaultString);
				QString actualString = t.toPlainText();

				if(actualString.trimmed().size()==0) continue;

				if(actualString.contains(searchText,Qt::CaseInsensitive))
				{
					QString tempStr=speakerString.replace(searchText,replaceTxt);
					ui.tblEventRetriever->item(row,SPEAKER_COUNT)->setText(tempStr);
				}
				else
				{
					ui.tblEventRetriever->item(row,SPEAKER_COUNT)->setText(defaultString);
				}
			}

			{
				QString NoteString = ui.tblEventRetriever->item(row,NOTE_COUNT)->text();
				QString defaultString = ui.tblEventRetriever->item(row,NOTE_COUNT)->text();
				defaultString=defaultString.replace("<!--searched text-->","");

				QTextDocument t;
				t.setHtml(defaultString);
				QString actualString = t.toPlainText();

				if(actualString.trimmed().size()==0) continue;

				if(actualString.contains(searchText,Qt::CaseInsensitive))
				{
					QString tempStr=NoteString.replace(searchText,replaceTxt);
					ui.tblEventRetriever->item(row,NOTE_COUNT)->setText(tempStr);
				}
				else
				{
					ui.tblEventRetriever->item(row,NOTE_COUNT)->setText(defaultString);
				}
			} 	
		}
	}
}
void EventRetriever::insertAfterRow(int nrow)
{
	ui.tblEventRetriever->insertRow(nrow+1);
	ui.tblEventRetriever->activate(nrow+1);
}
void EventRetriever::insertBeforeRow(int nrow)
{
	ui.tblEventRetriever->insertRow(nrow);
	ui.tblEventRetriever->activate(nrow);

}
void EventRetriever::deleteFromRow(int noteid_)
{
	EventNoteUtil *eutil=new EventNoteUtil(this);

	try
	{
		eutil->deleteEventNote(noteid_,logSession->getDriverName());
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
}
void EventRetriever::keyPressEvent(QKeyEvent *e_)
{
	if (e_->modifiers ()== Qt::ControlModifier)
	{
		if (e_->key()==Qt::Key_X)
		{
// 			QList<QTableWidgetSelectionRange> selectedList=getSelectedColumn();
// 			QTableWidgetSelectionRange range = selectedRows.first();
// 			QString str;
// 			for (int i=0;i<range.rowCount();++i) 
// 			{
// 				if (i>0)
// 					str=str+"#";
// 				for (int j= 0;j<range.columnCount();++j)
// 				{
// 					if (j>0)
// 						str=str+"@";
// 					str +=ui.tblEventRetriever->item(range.topRow() + i,range.leftColumn() + j)->text();
// 				}
// 			}
// 			QApplication::clipboard()->setText(str);
// 
// 			for (int i=0;i<range.rowCount();++i) 
// 			{
// 				ui.tblEventRetriever->removeRow(range.topRow() + i);
// 			}

			copyStringStatus=false;
			QString str;
			QList<QStringList> selectedRows;
			QList<int> selectedIndex;
			for(int i=0;i<ui.tblEventRetriever->rowCount();i++)
			{
				if(ui.tblEventRetriever->item(i,1)->isSelected())
				{
					QStringList tmpList;
					tmpList<<ui.tblEventRetriever->item(i,0)->text();
					tmpList<<ui.tblEventRetriever->item(i,1)->text();
					tmpList<<ui.tblEventRetriever->item(i,2)->text();
					tmpList<<ui.tblEventRetriever->item(i,3)->text();
					tmpList<<ui.tblEventRetriever->item(i,4)->text();
					selectedRows<<tmpList;
					selectedIndex<<i;
				}
			}
			selectedRowsCount=selectedRows.count();
			int listCount=0;
			foreach(QStringList temp,selectedRows)
			{
				if (listCount>0)
				{
				str=str+"#";
				}
				for (int i=0;i<temp.count();i++)
				{
					
					if (i>0)
					{
						str=str+"@";
					}
					str +=temp[i];

				}
				listCount++;
			}
			QApplication::clipboard()->setText(str);

			for (int i=0;i<selectedIndex.count();i++) 
			{
				if (i==0)
				{
					ui.tblEventRetriever->removeRow(selectedIndex.at(i));
				}
				else
				{
					ui.tblEventRetriever->removeRow(selectedIndex.at(i)-1);
				}	
			}
		}
	}

	if (e_->modifiers ()== Qt::ControlModifier)
	{
		if (e_->key()==Qt::Key_C)
		{
			copyStringStatus=false;
			QString str;
			QList<QStringList> selectedRows;
			QList<int> selectedIndex;
			for(int i=0;i<ui.tblEventRetriever->rowCount();i++)
			{
				if(ui.tblEventRetriever->item(i,1)->isSelected())
				{
					QStringList tmpList;
					tmpList<<ui.tblEventRetriever->item(i,0)->text();
					tmpList<<ui.tblEventRetriever->item(i,1)->text();
					tmpList<<ui.tblEventRetriever->item(i,2)->text();
					tmpList<<ui.tblEventRetriever->item(i,3)->text();
					tmpList<<ui.tblEventRetriever->item(i,4)->text();
					selectedRows<<tmpList;
					selectedIndex<<i;
				}
			}
			selectedRowsCount=selectedRows.count();
			int listCount=0;
			foreach(QStringList temp,selectedRows)
			{
				if (listCount>0)
				{
					str=str+"#";
				}
				for (int i=0;i<temp.count();i++)
				{

					if (i>0)
					{
						str=str+"@";
					}
					str +=temp[i];

				}
				listCount++;
			}
			QApplication::clipboard()->setText(str);

		}
	}

	if (e_->modifiers ()== Qt::ControlModifier)
	{
		if (e_->key()==Qt::Key_V)
		{
// 			QList<QTableWidgetSelectionRange> selectedList=getSelectedColumn();
// 
// 			QTableWidgetSelectionRange range = selectedList.first();
// 			QString str = QApplication::clipboard()->text();
// 			QStringList rows = str.split('#');
// 			int numRows = rows.count();
// 			int numColumns = rows.first().count('@') + 1;
// 			if (range.rowCount() * range.columnCount() != 1 && (range.rowCount()!= numRows|| range.columnCount() != numColumns)) 
// 			{
// 				QMessageBox::information(this, tr("ERROR"),
// 				tr("The information cannot be pasted because the copy "
// 				"and paste areas aren't the same size."));
// 				return;
// 			}
// 			for (int i=0;i<numRows;++i) 
// 			{
// 				QStringList columns = rows[i].split('@');
// 				for (int j=0; j<numColumns;++j) 
// 				{
// 					int row = range.topRow() + i;
// 					int column = range.leftColumn() + j;
// 					ui.tblEventRetriever->item(row,column)->setText(columns[j]);	
// 				}
// 			}

			QList<QStringList> selectedRows;
			QList<int> selectedIndex;
			for(int i=0;i<ui.tblEventRetriever->rowCount();i++)
			{
				if(ui.tblEventRetriever->item(i,1)->isSelected())
				{
					QStringList tmpList;
					tmpList<<ui.tblEventRetriever->item(i,0)->text();
					tmpList<<ui.tblEventRetriever->item(i,1)->text();
					tmpList<<ui.tblEventRetriever->item(i,2)->text();
					tmpList<<ui.tblEventRetriever->item(i,3)->text();
					tmpList<<ui.tblEventRetriever->item(i,4)->text();
					selectedRows<<tmpList;
					selectedIndex<<i;
				}
			}
			if (selectedRows.count()==0)
			{
				QMessageBox::information(this, tr("ERROR"),
					tr("Please Click the row to paste the information"));
				return;
			}
			if (selectedRows.count()!=selectedRowsCount)
			{
				QMessageBox::information(this, tr("ERROR"),
				tr("The information cannot be pasted because the copy "
				"and paste areas aren't the same size."));
				return;
			}
			QString str = QApplication::clipboard()->text();
			QStringList rows = str.split('#');
			int numRows = rows.count();
			int numColumns = rows.first().count('@') + 1;
			for (int i=0;i<numRows;i++) 
			{
				QStringList columns = rows[i].split('@');
				for (int j=0; j<numColumns;j++) 
				{

					int row=selectedIndex.at(i);
					ui.tblEventRetriever->item(row,j)->setText(columns[j]);	
				}
			}
		}
	}
}
void EventRetriever::actionText_Highlight_Clicked()
{
	emit textHighlightColor();
}
EventRetriever::~EventRetriever()
{

}
