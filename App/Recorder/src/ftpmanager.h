/*Owner & Copyrights: Vance King Saxbe. A.*/#ifndef FTPMANAGER_H
#define FTPMANAGER_H

#include <QDialog>
#include "ui_ftpmanager.h"

#include <QtNetwork>
#include <QMessageBox>
#include <QHash>
#include <QDebug>
#include <QHeaderView>
#include <QProgressDialog>
#include <QKeyEvent>
#include <QFtp>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QTimer>

struct ftpDetails
{
	QString hostName;
	QString userName;
	QString password;
	int portNo;
};

class FTPManager : public QDialog
{
	Q_OBJECT

public:
	FTPManager(QWidget *parent = 0);
	~FTPManager();

	void refresh(ftpDetails);

	QHash<QString, bool> isDirectory;
	QString currentPath;
	QFtp *ftp;
	QFile *file;
	int num;
	int totalFolders;
	QStringList folderFiles;
	QString forMkdir;
	QString folderUploading;
	QProgressDialog *progressDialog;
	QSize sizeHint()const;
	QString fileName;
	QTimer *timerUp;
	QFileInfo UploadFileInfo;
	void folderNameUp(QString);
	bool connectToServer();
	QString lastError();
	//bool copy(QString);
private:
	void closeEvent(QCloseEvent *event);

signals :
	void hideFTPbutton();

protected:
	void keyPressEvent(QKeyEvent *e);

	private slots:
		int connectOrDisconnect();
		void downloadFile();
		void cancelDownload();
		void ftpCommandFinished(int commandId, bool error);
		void addToList(const QUrlInfo &urlInfo);
		void processItem(QTreeWidgetItem *item, int column);
		void cdToParent();
		void Upload();
		void updateDataTransferProgress(qint64 readBytes,qint64 totalBytes);
		void ftpdone( bool error);
		void enableDownloadButton();
		void uploadNext();
		void deleteFileFromFTP();

private:
	Ui::FTPManagerClass ui;

	void reload();
};

#endif // FTPMANAGER_H
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/