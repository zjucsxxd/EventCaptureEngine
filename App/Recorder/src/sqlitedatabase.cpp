/*Owner & Copyrights: Vance King Saxbe. A.*//* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "sqlitedatabase.h"

#include "fileutil.h"
#include "eventnoteutil.h"
#include "eventutil.h"
#include "sessionutil.h"
#include "attachedfileutil.h"
#include "referencebookutil.h"
#include "referencelinkutil.h"
#include <QDir>
#include <QFile>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QTextDocument>

SQLiteDataBase::SQLiteDataBase()
{
	logSession=LoginUserSession::session();
}

bool SQLiteDataBase::connectDatabase()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","Inst_SQLLite");
	db.setHostName("DLCRS");
	db.setDatabaseName(QDir::homePath()+"/CRSTempData.Db");
	db.setUserName("softnotions");
	db.setPassword("~s0ftn0ti0ns~");
	PRINT_MESSAGE("local Databasefile path is " + QDir::homePath() + "/CRSTempData.Db");

	if(!db.open())
	{
		PRINT_ERROR(db.lastError().text());
		return false;
	}

	return true;
}
bool SQLiteDataBase::checkDataExistence()
{
	if (detailsTableExistence()!=false)
	{
		QSqlDatabase db= QSqlDatabase::database("Inst_SQLLite");

		QSqlQuery sqlData(db);

		sqlData.prepare("SELECT * FROM details");

		bool status=sqlData.exec();
		if (status=true && sqlData.next())
		{
			return true;
		}
		else 
		{
			QSqlQuery sqlClearNotes(db);
			sqlClearNotes.exec("delete from notes");

			QSqlQuery sqlClearreferenceFile(db);
			sqlClearreferenceFile.exec("delete from referenceFile");

			QSqlQuery sqlClearreferenceBooks(db);
			sqlClearreferenceBooks.exec("delete from referenceBooks");

			QSqlQuery sqlClearreferenceLinks(db);
			sqlClearreferenceLinks.exec("delete from referenceLinks");
			return false;
		}
	}
	else
	{
		return false;
	}
}
bool SQLiteDataBase::detailsTableExistence()
{
	bool status;

	QSqlDatabase db= QSqlDatabase::database("Inst_SQLLite");

	QSqlQuery sqlExistence(db);

	sqlExistence.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name='details'");

	if (!sqlExistence.exec())
	{
		PRINT_ERROR("unable to check table existence!!!");
		status=false;
	}
	else
	{
		if (sqlExistence.next())
		{
			status=true;
		}
		else
		{
			status=false;
		}
	}

	return status;
}
bool SQLiteDataBase::notesTableExistence()
{
	bool status;

	QSqlDatabase db= QSqlDatabase::database("Inst_SQLLite");

	QSqlQuery sqlExistence(db);

	sqlExistence.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name='notes'");

	if (!sqlExistence.exec())
	{
		PRINT_ERROR("unable to check table existence!!!");
		status=false;
	}
	else
	{
		if (sqlExistence.next())
		{
			status=true;
		}
		else
		{
			status=false;
		}
	}

	return status;
}
bool SQLiteDataBase::refFileTableExistence()
{
	bool status;

	QSqlDatabase db= QSqlDatabase::database("Inst_SQLLite");

	QSqlQuery sqlExistence(db);

	sqlExistence.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name='referenceFile'");

	if (!sqlExistence.exec())
	{
		PRINT_ERROR("unable to check table existence!!!");
		status=false;
	}
	else
	{
		if (sqlExistence.next())
		{
			status=true;
		}
		else
		{
			status=false;
		}
	}

	return status;
}
bool SQLiteDataBase::refBookTableExistence()
{
	bool status;

	QSqlDatabase db= QSqlDatabase::database("Inst_SQLLite");

	QSqlQuery sqlExistence(db);

	sqlExistence.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name='referenceBooks'");

	if (!sqlExistence.exec())
	{
		PRINT_ERROR("unable to check table existence!!!");
		status=false;
	}
	else
	{
		if (sqlExistence.next())
		{
			status=true;
		}
		else
		{
			status=false;
		}
	}

	return status;
}
bool SQLiteDataBase::refLinkTableExistence()
{
	bool status;

	QSqlDatabase db= QSqlDatabase::database("Inst_SQLLite");

	QSqlQuery sqlExistence(db);

	sqlExistence.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name='referenceLinks'");

	if (!sqlExistence.exec())
	{
		PRINT_ERROR("unable to check table existence!!!");
		status=false;
	}
	else
	{
		if (sqlExistence.next())
		{
			status=true;
		}
		else
		{
			status=false;
		}
	}

	return status;
}
bool SQLiteDataBase::createTempTables()
{
	bool status=true;

	QSqlDatabase db= QSqlDatabase::database("Inst_SQLLite");

	if (detailsTableExistence()!=true)
	{
		QSqlQuery sqlCreateDetails(db);

		sqlCreateDetails.prepare("CREATE TABLE details"
			"(eventid INTEGER,"
			"pcid INTEGER,"
			"eventtypeid INTEGER,"
			"sessionid INTEGER,"
			"userid INTEGER,"
			"roomid INTEGER);");

		if(!sqlCreateDetails.exec())
		{	
			PRINT_ERROR("unable to create temporary table 'details' in SQLite Database!!!"+sqlCreateDetails.lastError().text());		
			status=false;
		}	
	}
	

	if(notesTableExistence()!=true)
	{
		QSqlQuery sqlCreateNotes(db);

		sqlCreateNotes.prepare("CREATE TABLE notes"
			"(EventTime TEXT,"
			"RichEventSpeaker TEXT,"
			"RichEventNote TEXT,"
			"FilePath TEXT,"
			"SplitStatus INTEGER,"
			"InternalReference TEXT,"
			"ExternalReference TEXT,"
			"Visibility INTEGER);");

		if(!sqlCreateNotes.exec())
		{	
			PRINT_ERROR("unable to create temporary table 'notes' in SQLite Database!!!"+sqlCreateNotes.lastError().text());
			status=false;
		}	
	}

	if(refFileTableExistence()!=true)
	{
		QSqlQuery sqlCreaterefFile(db);

		sqlCreaterefFile.prepare("CREATE TABLE referenceFile"
			"(AttachedPath TEXT,"
			"AttachedHash TEXT,"
			"AttachedSize TEXT,"
			"AttachedType TEXT,"
			"UploadStatus INTEGER,"
			"CurrentTime TEXT);");

		if(!sqlCreaterefFile.exec())
		{	
			PRINT_ERROR("unable to create temporary table 'notes' in SQLite Database!!!"+sqlCreaterefFile.lastError().text());
			status=false;
		}	
	}
	
	if(refBookTableExistence()!=true)
	{
		QSqlQuery sqlCreaterefFile(db);

		sqlCreaterefFile.prepare("CREATE TABLE referenceBooks"
			"(BookName TEXT,"
			"AuthorName TEXT,"
			"BookDescription TEXT,"
			"Edition TEXT,"
			"CurrentTime TEXT);");

		if(!sqlCreaterefFile.exec())
		{	
			PRINT_ERROR("unable to create temporary table 'notes' in SQLite Database!!!"+sqlCreaterefFile.lastError().text());
			status=false;
		}	
	}
	if(refLinkTableExistence()!=true)
	{
		QSqlQuery sqlCreaterefFile(db);

		sqlCreaterefFile.prepare("CREATE TABLE referenceLinks"
			"(URL TEXT,"
			"URLDescription TEXT,"
			"CurrentTime TEXT);");

		if(!sqlCreaterefFile.exec())
		{	
			PRINT_ERROR("unable to create temporary table 'notes' in SQLite Database!!!"+sqlCreaterefFile.lastError().text());
			status=false;
		}	
	}
	return status;
}
bool SQLiteDataBase::clearTempTables()
{
	bool status=true;

	QSqlDatabase db= QSqlDatabase::database("Inst_SQLLite");

	if (detailsTableExistence()==true)
	{
		QSqlQuery sqlClearDetails(db);

		sqlClearDetails.prepare("DELETE FROM details");

		if(!sqlClearDetails.exec())
		{	
			PRINT_ERROR("unable to clear temporary table 'details' in SQLite Database!!!"+sqlClearDetails.lastError().text());		
			status=false;
		}	
	}


	if(notesTableExistence()==true)
	{
		QSqlQuery sqlClearNotes(db);

		sqlClearNotes.prepare("DELETE FROM notes");

		if(!sqlClearNotes.exec())
		{	
			PRINT_ERROR("unable to clear temporary table 'notes' in SQLite Database!!!"+sqlClearNotes.lastError().text());
			status=false;
		}	
	}

	if(refFileTableExistence()==true)
	{
		QSqlQuery sqlClearFile(db);

		sqlClearFile.prepare("DELETE FROM referenceFile");

		if(!sqlClearFile.exec())
		{	
			PRINT_ERROR("unable to clear temporary table 'notes' in SQLite Database!!!"+sqlClearFile.lastError().text());
			status=false;
		}	
	}

	if(refBookTableExistence()==true)
	{
		QSqlQuery sqlClearBooks(db);

		sqlClearBooks.prepare("DELETE FROM referenceBooks");

		if(!sqlClearBooks.exec())
		{	
			PRINT_ERROR("unable to clear temporary table 'notes' in SQLite Database!!!"+sqlClearBooks.lastError().text());
			status=false;
		}	
	}
	if(refLinkTableExistence()==true)
	{
		QSqlQuery sqlClearLinks(db);

		sqlClearLinks.prepare("DELETE FROM referenceLinks");

		if(!sqlClearLinks.exec())
		{	
			PRINT_ERROR("unable to clear temporary table 'notes' in SQLite Database!!!"+sqlClearLinks.lastError().text());
			status=false;
		}	
	}

	return status;
}
bool SQLiteDataBase::startCase(tempDetails details_,tempNotes notes_,QString path_,int split_)
{

	QSqlDatabase db= QSqlDatabase::database("Inst_SQLLite");

	QSqlQuery sqlInsertDetails(db);

	sqlInsertDetails.prepare("INSERT INTO details values"
		"(:eventid,"
		":pcid,"
		":eventtypeid ,"
		":sessionid,"
		":userid,"
		":roomid)");

	sqlInsertDetails.bindValue(":eventid",details_.eventid);
	sqlInsertDetails.bindValue(":pcid",details_.pcid);
	sqlInsertDetails.bindValue(":eventtypeid ",details_.eventtypeid);
	sqlInsertDetails.bindValue(":sessionid",details_.sessionid);
	sqlInsertDetails.bindValue(":userid",details_.userid);
	sqlInsertDetails.bindValue(":roomid",details_.roomid);


	if(!sqlInsertDetails.exec())
	{		
		PRINT_ERROR("unable to insert details in SQLite Database!!!"+sqlInsertDetails.lastError().text());
		return false;
	}	
	
	if(!insertNotes(notes_,path_,split_)) return false;

	return true;
}
bool SQLiteDataBase::insertNotes(tempNotes notes_,QString filePath,int split_)
{
	QSqlDatabase db= QSqlDatabase::database("Inst_SQLLite");
	
	QSqlQuery sqlInsertNotes(db);
	
	sqlInsertNotes.prepare("INSERT INTO notes values"
		"(:EventTime,"
		":RichEventSpeaker,"
		":RichEventNote,"
		":FilePath,"
		":SplitStatus,"
		":InternalReference,"
		":ExternalReference,"
		":Visibility)");

	sqlInsertNotes.bindValue(":EventTime",notes_.eventTime);
	sqlInsertNotes.bindValue(":RichEventSpeaker",notes_.richEventSpeaker);
	sqlInsertNotes.bindValue(":RichEventNote",notes_.richEventNotes);
	sqlInsertNotes.bindValue(":FilePath",filePath);
	sqlInsertNotes.bindValue(":SplitStatus",split_);
	sqlInsertNotes.bindValue(":InternalReference",notes_.internalReference);
	sqlInsertNotes.bindValue(":ExternalReference",notes_.externalReference);
	sqlInsertNotes.bindValue(":Visibility",notes_.visibility);

	if(!sqlInsertNotes.exec())
	{		
		PRINT_ERROR("unable to insert notes in SQLite Database!!!"+sqlInsertNotes.lastError().text());
		return false;
	}	
	
	return true;
}
bool SQLiteDataBase::insertReferenceFile(tempReferenceFile rfile_)
{
	QSqlDatabase db= QSqlDatabase::database("Inst_SQLLite");

	QSqlQuery sqlInsertReferenceFile(db);

	sqlInsertReferenceFile.prepare("INSERT INTO referenceFile values"
		"(:AttachedPath,"
		":AttachedHash,"
		":AttachedSize,"
		":AttachedType,"
		":UploadStatus,"
		":CurrentTime)");

	sqlInsertReferenceFile.bindValue(":AttachedPath",rfile_.attachedPath);
	sqlInsertReferenceFile.bindValue(":AttachedHash",rfile_.attachedHash);
	sqlInsertReferenceFile.bindValue(":AttachedSize",rfile_.attachedSize);
	sqlInsertReferenceFile.bindValue(":AttachedType",rfile_.attachedType);
	sqlInsertReferenceFile.bindValue(":UploadStatus",rfile_.uploadStatus);
	sqlInsertReferenceFile.bindValue(":CurrentTime",rfile_.currentTime);
	if(!sqlInsertReferenceFile.exec())
	{		
		PRINT_ERROR("unable to insert notes in SQLite Database!!!"+sqlInsertReferenceFile.lastError().text());
		return false;
	}	
	return true;
}
bool SQLiteDataBase::insertReferenceBook(tempReferenceBook rbook_)
{
	QSqlDatabase db= QSqlDatabase::database("Inst_SQLLite");

	QSqlQuery sqlInsertReferenceBook(db);

	sqlInsertReferenceBook.prepare("INSERT INTO referenceBooks values"
		"(:BookName,"
		":AuthorName,"
		":BookDescription,"
		":Edition,"
		":CurrentTime)");

	sqlInsertReferenceBook.bindValue(":BookName",rbook_.bookname);
	sqlInsertReferenceBook.bindValue(":AuthorName",rbook_.author);
	sqlInsertReferenceBook.bindValue(":BookDescription",rbook_.bookdescription);
	sqlInsertReferenceBook.bindValue(":Edition",rbook_.edition);
	sqlInsertReferenceBook.bindValue(":CurrentTime",rbook_.bcurrentTime);
	if(!sqlInsertReferenceBook.exec())
	{		
		PRINT_ERROR("unable to insert notes in SQLite Database!!!"+sqlInsertReferenceBook.lastError().text());
		return false;
	}	
	return true;
}
bool SQLiteDataBase::insertReferenceLink(tempReferenceLink rlink_)
{
	QSqlDatabase db= QSqlDatabase::database("Inst_SQLLite");

	QSqlQuery sqlInsertReferenceLink(db);

	sqlInsertReferenceLink.prepare("INSERT INTO referenceLinks values"
		"(:URL,"
		":URLDescription,"
		":CurrentTime)");

	sqlInsertReferenceLink.bindValue(":URL",rlink_.rlink);
	sqlInsertReferenceLink.bindValue(":URLDescription",rlink_.rdescription);
	sqlInsertReferenceLink.bindValue(":CurrentTime",rlink_.rcurrentTime);
	if(!sqlInsertReferenceLink.exec())
	{		
		PRINT_ERROR("unable to insert notes in SQLite Database!!!"+sqlInsertReferenceLink.lastError().text());
		return false;
	}	
	return true;
}
bool SQLiteDataBase::updateNotes(tempNotes notes_)
{
	QSqlDatabase db= QSqlDatabase::database("Inst_SQLLite");

	QSqlQuery sqlInsertNotes(db);

	sqlInsertNotes.prepare("UPDATE notes SET "
		"RichEventSpeaker=:RichEventSpeaker,"
		"RichEventNote=:RichEventNote,"
		"InternalReference=:InternalReference,"
		"ExternalReference=:ExternalReference"
		"WHERE EventTime=:EventTime AND Visibility=:Visibility");
	
	sqlInsertNotes.bindValue(":EventTime",notes_.eventTime);
	sqlInsertNotes.bindValue(":RichEventSpeaker",notes_.richEventSpeaker);
	sqlInsertNotes.bindValue(":RichEventNote",notes_.richEventNotes);
	sqlInsertNotes.bindValue(":InternalReference",notes_.internalReference);
	sqlInsertNotes.bindValue(":ExternalReference",notes_.externalReference);
	sqlInsertNotes.bindValue(":Visibility",1);

	if(!sqlInsertNotes.exec())
	{		
		PRINT_ERROR("unable to update notes in SQLite Database!!!"+sqlInsertNotes.lastError().text());
		return false;
	}	

	return true;
}

bool SQLiteDataBase::deleteEventNotes(QString evtTime_)
{
	QSqlDatabase db= QSqlDatabase::database("Inst_SQLLite");

	QSqlQuery sqlDelete(db);

	sqlDelete.prepare("DELETE FROM notes WHERE EventTime=:EventTime");
	sqlDelete.bindValue(":EventTime",evtTime_);
	if(!sqlDelete.exec())
	{		
		PRINT_ERROR("unable to Delete notes in SQLite Database!!!"+sqlDelete.lastError().text());
		return false;
	}	
	return true;
}

bool SQLiteDataBase::deleteReferenceFile(QString fname_,QString fsize_,QString ftype_)
{
	QSqlDatabase db= QSqlDatabase::database("Inst_SQLLite");

	QSqlQuery sqlDeleteFile(db);

	sqlDeleteFile.prepare("DELETE FROM referenceFile WHERE AttachedPath=:AttachedPath AND AttachedSize=:AttachedSize AND AttachedType=:AttachedType");
	sqlDeleteFile.bindValue(":AttachedPath",fname_);
	sqlDeleteFile.bindValue(":AttachedSize",fsize_);
	sqlDeleteFile.bindValue(":AttachedType",ftype_);
	if(!sqlDeleteFile.exec())
	{		
		PRINT_ERROR("unable to Delete notes in SQLite Database!!!"+sqlDeleteFile.lastError().text());
		return false;
	}	
	return true;
}
bool SQLiteDataBase::deleteBook(QString bName_,QString bAuthor_,QString bEdition_,QString bDescription_)
{
	QSqlDatabase db= QSqlDatabase::database("Inst_SQLLite");

	QSqlQuery sqlDeleteBook(db);

	sqlDeleteBook.prepare("DELETE FROM referenceBooks WHERE BookName=:BookName AND AuthorName=:AuthorName AND BookDescription=:BookDescription AND Edition=:Edition");
	sqlDeleteBook.bindValue(":BookName",bName_);
	sqlDeleteBook.bindValue(":AuthorName",bAuthor_);
	sqlDeleteBook.bindValue(":BookDescription",bDescription_);
	sqlDeleteBook.bindValue(":Edition",bEdition_);
	if(!sqlDeleteBook.exec())
	{		
		PRINT_ERROR("unable to Delete notes in SQLite Database!!!"+sqlDeleteBook.lastError().text());
		return false;
	}	
	return true;
}

bool SQLiteDataBase::deleteLink(QString url_,QString description_)
{
	QSqlDatabase db= QSqlDatabase::database("Inst_SQLLite");

	QSqlQuery sqlDeleteLink(db);

	sqlDeleteLink.prepare("DELETE FROM referenceLinks WHERE URL=:URL AND URLDescription=:URLDescription ");
	sqlDeleteLink.bindValue(":URL",url_);
	sqlDeleteLink.bindValue(":URLDescription",description_);
	if(!sqlDeleteLink.exec())
	{		
		PRINT_ERROR("unable to Delete notes in SQLite Database!!!"+sqlDeleteLink.lastError().text());
		return false;
	}	
	return true;
}
QStringList SQLiteDataBase::loadReferenceFile(QString fsize_,QString fType_)
{
	QStringList fileList;

	QSqlDatabase db= QSqlDatabase::database("Inst_SQLLite");

	QSqlQuery sqlLoadFile(db);

	sqlLoadFile.prepare("select * from referenceFile WHERE AttachedSize=:AttachedSize AND AttachedType=:AttachedType");
	sqlLoadFile.bindValue(":AttachedSize",fsize_);
	sqlLoadFile.bindValue(":AttachedType",fType_);
	if(!sqlLoadFile.exec())
	{		
		PRINT_ERROR("unable to Delete notes in SQLite Database!!!"+sqlLoadFile.lastError().text());
	
	}	
	while(sqlLoadFile.next())
	{
		QString aFile;
		aFile= sqlLoadFile.value(sqlLoadFile.record().indexOf("AttachedPath")).toString();
		fileList<<aFile;
	}
	return fileList;

}
QList<QStringList> SQLiteDataBase::loadReferenceFile()
{
	QList<QStringList> fileList;

	QSqlDatabase db= QSqlDatabase::database("Inst_SQLLite");

	QSqlQuery sqlLoadFile(db);

	sqlLoadFile.prepare("select * from referenceFile");
	
	if(!sqlLoadFile.exec())
	{		
		PRINT_ERROR("unable to Delete notes in SQLite Database!!!"+sqlLoadFile.lastError().text());

	}	
	while(sqlLoadFile.next())
	{
		QString aFile,aSize,aType;
		QStringList tempList;
		aFile= sqlLoadFile.value(sqlLoadFile.record().indexOf("AttachedPath")).toString();
		aSize= sqlLoadFile.value(sqlLoadFile.record().indexOf("AttachedSize")).toString();
		aType= sqlLoadFile.value(sqlLoadFile.record().indexOf("AttachedType")).toString();
		tempList<<aFile<<aSize<<aType;
		fileList<<tempList;
	}
	return fileList;

}
QList<QStringList> SQLiteDataBase::loadReferenceBook()
{
	QList<QStringList> bookList;

	QSqlDatabase db= QSqlDatabase::database("Inst_SQLLite");

	QSqlQuery sqlLoadBook(db);

	sqlLoadBook.prepare("select * from referenceBooks");

	if(!sqlLoadBook.exec())
	{		
		PRINT_ERROR("unable to Delete notes in SQLite Database!!!"+sqlLoadBook.lastError().text());

	}	
	while(sqlLoadBook.next())
	{
		QString bname;
		QString author;
		QString bdescription,bEdition;
		QStringList tempList;
		bname = sqlLoadBook.value(sqlLoadBook.record().indexOf("BookName")).toString();
		author= sqlLoadBook.value(sqlLoadBook.record().indexOf("AuthorName")).toString();
		bdescription= sqlLoadBook.value(sqlLoadBook.record().indexOf("BookDescription")).toString();
		bEdition= sqlLoadBook.value(sqlLoadBook.record().indexOf("Edition")).toString();

		tempList<<bname<<author<<bEdition<<bdescription;
		bookList<<tempList;
	}
	return bookList;

}

QList<QStringList> SQLiteDataBase::loadReferenceLink()
{
	QList<QStringList> bookLink;

	QSqlDatabase db= QSqlDatabase::database("Inst_SQLLite");

	QSqlQuery sqlLoadLink(db);

	sqlLoadLink.prepare("select * from referenceLinks");

	if(!sqlLoadLink.exec())
	{		
		PRINT_ERROR("unable to Delete notes in SQLite Database!!!"+sqlLoadLink.lastError().text());

	}	
	while(sqlLoadLink.next())
	{
		QString url;
		QString urldescription;
		QStringList tempList;

		url = sqlLoadLink.value(sqlLoadLink.record().indexOf("URL")).toString();
		urldescription= sqlLoadLink.value(sqlLoadLink.record().indexOf("URLDescription")).toString();

		tempList<<url<<urldescription;
		bookLink<<tempList;
	}
	return bookLink;

}
bool SQLiteDataBase::commitData()
{
	int eventid;
	int pcid;
	int eventtypeid;
	int sessionid;
	int userid;
	int roomid;
	int eventNoteID;

	QString prevFilePath="";
	QString prevattachedFile="";
	QString prevbookname="";
	QString prevurl="";

	QSqlDatabase db= QSqlDatabase::database("Inst_SQLLite");
	try
	{
		QSqlQuery sqlDetails(db);

		sqlDetails.prepare("select * from details");

		if(!sqlDetails.exec())
		{
			PRINT_ERROR("unable to retrieve details from SQLite Database!!!"+sqlDetails.lastError().text());
			return false;
		}
		else
		{
			if(sqlDetails.next())
			{
				eventid = sqlDetails.value(sqlDetails.record().indexOf("eventid")).toInt();
				pcid= sqlDetails.value(sqlDetails.record().indexOf("pcid")).toInt() ;
				eventtypeid= sqlDetails.value(sqlDetails.record().indexOf("eventtypeid")).toInt() ;
				sessionid= sqlDetails.value(sqlDetails.record().indexOf("sessionid")).toInt() ;
				userid= sqlDetails.value(sqlDetails.record().indexOf("userid")).toInt() ;
				roomid= sqlDetails.value(sqlDetails.record().indexOf("roomid")).toInt() ;
			}
		}

		QSqlQuery sqlNotes(db);

		QSqlQuery sqlreferenceFile(db);
		QSqlQuery sqlreferenceBook(db);
		QSqlQuery sqlreferenceLink(db);

		sqlNotes.prepare("select * from notes");	

		if(!sqlNotes.exec())
		{
			PRINT_ERROR("unable to retrieve notes from SQLite Database!!!"+sqlNotes.lastError().text());
			return false;
		}
		else
		{
			int fileid;
			QDateTime maxTime;

			while(sqlNotes.next())
			{
				QDateTime eventTime;
				QString richEventSpeaker;
				QString richEventNotes;			
				QString filePath;
				QString iReference,eReference;
				int visibility,splitStatus;

				QString strTime=sqlNotes.value(sqlNotes.record().indexOf("EventTime")).toString();
				eventTime = QDateTime::fromString(strTime,"dd/MM/yyyy hh:mm:ss AP");
				richEventSpeaker = sqlNotes.value(sqlNotes.record().indexOf("RichEventSpeaker")).toString();
				richEventNotes = sqlNotes.value(sqlNotes.record().indexOf("RichEventNote")).toString();
				filePath = sqlNotes.value(sqlNotes.record().indexOf("FilePath")).toString();
				splitStatus = sqlNotes.value(sqlNotes.record().indexOf("SplitStatus")).toInt();
				iReference = sqlNotes.value(sqlNotes.record().indexOf("InternalReference")).toString();
				eReference = sqlNotes.value(sqlNotes.record().indexOf("ExternalReference")).toString();
				visibility = sqlNotes.value(sqlNotes.record().indexOf("Visibility")).toInt();
				
				if(filePath!=prevFilePath)
				{
					FileUtil *fUtil=new FileUtil(this);
					FileInfo fInfo;

					fInfo.setFilePath(filePath);
					fInfo.setFileHash("--");
					fInfo.setCreatedDate(eventTime);		
					fInfo.setEventID(eventid);
					fInfo.setSessionID(sessionid);
					fInfo.setSplitStatus(splitStatus);

					fileid=fUtil->insertFileOrUpdate(fInfo,logSession->getDriverName());
					delete fUtil;
					
					prevFilePath=filePath;
				}			

				EventNoteUtil *enUtil=new EventNoteUtil(this);
				EventNoteInfo enInfo;
				
				enInfo.setEventTime(eventTime);
				enInfo.setEventSpeaker(PlainText(richEventSpeaker));
				enInfo.setEventNote(PlainText(richEventNotes));
				enInfo.setRichSpeaker(richEventSpeaker);
				enInfo.setRichNote(richEventNotes);
				enInfo.setVisibility(visibility);
				enInfo.setEventID(eventid);	
				enInfo.setFileID(fileid);
				enInfo.setRoomID(roomid);
				enInfo.setUserID(userid);
				enInfo.setPCID(pcid);
				enInfo.setSessionID(sessionid);	

				if(eReference=="")
				{
					enInfo.setSessionURL(PlainText("--"));
				}
				else
				{
					enInfo.setSessionURL(eReference);
				}
				if(iReference=="")
				{
					enInfo.setElapsedTime(PlainText("--"));
				}
				else
				{
					enInfo.setElapsedTime(iReference);
				}

				eventNoteID=enUtil->insertOrUpdateEventNote(enInfo,logSession->getDriverName());
				delete enUtil;	

				maxTime=eventTime;

				sqlreferenceFile.prepare("select * from referenceFile where CurrentTime=:CurrentTime");
				sqlreferenceFile.bindValue(":CurrentTime",strTime);

				if(!sqlreferenceFile.exec())
				{
					PRINT_ERROR("unable to retrieve notes from SQLite Database!!!"+sqlreferenceFile.lastError().text());
					return false;
				}
				else
				{
					while(sqlreferenceFile.next())
					{
						QString aFile;
						QString aHash,aSize,aType;
						int uploadstaus;

						aFile= sqlreferenceFile.value(sqlreferenceFile.record().indexOf("AttachedPath")).toString();
						aHash= sqlreferenceFile.value(sqlreferenceFile.record().indexOf("AttachedHash")).toString();
						aSize= sqlreferenceFile.value(sqlreferenceFile.record().indexOf("AttachedSize")).toString();
						aType= sqlreferenceFile.value(sqlreferenceFile.record().indexOf("AttachedType")).toString();
						uploadstaus= sqlreferenceFile.value(sqlreferenceFile.record().indexOf("UploadStatus")).toInt();

						if(aFile!=prevattachedFile)
						{
							AttachedFileUtil *util=new AttachedFileUtil(this);
							AttachedFileInfo info;
							info.setAttachedPath(aFile);
							info.setAttachedhash(aHash);
							info.setAttachedSize(aSize);
							info.setAttachedType(aType);
							info.setUploadStatus(uploadstaus);
							info.setEventNoteID(eventNoteID);
						
							util->insertOrUpdateAttachedFile(info,logSession->getDriverName());
							delete util;
							prevattachedFile=aFile;

						}
							
					}
				}

				sqlreferenceBook.prepare("select * from referenceBooks where CurrentTime=:CurrentTime");
				sqlreferenceBook.bindValue(":CurrentTime",strTime);

				if(!sqlreferenceBook.exec())
				{
					PRINT_ERROR("unable to retrieve referenceBooks from SQLite Database!!!"+sqlreferenceBook.lastError().text());
					return false;
				}
				else
				{
					while(sqlreferenceBook.next())
					{
						QString bname;
						QString author;
						QString bdescription,bEdition;

						bname = sqlreferenceBook.value(sqlreferenceBook.record().indexOf("BookName")).toString();
						author= sqlreferenceBook.value(sqlreferenceBook.record().indexOf("AuthorName")).toString();
						bdescription= sqlreferenceBook.value(sqlreferenceBook.record().indexOf("BookDescription")).toString();
						bEdition= sqlreferenceBook.value(sqlreferenceBook.record().indexOf("Edition")).toString();

						if(bname!=prevbookname)
						{
							ReferenceBookUtil *refutil=new ReferenceBookUtil(this);
							ReferenceBookinfo refinfo;
							refinfo.setBookName(bname);
							refinfo.setAuthor(author);
							refinfo.setEdition(bEdition);
							refinfo.setBookDecription(bdescription);
							refinfo.setEventNoteID(eventNoteID);
							refutil->insertOrUpdateReferenceBook(refinfo,logSession->getDriverName());
							delete refutil;
							prevbookname=bname;

						}

					}
				}
				sqlreferenceLink.prepare("select * from referenceLinks where CurrentTime=:CurrentTime");
				sqlreferenceLink.bindValue(":CurrentTime",strTime);

				if(!sqlreferenceLink.exec())
				{
					PRINT_ERROR("unable to retrieve referenceLinks from SQLite Database!!!"+sqlreferenceLink.lastError().text());
					return false;
				}
				else
				{
					while(sqlreferenceLink.next())
					{
						QString url;
						QString urldescription;

						url = sqlreferenceLink.value(sqlreferenceLink.record().indexOf("URL")).toString();
						urldescription= sqlreferenceLink.value(sqlreferenceLink.record().indexOf("URLDescription")).toString();

						if(url!=prevurl)
						{
							ReferenceLinkutil *linkutil=new ReferenceLinkutil(this);
							ReferenceLinkInfo linkinfo;
							linkinfo.setURL(url);
							linkinfo.setURLDescription(urldescription);
							linkinfo.setEventNoteID(eventNoteID);
							linkutil->insertOrUpdateReferenceLink(linkinfo,logSession->getDriverName());
							delete linkutil;
							prevurl=url;

						}

					}
				}	
			}	

			SessionUtil *sUtil=new SessionUtil(this);
			SessionInfo sInfo;
			sInfo.setSessionID(sessionid);
			sInfo.setEndedTime(maxTime);

			sUtil->endSession(sInfo,logSession->getDriverName());

			delete sUtil;
		}		


		return true;
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
		return false;
	}
	catch(...)//catching unknown exceptions
	{
		PRINT_ERROR("UNKnown ERROR");
		return false;
	}
}
bool SQLiteDataBase::commitAllData()
{
	int eventid;
	int pcid;
	int eventtypeid;
	int sessionid;
	int userid;
	int roomid;
	int eventNoteID;

	QString prevFilePath="";
	QString prevattachedFile="";
	QString prevbookname="";
	QString prevurl="";

	QSqlDatabase db= QSqlDatabase::database("Inst_SQLLite");

	try
	{
		QSqlQuery sqlDetails(db);

		sqlDetails.prepare("select * from details");

		if(!sqlDetails.exec())
		{
			PRINT_ERROR("unable to retrieve details from SQLite Database!!!"+sqlDetails.lastError().text());
			return false;
		}
		else
		{
			if(sqlDetails.next())
			{
				eventid = sqlDetails.value(sqlDetails.record().indexOf("eventid")).toInt();
				pcid= sqlDetails.value(sqlDetails.record().indexOf("pcid")).toInt() ;
				eventtypeid= sqlDetails.value(sqlDetails.record().indexOf("eventtypeid")).toInt() ;
				sessionid= sqlDetails.value(sqlDetails.record().indexOf("sessionid")).toInt() ;
				userid= sqlDetails.value(sqlDetails.record().indexOf("userid")).toInt() ;
				roomid= sqlDetails.value(sqlDetails.record().indexOf("roomid")).toInt() ;
			}
		}

		QSqlQuery sqlNotes(db);

		QSqlQuery sqlreferenceFile(db);
		QSqlQuery sqlreferenceBook(db);
		QSqlQuery sqlreferenceLink(db);


		sqlNotes.prepare("select * from notes");	

		if(!sqlNotes.exec())
		{
			PRINT_ERROR("unable to retrieve notes from SQLite Database!!!"+sqlNotes.lastError().text());
			return false;
		}
		else
		{
			int fileid;
			QDateTime maxTime;

			while(sqlNotes.next())
			{
				QDateTime eventTime;
				QString richEventSpeaker;
				QString richEventNotes;			
				QString filePath,iReference,eReference;
				int visibility,splitStatus;

				QString strTime=sqlNotes.value(sqlNotes.record().indexOf("EventTime")).toString();
				eventTime = QDateTime::fromString(strTime,"dd/MM/yyyy hh:mm:ss AP");
				richEventSpeaker = sqlNotes.value(sqlNotes.record().indexOf("RichEventSpeaker")).toString();
				richEventNotes = sqlNotes.value(sqlNotes.record().indexOf("RichEventNote")).toString();
				filePath = sqlNotes.value(sqlNotes.record().indexOf("FilePath")).toString();
				splitStatus = sqlNotes.value(sqlNotes.record().indexOf("SplitStatus")).toInt();
				iReference = sqlNotes.value(sqlNotes.record().indexOf("InternalReference")).toString();
				eReference = sqlNotes.value(sqlNotes.record().indexOf("ExternalReference")).toString();
				visibility = sqlNotes.value(sqlNotes.record().indexOf("Visibility")).toInt();
				
				if(filePath!=prevFilePath)
				{
					FileUtil *fUtil=new FileUtil(this);
					FileInfo fInfo;

					fInfo.setFilePath(filePath);
					fInfo.setFileHash("--");
					fInfo.setCreatedDate(eventTime);		
					fInfo.setEventID(eventid);
					fInfo.setSessionID(sessionid);
					fInfo.setSplitStatus(splitStatus);

					fileid=fUtil->insertFileOrUpdate(fInfo,logSession->getDriverName());
					delete fUtil;
					
					prevFilePath=filePath;
				}			

				EventNoteUtil *enUtil=new EventNoteUtil(this);
				EventNoteInfo enInfo;
				
				enInfo.setEventTime(eventTime);
				enInfo.setEventSpeaker(PlainText(richEventSpeaker));
				enInfo.setEventNote(PlainText(richEventNotes));
				enInfo.setRichSpeaker(richEventSpeaker);
				enInfo.setRichNote(richEventNotes);
				enInfo.setVisibility(visibility);
				enInfo.setEventID(eventid);	
				enInfo.setFileID(fileid);
				enInfo.setRoomID(roomid);
				enInfo.setUserID(userid);
				enInfo.setPCID(pcid);
				enInfo.setSessionID(sessionid);
				if(eReference=="")
				{
					enInfo.setSessionURL(PlainText("--"));
				}
				else
				{
					enInfo.setSessionURL(eReference);
				}
				if(iReference=="")
				{
					enInfo.setElapsedTime(PlainText("--"));
				}
				else
				{
					enInfo.setElapsedTime(iReference);
				}

				eventNoteID=enUtil->insertOrUpdateEventNote(enInfo,logSession->getDriverName());
				delete enUtil;		

				maxTime=eventTime;

				sqlreferenceFile.prepare("select * from referenceFile where CurrentTime=:CurrentTime");
				sqlreferenceFile.bindValue(":CurrentTime",strTime);

				if(!sqlreferenceFile.exec())
				{
					PRINT_ERROR("unable to retrieve notes from SQLite Database!!!"+sqlreferenceFile.lastError().text());
					return false;
				}
				else
				{
					while(sqlreferenceFile.next())
					{
						QString aFile;
						QString aHash,aSize,aType;
						int uploadstaus;

						aFile = sqlreferenceFile.value(sqlreferenceFile.record().indexOf("AttachedPath")).toString();
						aHash= sqlreferenceFile.value(sqlreferenceFile.record().indexOf("AttachedHash")).toString();
						aSize= sqlreferenceFile.value(sqlreferenceFile.record().indexOf("AttachedSize")).toString();
						aType= sqlreferenceFile.value(sqlreferenceFile.record().indexOf("AttachedType")).toString();
						uploadstaus= sqlreferenceFile.value(sqlreferenceFile.record().indexOf("UploadStatus")).toInt();

						if(aFile!=prevattachedFile)
						{
							AttachedFileUtil *util=new AttachedFileUtil(this);
							AttachedFileInfo info;
							info.setAttachedPath(aFile);
							info.setAttachedhash(aHash);
							info.setAttachedSize(aSize);
							info.setAttachedType(aType);
							info.setUploadStatus(uploadstaus);
							info.setEventNoteID(eventNoteID);

							util->insertOrUpdateAttachedFile(info,logSession->getDriverName());
							delete util;

							prevattachedFile=aFile;

						}

					}
				}

				sqlreferenceBook.prepare("select * from referenceBooks where CurrentTime=:CurrentTime");
				sqlreferenceBook.bindValue(":CurrentTime",strTime);

				if(!sqlreferenceBook.exec())
				{
					PRINT_ERROR("unable to retrieve notes from SQLite Database!!!"+sqlreferenceBook.lastError().text());
					return false;
				}
				else
				{
					while(sqlreferenceBook.next())
					{
						QString bname;
						QString author;
						QString bdescription,bEdition;

						bname = sqlreferenceBook.value(sqlreferenceBook.record().indexOf("BookName")).toString();
						author= sqlreferenceBook.value(sqlreferenceBook.record().indexOf("AuthorName")).toString();
						bdescription= sqlreferenceBook.value(sqlreferenceBook.record().indexOf("BookDescription")).toString();
						bEdition= sqlreferenceBook.value(sqlreferenceBook.record().indexOf("Edition")).toString();

						if(bname!=prevbookname)
						{
							ReferenceBookUtil *refutil=new ReferenceBookUtil(this);
							ReferenceBookinfo refinfo;
							refinfo.setBookName(bname);
							refinfo.setAuthor(author);
							refinfo.setEdition(bEdition);
							refinfo.setBookDecription(bdescription);
							refinfo.setEventNoteID(eventNoteID);
							refutil->insertOrUpdateReferenceBook(refinfo,logSession->getDriverName());
							delete refutil;
							prevbookname=bname;

						}

					}
				}
				sqlreferenceLink.prepare("select * from referenceLinks where CurrentTime=:CurrentTime");
				sqlreferenceLink.bindValue(":CurrentTime",strTime);

				if(!sqlreferenceLink.exec())
				{
					PRINT_ERROR("unable to retrieve notes from SQLite Database!!!"+sqlreferenceLink.lastError().text());
					return false;
				}
				else
				{
					while(sqlreferenceLink.next())
					{
						QString url;
						QString urldescription;

						url = sqlreferenceLink.value(sqlreferenceLink.record().indexOf("URL")).toString();
						urldescription= sqlreferenceLink.value(sqlreferenceLink.record().indexOf("URLDescription")).toString();

						if(url!=prevurl)
						{
							ReferenceLinkutil *linkutil=new ReferenceLinkutil(this);
							ReferenceLinkInfo linkinfo;
							linkinfo.setURL(url);
							linkinfo.setURLDescription(urldescription);
							linkinfo.setEventNoteID(eventNoteID);
							linkutil->insertOrUpdateReferenceLink(linkinfo,logSession->getDriverName());
							delete linkutil;
							prevurl=url;

						}
					}
				}	
			}	

			SessionUtil *sUtil=new SessionUtil(this);

			SessionInfo sInfo;
			sInfo.setSessionID(sessionid);
			sInfo.setEndedTime(maxTime);

			sUtil->endSession(sInfo,logSession->getDriverName());

			delete sUtil;
		
			QSqlQuery sqlDeleteDetails(db);
			
			sqlDeleteDetails.prepare("delete from details");	

			if(!sqlDeleteDetails.exec())
			{
				PRINT_ERROR("unable to delete details from SQLite Database!!!"+sqlNotes.lastError().text());
				return false;
			}

			QSqlQuery sqlDeleteNotes(db);

			sqlDeleteNotes.prepare("delete from notes");	

			if(!sqlDeleteNotes.exec())
			{
				PRINT_ERROR("unable to delete details from SQLite Database!!!"+sqlNotes.lastError().text());
				return false;
			}
		}		

		return true;
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
		return false;
	}
	catch(...)//catching unknown exceptions
	{
		PRINT_ERROR("UNKnown ERROR");
		return false;
	}
}

QString SQLiteDataBase::PlainText(QString str_)
{
	QTextDocument doc;
	doc.setHtml(str_);
	return doc.toPlainText ();
}
SQLiteDataBase::~SQLiteDataBase()
{

}
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/