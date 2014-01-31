#include "referenceimage.h"
#include "eventeditor.h"
#include "eventretriever.h"
#include "imagedetails.h"
#include <QFileDialog>
#include <QLabel>
#include <QImage>

ReferenceImage::ReferenceImage(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.btnOk, SIGNAL(clicked()),this,SLOT(btnOk_Clicked()));
	connect(ui.btnBrowse, SIGNAL(clicked()),this,SLOT(btnBrowse_Clicked()));
	ui.btnOk->setEnabled(false);

	QLabel *imageLabel = new QLabel(this);
	QImage image(":/Recorder/Resources/no-image.jpg");
	imageLabel->setPixmap(QPixmap::fromImage(image));
	imageLabel->setAlignment(Qt::AlignCenter);

	ui.scrollArea->setBackgroundRole(QPalette::Shadow);
	ui.scrollArea->setWidget(imageLabel);

}

void ReferenceImage::btnOk_Clicked()
{
	ImageDetails *img=static_cast<ImageDetails*>(this->parent());

	if (tabIndex==0)
	{	
		EventEditor *editor=new EventEditor(this);
		editor->attachFile(ui.txtPath->text(),ui.txtSize->text(),ui.txtType->text(),referenceTime);
	}
	else
	{
		EventRetriever *retriever=new EventRetriever(this);
		retriever->attachFile(ui.txtPath->text(),ui.txtSize->text(),ui.txtType->text(),referenceTime);
	}

	img->loadFileDetails(ui.txtPath->text(),ui.txtSize->text(),ui.txtType->text());

	ui.btnOk->setEnabled(false);

}
void ReferenceImage::btnBrowse_Clicked()
{
	QString path = QFileDialog::getOpenFileName(this, tr("Open File"),"/home",
				   "Image files (*.jpg *.jpeg *.png *.bmp *.gif *.tif);;");

	ui.txtPath->setText(path);

	QString imagepath;

	imagepath=ui.txtPath->text().replace("/","\\");

	QLabel *imageLabel = new QLabel(this);
	QImage image(imagepath);
	imageLabel->setPixmap(QPixmap::fromImage(image));
	imageLabel->setAlignment(Qt::AlignCenter);

	ui.scrollArea->setBackgroundRole(QPalette::Dark);
	ui.scrollArea->setWidget(imageLabel);

	QFile file(path);
	int fileSize=file.size()/1024;

	ui.txtSize->setText(QString::number(fileSize)+" KB");

	QStringList tmpList=path.split("/");
	QString filetxt=tmpList.last();
	QStringList fList=filetxt.split(".");
	QString ftxt=fList.last();

	if (ftxt=="gif")
	{
		ui.txtType->setText("Animation File");
	}
	else
	{
		ui.txtType->setText("Image File");
	}

	ui.btnOk->setEnabled(true);
}

void ReferenceImage::setTabIndex(int index_)
{
	tabIndex=index_;
	exec();
}
void ReferenceImage::setReferenceTime(QString time_)
{
	referenceTime=time_;
}
ReferenceImage::~ReferenceImage()
{

}
