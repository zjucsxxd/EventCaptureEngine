/*Owner & Copyrights: Vance King Saxbe. A.*/#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QDialog>
#include "ui_filedownloader.h"

#include "loginusersession.h"

#include <QIcon>

class FileDownloader : public QDialog
{
	Q_OBJECT

public:
	FileDownloader(QWidget *parent = 0);
	~FileDownloader();

	void refreshDialog(int);

private:
	Ui::FileDownloaderClass ui;

	LoginUserSession *logSession;
	int currentEvent;
	bool uiClosed;

	QIcon icoWordDoc,icoXls,icoPdfDoc,icoTextDoc,icoImage,icoVideo,icoAudio,icoUnknown;

private slots:
	void btnDownload_click();
	void downloadListWidget_click();
	void downloadListWidget_doubleclick();
	void btnDelete_click();
	void btnClose_click();
};

#endif // FILEDOWNLOADER_H
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/