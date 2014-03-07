/*Owner & Copyrights: Vance King Saxbe. A.*/#ifndef IMAGEPREVIEW_H
#define IMAGEPREVIEW_H

#include <QDialog>
#include "ui_imagepreview.h"
#include "loginusersession.h"
#include "sqlitedatabase.h"

class ImagePreview : public QDialog
{
	Q_OBJECT

public:
	ImagePreview(QWidget *parent = 0);
	~ImagePreview();
	void setPreview(int,QString,QString,QString);

private:

	Ui::ImagePreviewClass ui;
	LoginUserSession *logSession;
	SQLiteDataBase *tempDB;
};

#endif // IMAGEPREVIEW_H
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/