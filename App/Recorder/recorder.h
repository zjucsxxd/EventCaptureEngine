#ifndef RECORDER_H
#define RECORDER_H

#include <QtGui/QMainWindow>
#include "ui_recorder.h"

class Recorder : public QMainWindow
{
	Q_OBJECT

public:
	Recorder(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Recorder();

private:
	Ui::RecorderClass ui;
};

#endif // RECORDER_H
