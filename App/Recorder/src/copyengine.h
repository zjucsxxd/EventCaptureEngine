#ifndef COPYENGINE_H
#define COPYENGINE_H

#include <QThread>

class CopyEngine : public QThread
{
	Q_OBJECT

public:
	CopyEngine(QObject *parent);
	~CopyEngine();

	void setSrcFile(QString);
	void setDstFile(QString);
	int getProgress();
	int getStatus();
	QString lastError();
	void setExitStatus(bool);

	void run()
	{
		progress=0;
		status=0;
		errorString="";

		copyFile(srcFile,dstFile);

		srcFile="";
		dstFile="";
	}

private:
	QString srcFile,dstFile;
	QString errorString;
	int progress,status;
	bool exitStatus;
	void copyFile(QString,QString);

};

#endif // COPYENGINE_H