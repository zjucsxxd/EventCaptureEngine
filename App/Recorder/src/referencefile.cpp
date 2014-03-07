/*Owner & Copyrights: Vance King Saxbe. A.*//* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "referencefile.h"
#include "eventeditor.h"
#include "eventretriever.h"
#include "filesdetails.h"
#include <QFile>
#include <QFileDialog>

ReferenceFile::ReferenceFile(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.btnOk, SIGNAL(clicked()),this,SLOT(btnOk_Clicked()));
	connect(ui.btnBrowse, SIGNAL(clicked()),this,SLOT(btnBrowse_Clicked()));
	ui.btnOk->setEnabled(false);
}
void ReferenceFile::btnOk_Clicked()
{
	FilesDetails *files=static_cast<FilesDetails*>(this->parent());

	if (tabIndex==0)
	{	
		EventEditor *editor=new EventEditor(this);
		editor->attachFile(ui.txtFilePath->text(),ui.txtSize->text(),ui.txtFileType->text(),currentRowTime);
	}
	else
	{
		EventRetriever *retriever=new EventRetriever(this);
		retriever->attachFile(ui.txtFilePath->text(),ui.txtSize->text(),ui.txtFileType->text(),currentRowTime);
	}

	files->loadFileDetails(ui.txtFilePath->text(),ui.txtSize->text(),ui.txtFileType->text());

	ui.btnOk->setEnabled(false);
	
}
void ReferenceFile::btnBrowse_Clicked()
{
	QString path = QFileDialog::getOpenFileName(this, tr("Open File"),"/home","Text files (*.txt *.pdf *.doc *.docx *.xlsx *.xls);;"
	"Video files (*.vob *.wmv *.xvid *.mov *.mpg *.flv *.dvx *.avi *.3gp);;"
	"Audio files (*.aac *.mid *.mp3 *.wav *.wma);;"
	"Any files (*)");

	ui.txtFilePath->setText(path);

	QStringList tmpList=path.split("/");
	QString filetxt=tmpList.last();
	QStringList fList=filetxt.split(".");
	QString ftxt=fList.last();

	if (ftxt=="txt")
	{
		ui.txtFileType->setText("Notepad File");
	}
	if (ftxt=="pdf")
	{
		ui.txtFileType->setText("PDF Presentation");
	}
	if (ftxt=="doc" || ftxt=="docx")
	{
		ui.txtFileType->setText("Word File");
	}
	if (ftxt=="xls" || ftxt=="xlsx")
	{
		ui.txtFileType->setText("ExcelSheet File");
	}
	if (ftxt=="vob" || ftxt=="wmv"  || ftxt=="xvid" || ftxt=="mov" || ftxt=="mpg" || ftxt=="flv" ||ftxt=="dvx" || ftxt=="avi" || ftxt=="3gp")
	{
		ui.txtFileType->setText("Video File");
	}
	if (ftxt=="aac" || ftxt=="mid" || ftxt=="mp3" || ftxt=="wav" ||ftxt=="wma")
	{
		ui.txtFileType->setText("Audio File");
	}

	QFile file(path);
	int fileSize=file.size()/1024;

	ui.txtSize->setText(QString::number(fileSize)+" KB");

	ui.btnOk->setEnabled(true);
}

void ReferenceFile::setTabIndex(int index_)
{
	tabIndex=index_;
	exec();
}
void ReferenceFile::setCurrentTime(QString RowTime_)
{
	currentRowTime=RowTime_;
}
ReferenceFile::~ReferenceFile()
{

}
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/