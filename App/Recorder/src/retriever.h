#ifndef RETRIEVER_H
#define RETRIEVER_H

#include <QWidget>
#include <QTableWidget>
#include <QIcon>

#include "eventnoteinfo.h"

class Retriever : public QTableWidget
{
	Q_OBJECT

public:
	Retriever(QWidget *parent);
	~Retriever();

	void insertRow(int);
	void activate(int);
	void clear();
	void insertRows(QList<EventNoteInfo>);
	void insertImportRows(QList<EventNoteInfo>);

private:

	QString dayTimeNow();
	QIcon fixedIco,currentIco;

private slots:
	void updateRowHeader(int,int);
	void tblCellChanged();

};

#endif // RETRIEVER_H
