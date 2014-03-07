/*Owner & Copyrights: Vance King Saxbe. A.*//* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*/#include "dlcrmain.h"
#include <QDesktopServices>
#include <QDir>
#include <QUrl>
#include <QTime>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QByteArray>
#include <QClipboard>
#include <QTextDocument>
#include <QDebug>

#include "advancedsearch.h"
#include "uispellchecker.h"
#include "appsettingsutil.h"
#include "eventnoteutil.h"
#include "backupserverconnection.h"
#include "hashmanager.h"
#include "filedownloader.h"
#include "aboutcrs.h"
#include "ftpmanager.h"
#include "advancedsearch.h"
#include "diskanalizer.h"
#include "writetodisk.h"
#include "mailsettingsutil.h"
#include "maildetailsutil.h"
//#include "recorderhelp.h"
#include "keygen.h"
#include "systemfetch.h"
#include "licenseutil.h"
#include "pcutil.h"
#include "audiodeviceutil.h"
#include "videodeviceutil.h"
#include "wizard.h"
#include "languages.h"
#include "dictionary.h"
#include "languageutil.h"
#include "actionmenu.h"
#include "scheduler.h"
#include "replace.h"
#include "find.h"
#include "goto.h"

#include "SchedulerUtil.h"
#include "sessionutil.h"
#include "appsettingsutil.h"
#include "eventtypeutil.h"
#include "locationutil.h"
#include "roomutil.h"
#include "importnotes.h"
#include "filesdetails.h"
#include "linkdetails.h"
#include "imagedetails.h"
#include "bookdetails.h"


DLCRMain::DLCRMain(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	logSession=LoginUserSession::session();
	sharedObj=new DLCRShared();
	//engine=new LicenseEngine(this);

	explore=new ExploreSession(this);
	tempDB=SQLiteDataBase::dataBase();
	shortcuts=new EasyNotes(this);
	history=new LoginHistory(this);
	recordingStatus=false;

	icoPlay=QIcon(QPixmap(":/Recorder/Resources/play.png"));
	icoPause=QIcon(QPixmap(":/Recorder/Resources/pause.png"));
	setWindowTitle("THE TAPE");
	ui.actionPause_Resume->setIcon(icoPause);
	ui.actionPause_Resume->setText("Pause Case");
	ui.recorder->setPaused(false);
	ui.actionPause_Resume->setEnabled(false);

	myTimer=new QTimer(this);
	ScheduleTimer=new QTimer(this);
	ScheduleProcess=new QTimer(this);
	ScheduleStop=new QTimer(this);
	ScheduleDuration=new QTimer(this);
	
	connect(ui.actionNew_OpenCase,			SIGNAL(triggered()),			this,	SLOT(actionNew_OpenCase_clicked()));
	connect(ui.actionStop_Case,				SIGNAL(triggered()),			this,	SLOT(actionStop_Case_clicked()));
	connect(ui.actionSave_Notes,			SIGNAL(triggered()),			this,	SLOT(actionSaveToServer_clicked()));
	connect(ui.actionLog_Editor,			SIGNAL(triggered()),			this,	SLOT(actionLog_Editor_clicked()));
	connect(ui.actionRecorder,				SIGNAL(triggered()),			this,	SLOT(actionRecorder_clicked()));
	connect(ui.actionSpellChecker,			SIGNAL(triggered()),			this,	SLOT(actionSpell_Check_Tool_clicked()));	
	connect(ui.actionSpell_Check_Tool,		SIGNAL(triggered()),			this,	SLOT(actionSpell_Check_Tool_clicked()));
	connect(ui.actionEasy_Notes,			SIGNAL(triggered()),			this,	SLOT(actionEasy_Notes_clicked()));
	connect(ui.actionRetrieve,				SIGNAL(triggered()),			this,	SLOT(actionRetrieve_clicked()));
	connect(ui.actionExit_Recorder,			SIGNAL(triggered()),			this,	SLOT(actionExit_Recorder_clicked()));
	connect(ui.actionAbout_DLCR,			SIGNAL(triggered()),			this,	SLOT(actionAboutDLCR_clicked()));
	connect(ui.actionOnline_Support,		SIGNAL(triggered()),			this,	SLOT(actionOnlineSupport_clicked()));
	connect(ui.actionOnline_Update,			SIGNAL(triggered()),			this,	SLOT(actionOnlineUpdate_clicked()));	
	connect(ui.actionAttach_File,			SIGNAL(triggered()),			this,	SLOT(actionAttachFile_clicked()));
	connect(ui.actionDownload_file,			SIGNAL(triggered()),			this,	SLOT(actionDownloadFile_clicked()));
	connect(ui.actionHelp,					SIGNAL(triggered()),			this,	SLOT(actionHelp_clicked()));
	connect(ui.actionFTP_Uploader,			SIGNAL(triggered()),			this,	SLOT(actionFTPUploader_clicked()));
	connect(ui.actionLogin_History,			SIGNAL(triggered()),			this,	SLOT(actionLoginHistory_clicked()));
	connect(ui.actionAdvanced_Search,		SIGNAL(triggered()),			this,	SLOT(actionAdvanced_Search_clicked()));
	connect(ui.actionDisk_Analizer,			SIGNAL(triggered()),			this,	SLOT(actionDiskAnalizer_click()));
	connect(ui.actionBackup_Utility,		SIGNAL(triggered()),			this,	SLOT(actionBackUputility_clicked()));
	connect(ui.actionRecorderSettings,		SIGNAL(triggered()),			this,	SLOT(actionRecorderSettings_clicked()));
	connect(ui.actionSearch,				SIGNAL(triggered()),			this,	SLOT(actionSearch_clicked()));

	connect(ui.tabEvents,					SIGNAL(tabCloseRequested(int)),	this,	SLOT(tabEvents_closeRequested(int)));
	connect(ui.tabEvents,					SIGNAL(currentChanged(int)),	this,	SLOT(tabEvents_currentChanged(int)));
	connect(ui.actionToPDF,					SIGNAL(triggered()),			ui.activeRetriever,	SLOT(exportPdf()));
	connect(ui.actionToCSV,					SIGNAL(triggered()),			ui.activeRetriever,	SLOT(exportToCsv_click()));
	connect(ui.actionToWORDDOC,				SIGNAL(triggered()),			ui.activeRetriever,	SLOT(exportWordDoc()));
	connect(ui.actionPrintNotes,			SIGNAL(triggered()),			ui.activeRetriever, SLOT(printDocument()));
	connect(ui.actionPrint_Case_Log,		SIGNAL(triggered()),			ui.activeRetriever, SLOT(printDocument()));
	connect(ui.actionPrint_Preview,			SIGNAL(triggered()),			ui.activeRetriever, SLOT(printPreview()));
	connect(ui.actionPrint_Priview,			SIGNAL(triggered()),			ui.activeRetriever, SLOT(printPreview()));
	connect(ui.actionPause_Resume,			SIGNAL(triggered(bool)),		this,	SLOT(buttonPlayPause_clicked(bool)));
	
	connect(ui.tabEvents,					SIGNAL(tabCloseRequested(int)),	this,	SLOT(tabEvents_closeRequested(int)));
	connect(ui.tabEvents,					SIGNAL(currentChanged(int)),	this,	SLOT(tabEvents_currentChanged(int)));
	   
	connect(ui.recorder,					SIGNAL(beginRestart()),			this,	SLOT(restartRecording()));
	connect(myTimer,						SIGNAL(timeout()),				this,	SLOT(currentRecorderTime()));
	connect(ui.recorder,					SIGNAL(audioLevel(int,int)),	this,	SLOT(updateAudioLevels(int,int)));
	connect(ui.widget,						SIGNAL(volumeControl(int ,int)),this,	SLOT(VolumeValue(int,int)));

	connect(ui.actionNew_Notes,				SIGNAL(triggered()),			this,	SLOT(actionNew_Notes_clicked()));
	connect(ui.actionBefore_Row,			SIGNAL(triggered()),			this,	SLOT(actionBefore_Rowclicked()));
	connect(ui.actionAfter_Row,				SIGNAL(triggered()),			this,	SLOT(actionAfter_Rowclicked()));
	connect(ui.actionDelete_Row,			SIGNAL(triggered()),			this,	SLOT(actionDelete_Row_clicked()));
	connect(ui.actionImport,				SIGNAL(triggered()),			this,	SLOT(actionImport_clicked()));
	connect(ui.actionCut,					SIGNAL(triggered()),			this,	SLOT(actionCut_clicked()));
	connect(ui.actionCopy,					SIGNAL(triggered()),			this,	SLOT(actionCopy_clicked()));
	connect(ui.actionPaste,					SIGNAL(triggered()),			this,	SLOT(actionPaste_clicked()));
	connect(ui.actionText_Color,			SIGNAL(triggered()),			this,	SLOT(actionText_Color_clicked()));
	connect(ui.actionGoto,					SIGNAL(triggered()),			this,	SLOT(actionGoto_clicked()));
	connect(ui.actionFindNotes,				SIGNAL(triggered()),			this,	SLOT(actionFindNotes_clicked()));
	connect(ui.actionReplace,				SIGNAL(triggered()),			this,	SLOT(actionReplace_clicked()));
	connect(ui.actionInsert,				SIGNAL(triggered()),			this,	SLOT(actionInsert_clicked()));
	connect(ui.actionFiles,					SIGNAL(triggered()),			this,	SLOT(actionFiles_clicked()));
	connect(ui.actionImages,				SIGNAL(triggered()),			this,	SLOT(actionImages_clicked()));
	connect(ui.actionBooks,					SIGNAL(triggered()),			this,	SLOT(actionBooks_clicked()));
	connect(ui.actionLinks,					SIGNAL(triggered()),			this,	SLOT(actionLinks_clicked()));

	connect(ui.actionSimilar_Sessions,		SIGNAL(triggered()),			this,	SLOT(actionSimilar_Sessions_clicked()));
	connect(ui.actionRequest_Lognotes,		SIGNAL(triggered()),			this,	SLOT(actionRequest_Lognotes_clicked()));
	
	connect(ui.actionManage_Dictionary,		SIGNAL(triggered()),			this,	SLOT(actionManage_Dictionary_clicked()));
	connect(ui.actionUnderLine,		        SIGNAL(triggered(bool)),		this,	SLOT(actionUnderLine_clicked(bool)));
	connect(ui.actionBold_Format,			SIGNAL(triggered(bool)),		this,	SLOT(actionBold_Format_clicked(bool)));
	connect(ui.actionItalics_Format,		SIGNAL(triggered(bool)),		this,	SLOT(actionItalics_Format_clicked(bool)));

	connect(ui.actionManage_Language,		SIGNAL(triggered()),			this,	SLOT(actionManage_Language_clicked()));
	connect(ui.actionScheduler,				SIGNAL(triggered()),			this,	SLOT(actionScheduler_clicked()));
	connect(ui.activeEditor,				SIGNAL(chkBStatus(bool)),		this,	SLOT(checked_Bold(bool)));
	connect(ui.activeEditor,				SIGNAL(chkIStatus(bool)),		this,	SLOT(checked_Italics(bool)));
	connect(ui.activeEditor,				SIGNAL(chkUStatus(bool)),		this,	SLOT(checked_UnderLine(bool)));
	connect(ui.activeRetriever,				SIGNAL(chkRBStatus(bool)),		this,	SLOT(checked_Bold(bool)));
	connect(ui.activeRetriever,				SIGNAL(chkRIStatus(bool)),		this,	SLOT(checked_Italics(bool)));
	connect(ui.activeRetriever,				SIGNAL(chkRUStatus(bool)),		this,	SLOT(checked_UnderLine(bool)));
	
	connect(ui.activeEditor,				SIGNAL(keyPress()),				this,	SLOT(checked_KeyPress()));
	connect(ui.activeEditor,				SIGNAL(editorRowPress()),		this,	SLOT(checked_RowPress()));
	connect(ui.activeRetriever,				SIGNAL(retreiveRowPress()),		this,	SLOT(checked_RetreiveRowPress()));
	connect(ui.activeRetriever,				SIGNAL(retrieveCellClicked()),	this,	SLOT(checked_cellPress()));
	connect(ui.activeEditor,				SIGNAL(editorRowDoublePress()),	this,	SLOT(checked_RowDoublePress()));
	connect(ui.activeRetriever,				SIGNAL(retrieveCellDoubleClicked()),this,SLOT(checked_cellDoublePress()));

	connect(ui.actionText_Highlight,		SIGNAL(triggered()),			this,	SLOT(actionText_Highlight_Triggred()));
	connect(ScheduleTimer,					SIGNAL(timeout()),				this,	SLOT(scheduler_Time()));
	connect(ScheduleProcess,				SIGNAL(timeout()),				this,	SLOT(scheduler_Processs()));
	connect(ScheduleStop,					SIGNAL(timeout()),				this,	SLOT(scheduler_Stop()));
	connect(ScheduleDuration,				SIGNAL(timeout()),			    this,	SLOT(scheduler_Duration()));

	ui.dockWidget->setHidden(true);
	ui.dockWidget->setHidden(true);
	ui.tabEvents->setHidden(true);
	ui.actionStop_Case->setEnabled(false);
	ui.actionSave_Notes->setEnabled(false);
	ui.actionBold->setEnabled(false);
	ui.actionItalics->setEnabled(false);
	ui.actionUnderline->setEnabled(false);
	ui.menuExportNotes->setEnabled(false);
	ui.actionPrintNotes->setEnabled(false);
	ui.actionPrint_Case_Log->setEnabled(false);
	ui.actionPrint_Priview->setEnabled(false);
	ui.actionSpell_Check_Tool->setEnabled(false);
	ui.actionPrint_Preview->setEnabled(false);
	ui.actionRetrieve->setEnabled(false);
	ui.actionAttach_File->setEnabled(false);
	ui.actionDownload_file->setEnabled(false);
	ui.StatusFrame->setVisible(false);
	ui.actionToHTML->setVisible(false);
	ui.actionToCSV->setVisible(true);
	ui.actionRecorder->setEnabled(false);
	ui.actionLog_Editor->setEnabled(false);
	ui.actionPause_Resume->setEnabled(false);
	
	ui.menuInsert_Row->setEnabled(false);
	ui.actionDelete_Row->setEnabled(false);
	ui.actionImport->setEnabled(false);
	ui.actionCut->setEnabled(false);
	ui.actionCopy->setEnabled(false);
	ui.actionPaste->setEnabled(false);
	ui.actionGoto->setEnabled(false);
	ui.actionFindNotes->setEnabled(false);
	ui.actionReplace->setEnabled(false);
	ui.actionInsert->setEnabled(false);
	ui.actionFiles->setEnabled(false);
	ui.actionImages->setEnabled(false);
	ui.actionBooks->setEnabled(false);
	ui.actionLinks->setEnabled(false);
	ui.actionSimilar_Sessions->setEnabled(false);
	ui.menuFormat_Style->setEnabled(false);
	ui.actionNew_Notes->setEnabled(false);
	ui.actionSpellChecker->setEnabled(false);
	ui.actionSimilar_Sessions->setVisible(false);
	ui.actionRequest_Lognotes->setVisible(false);
	ui.actionInsert->setVisible(false);
	ui.actionText_Highlight->setEnabled(false);
	ui.actionText_Color->setVisible(false);
	ui.actionSearch->setVisible(false);

	if(logSession->canUploadToFTP()) ui.actionFTP_Uploader->setEnabled(true);
	else ui.actionFTP_Uploader->setEnabled(false);

	if(NOT logSession->canWriteOpticalDisk() && NOT logSession->canWriteRemovableDisk()) 
	{
		ui.actionBackup_Utility->setEnabled(false);
	}

	createExitMsg();
	createTray();
	loadLanguage();	
}

void DLCRMain::actionText_Highlight_Triggred()
{
	if (ui.tabEvents->currentIndex()!=0)
	{
		ui.activeRetriever->actionText_Highlight_Clicked();
	}
	else
	{
		ui.activeEditor->action_Text_Color_selection();	
	}
}
void DLCRMain::checked_RowDoublePress()
{
	ui.menuFormat_Style->setEnabled(true);
	ui.actionPaste->setEnabled(false);
}

void DLCRMain::checked_cellDoublePress()
{
	if (logSession->canEditRetrieved())
	{
		ui.menuFormat_Style->setEnabled(true);
	}
	else
	{
		ui.menuFormat_Style->setEnabled(false);
	}
	
	ui.actionPaste->setEnabled(false);
}

void DLCRMain::scheduler_Time()
{
	SchedulerUtil *sutil=new SchedulerUtil(this);
	QList<SchedulerInfo> schedulersLists;
//	QString sAction,SRepeat,sDate,sTime,SEventNo,sEventName,sEventType,sEventDescription,sExpectTime;
	QString sAction,SRepeat,sDate,sTime,sExpectTime;
	int sun,mon,tues,wed,thur,fri,sat,sDuration;
	int previousTime;
	try
	{
		schedulersLists=sutil->selectSchedulerByRoom(logSession->getCurrentRoom(),1,logSession->getDriverName());
		foreach(SchedulerInfo sinfo,schedulersLists)
		{	
			sTime=sinfo.getScheduleTime();
			QTime time=QTime::fromString(sTime,"h:m:s AP");
			QTime t,cTime; 
		
			int schedulerTime=t.secsTo(time);
			int currentTime=cTime.secsTo(QTime::currentTime());
			int expectedTime=schedulerTime-currentTime;
			if (expectedTime<0)
			{
				continue;
			}
			if (expectedTime<previousTime)
			{
				previousTime=expectedTime;
				sExpectTime=sTime;
			}
		}
		SchedulerInfo sinfo=sutil->loadSchedulerByTime(sExpectTime,logSession->getCurrentRoom(),logSession->getDriverName());
		schedulersID=sinfo.getScheduleID();
		sDate=sinfo.getScheduleDate();
		sTime=sinfo.getScheduleTime();
		sun=sinfo.getSunday();
		mon=sinfo.getMonday();
		tues=sinfo.getTuesday();
		wed=sinfo.getWenesday();
		thur=sinfo.getThursday();
		fri=sinfo.getFriday();
		sat=sinfo.getSaturday();
		sDuration=sinfo.getDuration();
		sAction=sinfo.getSchedulerAction();
		SRepeat=sinfo.getSchedulerRepeat();
		SEventNo=sinfo.getEventNo();
		sEventName=sinfo.getEventName();
		sEventType=sinfo.getEventType(); 
		sEventID=sinfo.getEventID();
		sEventDescription=sinfo.getEventDescription();

		QDate date=QDate::fromString(sDate,"MM'dd'yyyy");
		QTime time=QTime::fromString(sTime,"h:m:s AP");
		QTime t,cTime; 
			
		if (SRepeat=="Once")
		{
			if(date==QDate::currentDate());
			{
				int schedulerTime=t.secsTo(time);
				int currentTime=cTime.secsTo(QTime::currentTime());
				int expectedTime=schedulerTime-currentTime;
				int durationTime=sDuration*60;
				int totalTime=durationTime+expectedTime;

				if (sAction=="Resume"||sAction=="StartNew")
				{
					ScheduleProcess->start(expectedTime*1000);
					ScheduleDuration->start(totalTime*1000);
 				}
				if(sAction=="Stop")
				{
					ScheduleStop->start(expectedTime*1000);
				}		
			}		
		}
		if (SRepeat=="Daily")
		{
			int schedulerTime=t.secsTo(time);
			int currentTime=cTime.secsTo(QTime::currentTime());
			int expectedTime=schedulerTime-currentTime;
			int durationTime=sDuration*60;
			int totalTime=durationTime+expectedTime;

			if (sAction=="Resume"||sAction=="StartNew")
			{
				ScheduleProcess->start(expectedTime*1000);
				ScheduleDuration->start(totalTime*1000);
			}
			if(sAction=="Stop")
			{
				ScheduleStop->start(expectedTime*1000);
			}
		}
		if (SRepeat=="Weekly")
		{
			int schedulerTime=t.secsTo(time);
			int currentTime=cTime.secsTo(QTime::currentTime());
			int expectedTime=schedulerTime-currentTime;

			int durationTime=sDuration*60;
			int totalTime=durationTime+expectedTime;

			QDate cdate=QDate::currentDate();
			int dayOfWeek=cdate.dayOfWeek();
			
			if (sAction=="Resume"||sAction=="StartNew")
			{
					if (dayOfWeek==7 && sun==1)
					{
						ScheduleProcess->start(expectedTime*1000);
						ScheduleDuration->start(totalTime*1000);
					}
					if (dayOfWeek==1 && mon==1)
					{
						ScheduleProcess->start(expectedTime*1000);
						ScheduleDuration->start(totalTime*1000);
					}
					if (dayOfWeek==2 && tues==1)
					{
						ScheduleProcess->start(expectedTime*1000);
						ScheduleDuration->start(totalTime*1000);
					}
					if (dayOfWeek==3 && wed==1)
					{
						ScheduleProcess->start(expectedTime*1000);
						ScheduleDuration->start(totalTime*1000);
					}
					if (dayOfWeek==4 && thur==1)
					{
						ScheduleProcess->start(expectedTime*1000);
						ScheduleDuration->start(totalTime*1000);
					}
					if (dayOfWeek==5 && fri==1)
					{
						ScheduleProcess->start(expectedTime*1000);
						ScheduleDuration->start(totalTime*1000);
					}
					if (dayOfWeek==6 && sat==1)
					{
						ScheduleProcess->start(expectedTime*1000);
						ScheduleDuration->start(totalTime*1000);
					}
			}
			if(sAction=="Stop")
			{
				if (dayOfWeek==1 && sun==1)
				{
					ScheduleStop->start(expectedTime*1000);
				}
				if (dayOfWeek==2 && mon==1)
				{
					ScheduleStop->start(expectedTime*1000);
				}
				if (dayOfWeek==3 && tues==1)
				{
					ScheduleStop->start(expectedTime*1000);
				}
				if (dayOfWeek==4 && wed==1)
				{
					ScheduleStop->start(expectedTime*1000);
				}
				if (dayOfWeek==5 && thur==1)
				{
					ScheduleStop->start(expectedTime*1000);
				}
				if (dayOfWeek==6 && fri==1)
				{
					ScheduleStop->start(expectedTime*1000);
				}
				if (dayOfWeek==7 && sat==1)
				{
					ScheduleStop->start(expectedTime*1000);
				}
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
}
void DLCRMain::scheduler_Processs()
{
	EventUtil *util=new EventUtil(this);
	SessionUtil *session=new SessionUtil(this);
	AppSettingsUtil *settings=new AppSettingsUtil(this);
	EventTypeUtil *etutil=new EventTypeUtil(this);
	RoomUtil *rutil=new RoomUtil(this);
	LocationUtil *lutil=new LocationUtil(this);

	if (ui.recorder->g->isRunning())
	{
		stopEvent();
	}

	try
	{
		printf("\Entered try...\n");
		AppSettingsInfo sInfo=settings->loadAppSettings(logSession->getPCID(),logSession->getDriverName());
		SessionInfo info;

		if (sInfo.getSplittingDuration()!=1000000)
		{
			info.setMergeStatus(0);
		}
		else
		{
			info.setMergeStatus(1);
		}

		int sessionID=session->createSession(info,logSession->getDriverName());
		EventTypeInfo eventTypeInfo=etutil->loadEventTypeID(sEventType,logSession->getDriverName());
		RoomInfo rinfo=rutil->loadRoom(logSession->getCurrentRoom(),logSession->getDriverName());
		LocationInfo linfo=lutil->loadLocation(rinfo.getLocationID(),logSession->getDriverName());
		printf("\Entered log...\n");
		logSession->setEventID(sEventID);
		logSession->setEventNo(SEventNo.trimmed());
		logSession->setEventName(sEventName);
		logSession->setEventDescription(sEventDescription);
		logSession->setSessionID(sessionID);
		logSession->setCurrentEventType(eventTypeInfo.getEventTypeID());

		caseDetails details;
		details.no=logSession->getEventNo();
		details.title=logSession->getEventName();
		details.description=logSession->getEventDescription();
		details.dt=sharedObj->getCurrentDateTime();
		details.location=linfo.getLocationName();
		details.room=rinfo.getRoomName();
		details.eventType=sEventType;

		ui.activeEditor->setCaseDetails(details);

		QString eventHomePath=sharedObj->getEventHomePath(linfo.getLocationName(),rinfo.getRoomName(),sEventType.trimmed(),logSession->getEventNo());
		logSession->setEventHomePath(eventHomePath);

		int videoCount=getVideoCount();
		QStringList filePath=sharedObj->getEventFileName(videoCount,logSession->getEventNo());
		QStringList filepaths;
		for (int i=0;i<videoCount;i++)
		{
			filepaths<<eventHomePath+"/"+filePath[i];
		}
		logSession->setCurrentFilePath(filepaths);
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

	delete settings;
	delete session;
	delete util;

	if(tempDB->connectDatabase()==false) 
	{
		SHOW_MESSAGE(tr("TAPE ERROR"),tr("Database connection failed!!!"),tr("Ok"));
		PRINT_ERROR("Failed to create SQLLite Database connection!!!");
		return;
	}

	if(tempDB->checkDataExistence()!=false)
	{
		bool commitStatus=tempDB->commitAllData();
	}	

	printf("\Entered start...\n");
	startEvent();
	SchedulerUtil *sutil=new SchedulerUtil(this);
	try
	{
		SchedulerInfo sinfo;
		sinfo=sutil->loadScheduler(schedulersID,logSession->getDriverName());

		if (sinfo.getSchedulerRepeat()=="Once")
		{
			sutil->editSchedulerStatus(schedulersID,0,logSession->getDriverName());
		}
		
		//sutil->editSchedulerStatus(sDate,sTime,sAction,SRepeat,SEventNo,sEventName,sEventDescription,sEventType,logSession->getCurrentRoom(),sEventID,0,logSession->getDriverName());
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
	delete sutil;
	ScheduleProcess->stop();
	//ScheduleTimer->start(1000);
}
void DLCRMain::scheduler_Stop()
{
	stopEvent();
	ScheduleStop->stop();
	//ScheduleTimer->start(1000);
}
void DLCRMain::scheduler_Duration()
{
	stopEvent();
	ScheduleDuration->stop();
	//ScheduleTimer->start(1000);
}

void DLCRMain::stopEvent()
{
	if (ui.recorder->g->isRunning())
	{
		if(!ui.recorder->stopGraph())
		{
			SHOW_CRITICAL_ERROR(tr("ERROR!!!"),tr("An unknown error happened..."),tr("Unable to stop session"),tr("Ok"));
			PRINT_WARNING("An unknown error happened...");
			ui.actionStop_Case->blockSignals(false);
			ui.actionStop_Case->setEnabled(true);
			return;
		}
	}
	else
	{
		return;
	}
	ui.recorder->stopSpiltTimer();
	tempNotes notes;
	notes.eventTime=sharedObj->getCurrentDateTime().toString("dd/MM/yyyy hh:mm:ss AP");
	//notes.filePath=sharedObj->filterToServerPath(logSession->getCurrentFilePath(),logSession->getSharedPath());
	notes.richEventSpeaker="Event stopped "+logSession->getEventNo();
	notes.richEventNotes="Auto generated field";
	notes.internalReference="--";
	notes.externalReference="--";
	notes.visibility=0;
	QStringList toServerpath;
	toServerpath=sharedObj->filterToServerPath(logSession->getCurrentFilePath(),logSession->getSharedPath());

	for (int i=0;i<toServerpath.count();i++)
	{
		tempDB->insertNotes(notes,toServerpath[i],0);
	}
	//tempDB->insertNotes(notes);

	if(tempDB->commitAllData()!=true)
	{
		SHOW_INFOMESSAGE(tr("ERROR!!!"),tr("Database server connection lost"),tr("Notes are safely saved in local database"),tr("Ok"));
	}
	ui.activeEditor->clear();

	ui.dockWidget->setHidden(true);	
	ui.StatusFrame->setVisible(false);
	ui.tabEvents->setHidden(true);	

	ui.actionNew_OpenCase->setEnabled(true);
	ui.actionBackup_Utility->setEnabled(true);
	ui.actionAdvanced_Search->setEnabled(true);
	ui.actionStop_Case->setEnabled(false);
	ui.actionSave_Notes->setEnabled(false);
	ui.actionBold->setEnabled(false);
	ui.actionItalics->setEnabled(false);
	ui.actionUnderline->setEnabled(false);
	ui.actionRetrieve->setEnabled(false);
	ui.actionAttach_File->setEnabled(false);
	ui.actionDownload_file->setEnabled(false);

	ui.actionLog_Editor->setChecked(false);
	ui.actionRecorder->setChecked(false);
	ui.actionLog_Editor->setEnabled(false);
	ui.actionRecorder->setEnabled(false);

	ui.menuInsert_Row->setEnabled(false);
	ui.actionDelete_Row->setEnabled(false);
	ui.actionImport->setEnabled(false);
	ui.actionCut->setEnabled(false);
	ui.actionCopy->setEnabled(false);
	ui.actionPaste->setEnabled(false);
	ui.actionGoto->setEnabled(false);
	ui.actionFindNotes->setEnabled(false);
	ui.actionReplace->setEnabled(false);
	ui.actionInsert->setEnabled(false);
	ui.actionFiles->setEnabled(false);
	ui.actionImages->setEnabled(false);
	ui.actionBooks->setEnabled(false);
	ui.actionLinks->setEnabled(false);
	ui.actionSimilar_Sessions->setEnabled(false);
	ui.menuFormat_Style->setEnabled(false);
	ui.actionNew_Notes->setEnabled(false);
	ui.actionScheduler->setEnabled(true);
	ui.actionRecorderSettings->setEnabled(true);
	ui.actionPrint_Case_Log->setEnabled(false);
	ui.actionPrint_Priview->setEnabled(false);
	ui.actionSpell_Check_Tool->setEnabled(false);
	ui.actionSpellChecker->setEnabled(false);

	ui.menuExportNotes->setEnabled(false);
	ui.actionPrintNotes->setEnabled(false);
	ui.actionPrint_Preview->setEnabled(false);
	ui.actionText_Highlight->setEnabled(false);

	recordingStatus=false;

	history->insertHistory(sharedObj->getCurrentDateTime(),logSession->getEventNo(),"Stopped");

	myTimer->stop();

	ScheduleDuration->stop();
	ScheduleStop->stop();
	ScheduleProcess->stop();

	trayIcon->setToolTip(tr("TAPE"));
}
void DLCRMain::checked_KeyPress()
{
		if(logSession->canAttachFile()) 
		{
			ui.actionFiles->setEnabled(true);
			ui.actionImages->setEnabled(true);
			ui.actionBooks->setEnabled(true);
			ui.actionLinks->setEnabled(true);
			ui.actionAttach_File->setEnabled(true);
		}
		else
		{
			ui.actionFiles->setEnabled(false);
			ui.actionImages->setEnabled(false);
			ui.actionBooks->setEnabled(false);
			ui.actionLinks->setEnabled(false);
			ui.actionAttach_File->setEnabled(false);
		}

		ui.menuInsert_Row->setEnabled(true);
		ui.actionDelete_Row->setEnabled(true);
		ui.actionCut->setEnabled(true);
		ui.actionCopy->setEnabled(true);
		ui.actionPaste->setEnabled(true);
		ui.actionGoto->setEnabled(true);
		ui.actionFindNotes->setEnabled(true);
		ui.actionReplace->setEnabled(true);
		
		ui.menuFormat_Style->setEnabled(false);
}
void DLCRMain::checked_RowPress()
{
	ui.actionBooks->setEnabled(false);
	ui.menuInsert_Row->setEnabled(false);
	ui.actionDelete_Row->setEnabled(false);
	ui.actionFiles->setEnabled(false);
	ui.actionImages->setEnabled(false);
	ui.actionBooks->setEnabled(false);
	ui.actionLinks->setEnabled(false);
	ui.actionCut->setEnabled(false);
	ui.actionCopy->setEnabled(false);
	ui.actionPaste->setEnabled(false);
	ui.actionGoto->setEnabled(false);
	ui.actionFindNotes->setEnabled(false);
	ui.actionReplace->setEnabled(false);
	ui.menuFormat_Style->setEnabled(false);
	ui.actionAttach_File->setEnabled(false);
}
void DLCRMain::checked_RetreiveRowPress()
{
	ui.actionBooks->setEnabled(false);
	ui.menuInsert_Row->setEnabled(false);
	ui.actionDelete_Row->setEnabled(false);
	ui.actionFiles->setEnabled(false);
	ui.actionImages->setEnabled(false);
	ui.actionBooks->setEnabled(false);
	ui.actionLinks->setEnabled(false);
	ui.actionCut->setEnabled(false);
	ui.actionCopy->setEnabled(false);
	ui.actionPaste->setEnabled(false);
	ui.actionGoto->setEnabled(false);
	ui.actionFindNotes->setEnabled(false);
	ui.actionReplace->setEnabled(false);
	ui.menuFormat_Style->setEnabled(false);
	ui.actionAttach_File->setEnabled(false);
}
void DLCRMain::checked_cellPress()
{
	if (logSession->canEditRetrieved())
	{
		if(logSession->canAttachFile()) 
		{
			ui.actionFiles->setEnabled(true);
			ui.actionImages->setEnabled(true);
			ui.actionBooks->setEnabled(true);
			ui.actionLinks->setEnabled(true);
			ui.actionAttach_File->setEnabled(true);
		}
		else
		{
			ui.actionFiles->setEnabled(false);
			ui.actionImages->setEnabled(false);
			ui.actionBooks->setEnabled(false);
			ui.actionLinks->setEnabled(false);
			ui.actionAttach_File->setEnabled(false);
		}
		ui.actionDelete_Row->setEnabled(true);
		ui.actionCut->setEnabled(true);
		ui.actionCopy->setEnabled(true);
		ui.actionPaste->setEnabled(true);
	}
	else
	{
		ui.actionBooks->setEnabled(false);
		ui.menuInsert_Row->setEnabled(false);
		ui.actionDelete_Row->setEnabled(false);
		ui.actionFiles->setEnabled(false);
		ui.actionImages->setEnabled(false);
		ui.actionBooks->setEnabled(false);
		ui.actionLinks->setEnabled(false);
		ui.actionCut->setEnabled(false);
		ui.actionCopy->setEnabled(false);
		ui.actionPaste->setEnabled(false);
		ui.actionGoto->setEnabled(false);
		ui.actionFindNotes->setEnabled(false);
		ui.actionReplace->setEnabled(false);
		ui.actionAttach_File->setEnabled(false);
	}

		ui.actionGoto->setEnabled(true);
		ui.actionFindNotes->setEnabled(true);
		ui.actionReplace->setEnabled(true);
		ui.menuFormat_Style->setEnabled(false);
		
}
void DLCRMain::checked_Bold(bool status_)
{
	ui.actionBold_Format->setChecked(status_);
}
void DLCRMain::checked_Italics(bool status_)
{
	ui.actionItalics_Format->setChecked(status_);
}
void DLCRMain::checked_UnderLine(bool status_)
{
	ui.actionUnderLine->setChecked(status_);
}
void DLCRMain::loadLanguage()
{
	ui.menuLanguage_Preference->clear();

	LanguageUtil *lutil=new LanguageUtil(this);
	SystemFetch *fetch=new SystemFetch();
	PCUtil *pUtil=new PCUtil(this);

	PCInfo pInfo;
	pInfo=pUtil->loadPCDetails(fetch->getSystemID(),logSession->getDriverName());

	QList<LanguageInfo>  languageList;
	languageList=lutil->selectLanguageByUser(pInfo.getPCID(),logSession->getUserID(),"Recorder",logSession->getDriverName());
	int i=0;
	foreach(LanguageInfo info,languageList)
	{
		QString str = info.getLanguage();
		actionLanguage[i]=new LanguageMenu(this);
		actionLanguage[i]->setText(str);
		ui.menuLanguage_Preference->addAction(actionLanguage[i]);

		connect(actionLanguage[i], SIGNAL(LAction(LanguageMenu*)),this,SLOT(actionLanguage_Preference_clicked(LanguageMenu*)));
		i++;
	}
}

void DLCRMain::VolumeValue(int idx,int val)
{
	ui.recorder->setChannelvolume(idx,val);

}
void DLCRMain::updateAudioLevels(int v,int index)
{
	ui.widget->channelList.at(index)->signalIndicator->setValue(v);	
}
void DLCRMain::currentRecorderTime()
{
	QTime tempTime=QTime::fromString(ui.lblPositionNow->text(),"hh:mm:ss");
	tempTime=tempTime.addSecs(1);
	ui.lblPositionNow->setText(tempTime.toString("hh:mm:ss"));
	ui.activeEditor->setRecorderTime(tempTime.toString("hh:mm:ss"));
}
void DLCRMain::buttonPlayPause_clicked(bool status)
{
	ui.recorder->pause(status);
	if (status==true)
	{
		myTimer->stop();
		ui.actionPause_Resume->setIcon(icoPause);
	}
	else
	{
		myTimer->start(1000);
		ui.actionPause_Resume->setIcon(icoPlay);
	}
}
void DLCRMain::createExitMsg()
{
	exitMessageBox.setText(tr("Recording system is going to Exit!"));
	exitMessageBox.setWindowTitle(tr("TAPE"));
	trayButton = exitMessageBox.addButton(tr("To Tray"), QMessageBox::ActionRole);
	closeButton = exitMessageBox.addButton(tr("Close"), QMessageBox::ActionRole);
	cancelButton = exitMessageBox.addButton(tr("Cancel"), QMessageBox::ActionRole);
	exitMessageBox.setDefaultButton(cancelButton);
	exitMessageBox.setIcon(QMessageBox::Warning);
	return;
}
void DLCRMain::createTray()
{
	minimizeAction = new QAction(tr("Minimize"), this);
	maximizeAction = new QAction(tr("Maximize"), this);
	restoreAction = new QAction(tr("&Restore"), this);
	quitAction = new QAction(tr("&Quit"), this);

	trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(minimizeAction);
	trayIconMenu->addAction(maximizeAction);
	trayIconMenu->addAction(restoreAction);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(quitAction);

 	trayIcon = new QSystemTrayIcon(this);
 	trayIcon->setContextMenu(trayIconMenu);
 	QIcon icon;
 	icon.addPixmap(QPixmap(QString::fromUtf8(":/Recorder/Resources/current.png")), QIcon::Normal, QIcon::Off);
 	trayIcon->setIcon(icon);
 	trayIcon->setToolTip(tr("The Tape"));
 	setWindowIcon(icon);
 	trayIcon->show();

	connect(restoreAction,	SIGNAL(triggered()), this, SLOT(showNormal()));
	connect(minimizeAction,	SIGNAL(triggered()), this, SLOT(hide()));
	connect(quitAction,		SIGNAL(triggered()), this, SLOT(close()));
	connect(maximizeAction,	SIGNAL(triggered()), this, SLOT(showMaximized()));
	return;
}

void DLCRMain::actionBackUputility_clicked()
{
	WriteToDisk *diskWriter=new WriteToDisk(this);
	diskWriter->exec();
	delete diskWriter;
}
void DLCRMain::actionRecorderSettings_clicked()
{
	Wizard *wizard=new Wizard(this);
	wizard->checkDBConnection();
	wizard->exec();
	delete wizard;

// 	if(ui.recorder->graphRunning())
// 	{
// 		ui.recorder->setbuildState(false);
// 	}

	SHOW_MESSAGE(tr("TAPE RESTART MESSAGE"),tr("Restart the Tape to take the change effective!!!"),tr("Ok"));
	ui.actionNew_OpenCase->setEnabled(false);
}
void DLCRMain::actionLog_Editor_clicked()
{
	if (ui.actionLog_Editor->isChecked())
	{
		ui.tabEvents->setTabEnabled(0,true);
		ui.tabEvents->setHidden(false);
		ui.StatusFrame->setVisible(true);
		ui.tabEvents->setCurrentIndex(0);
	}
	else
	{
		ui.tabEvents->setHidden(true);
		ui.StatusFrame->setVisible(false);
	}
}
void DLCRMain::actionRecorder_clicked()
{
	if (ui.actionRecorder->isChecked())
	{
		ui.dockWidget->setHidden(false);
	}
	else
	{
		ui.dockWidget->setHidden(true);
	}
}
void DLCRMain::actionSpell_Check_Tool_clicked()
{
    SpellChecker *spellWindow=new SpellChecker(this);

	if (ui.tabEvents->isHidden()==false)
	{
		if(ui.tabEvents->currentIndex()==0)
		{
 			if(ui.activeEditor->ui.tblEventEditor->currentItem()!=NULL)
 			{
				spellWindow->ui.btnApply->setEnabled(true);
				int row=ui.activeEditor->ui.tblEventEditor->currentRow();
				int column=ui.activeEditor->ui.tblEventEditor->currentColumn();
				ui.activeEditor->ui.tblEventEditor->setCurrentCell(row,0,QItemSelectionModel::SelectCurrent);
				ui.activeEditor->ui.tblEventEditor->setCurrentCell(row,column,QItemSelectionModel::SelectCurrent);
				spellWindow->setText(ui.activeEditor->ui.tblEventEditor->currentItem()->text());
 			}
			else
			{		
				spellWindow->ui.btnApply->setEnabled(false);
				spellWindow->setText("");				
			}
		}
		else
		{		
			if(ui.activeRetriever->ui.tblEventRetriever->currentItem()!=NULL && logSession->canEditRetrieved())
 			{
				spellWindow->ui.btnApply->setEnabled(true);
				int row=ui.activeRetriever->ui.tblEventRetriever->currentRow();
				int column=ui.activeRetriever->ui.tblEventRetriever->currentColumn();
				ui.activeRetriever->ui.tblEventRetriever->setCurrentCell(row,0,QItemSelectionModel::SelectCurrent);
				ui.activeRetriever->ui.tblEventRetriever->setCurrentCell(row,column,QItemSelectionModel::SelectCurrent);
				spellWindow->setText(ui.activeRetriever->ui.tblEventRetriever->currentItem()->text());
 			}
			else
			{
				spellWindow->ui.btnApply->setEnabled(false);	
				spellWindow->setText("");				
			}
		}
	}
	else
	{
		spellWindow->ui.btnApply->setEnabled(false);	
		spellWindow->setText("");
	}

    spellWindow->exec();
   	delete spellWindow;
}
bool DLCRMain::eventFilter(QObject *obj_, QEvent *event_)
{
	if (event_->type() == QEvent::KeyPress) 
	{

		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event_);
		
		if(keyEvent->key()==Qt::Key_Escape)
		{
//			ui.recorder->fullScreen(false);
			return true;
		}
		else if(keyEvent->key()==Qt::Key_Tab)
		{
// 			ui.recorder->fullScreen(false);
			return true;
		}
		else
		{
			return false;
		}
	}
	else 
	{
		return false;
	}
}
void DLCRMain::tabEvents_currentChanged(int index_)
{
	if(index_!=0)
	{
		ui.tabEvents->setTabsClosable(true);
		if(logSession->canExportToFile()) ui.menuExportNotes->setEnabled(true);
		if(logSession->canPrintAccess()) 
		{
			ui.actionPrintNotes->setEnabled(true);
			ui.actionPrint_Preview->setEnabled(true);
			ui.actionPrint_Case_Log->setEnabled(true);
			ui.actionPrint_Priview->setEnabled(true);
			
		}
		//ui.menuFormat_Style->setEnabled(true);
		ui.actionBold->setEnabled(true);
		ui.actionItalics->setEnabled(true);
		ui.actionUnderline->setEnabled(true);
		ui.actionNew_Notes->setEnabled(true);
		ui.actionSpellChecker->setEnabled(true);	
		ui.actionImport->setEnabled(true);
		ui.actionSpell_Check_Tool->setEnabled(true);
		ui.actionNew_Notes->setEnabled(false);
		ui.menuInsert_Row->setEnabled(false);
		ui.actionCut->setEnabled(false);
		ui.actionCopy->setEnabled(false);
		ui.actionPaste->setEnabled(false);
		ui.actionFiles->setEnabled(false);
		ui.actionImages->setEnabled(false);
		ui.actionBooks->setEnabled(false);
		ui.actionLinks->setEnabled(false);
		ui.actionAttach_File->setEnabled(false);
		ui.actionGoto->setEnabled(false);
		ui.actionFindNotes->setEnabled(false);
		ui.actionReplace->setEnabled(false);
	}
	else
	{
		ui.tabEvents->setTabsClosable(false);	
		ui.menuExportNotes->setEnabled(false);
		ui.actionPrintNotes->setEnabled(false);
		ui.actionPrint_Preview->setEnabled(false);
		ui.actionPrint_Case_Log->setEnabled(false);
		ui.actionPrint_Priview->setEnabled(false);
		ui.actionNew_Notes->setEnabled(true);
		ui.actionFiles->setEnabled(false);
		ui.actionImages->setEnabled(false);
		ui.actionBooks->setEnabled(false);
		ui.actionLinks->setEnabled(false);
		ui.actionCut->setEnabled(false);
		ui.actionCopy->setEnabled(false);
		ui.actionPaste->setEnabled(false);
		ui.menuFormat_Style->setEnabled(false);
		ui.actionAttach_File->setEnabled(false);
		ui.actionGoto->setEnabled(false);
		ui.actionFindNotes->setEnabled(false);
		ui.actionReplace->setEnabled(false);

	}
}
void DLCRMain::tabEvents_closeRequested(int index_)
{
	if (index_==1)
	{
		ui.tabEvents->removeTab(1);
		ui.tabEvents->setTabEnabled(0,true);
		ui.actionDelete_Row->setEnabled(false);
		if(ui.actionLog_Editor->isChecked()!=true)
		{
			ui.tabEvents->setHidden(true);
			ui.StatusFrame->setVisible(false);
			
			ui.actionDownload_file->setEnabled(false);	
			ui.actionAttach_File->setEnabled(false);
			ui.actionText_Highlight->setEnabled(false);
			ui.actionNew_Notes->setEnabled(false);
			ui.actionImport->setEnabled(false);
			
		}
		else
		{
			ui.actionText_Highlight->setEnabled(true);
			ui.actionNew_Notes->setEnabled(true);
			ui.actionImport->setEnabled(true);
		}
		ui.menuInsert_Row->setEnabled(false);
		ui.actionFiles->setEnabled(false);
		ui.actionImages->setEnabled(false);
		ui.actionBooks->setEnabled(false);
		ui.actionLinks->setEnabled(false);
		ui.actionCut->setEnabled(false);
		ui.actionCopy->setEnabled(false);
		ui.actionPaste->setEnabled(false);
		ui.menuFormat_Style->setEnabled(false);
		ui.actionSpellChecker->setEnabled(false);	
		ui.actionSpell_Check_Tool->setEnabled(false);
		ui.actionAttach_File->setEnabled(false);
		ui.actionGoto->setEnabled(false);
		ui.actionFindNotes->setEnabled(false);
		ui.actionReplace->setEnabled(false);
		
	}
}
void DLCRMain::actionRetrieve_clicked()
{
	EventNoteUtil *util=new EventNoteUtil(this);

	try
	{
		ui.tabEvents->setTabEnabled(0,true);

		if(ui.tabEvents->widget(1)==NULL) ui.tabEvents->insertTab(1,ui.tabRetriever,tr("Retrieved Case Notes"));

		ui.activeRetriever->createEditor();

		caseDetails currentDetails=ui.activeEditor->getCaseDetails();

		retrieveDetails details;

 		details.no=currentDetails.no;
 		details.title=currentDetails.title;
 		details.description=currentDetails.description;
 		details.dt=currentDetails.dt;
 		details.eventType=currentDetails.eventType;

		ui.activeRetriever->setEventDetails(details);

		logSession->setRetrievedID(logSession->getEventID());
		logSession->setRetrievedLocation(currentDetails.location);
		logSession->setRetrievedRoom(currentDetails.room);
		
		QList<EventNoteInfo> allEventNotes=util->selectAllEventNotes(logSession->getEventID(),logSession->getCurrentRoom(),logSession->getDriverName());

		ui.activeRetriever->insertRows(allEventNotes);

		if(logSession->canEditRetrieved())
		{
			ui.activeRetriever->setEditable(true);	
			if(logSession->canAttachFile()) ui.actionAttach_File->setEnabled(true);
			if(logSession->canDownloadFile()) ui.actionDownload_file->setEnabled(true);
		}
		else 
		{
			ui.activeRetriever->setEditable(false);	
			ui.actionAttach_File->setEnabled(false);
			ui.actionDownload_file->setEnabled(false);
		}
		
		shortcuts->refreshDialog();	
		shortcuts->applyShortcuts();

		ui.actionPrint_Case_Log->setEnabled(true);
		ui.actionPrint_Priview->setEnabled(true);
		
		ui.tabEvents->setCurrentIndex(1);
		ui.StatusFrame->setVisible(true);
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
}
void DLCRMain::actionNew_OpenCase_clicked()
{
	bool commitStatus=true;

	if(tempDB->connectDatabase()==false) 
	{
		SHOW_MESSAGE(tr("TAPE ERROR"),tr("Database connection failed!!!"),tr("Ok"));
		PRINT_ERROR("Failed to create SQLLite Database connection!!!");
			return;
	}

	if(tempDB->checkDataExistence()!=false)
	{
		commitStatus=tempDB->commitAllData();
	}	

	if(commitStatus!=false)
	{
		explore->loadUserDetails();
		explore->exec();
	}
}
// bool DLCRMain::isValidLicense()
// {
// 	LicenseUtil *util=new LicenseUtil(this);
// 	bool licenseStatus;
// 
// 	try
// 	{
// 		SystemFetch *fetch=new SystemFetch();
// 		PCUtil *pUtil=new PCUtil(this);
// 
// 		PCInfo pInfo=pUtil->loadPCDetails(fetch->getSystemID(),logSession->getDriverName());
// 
// 		LicenseInfo info=util->loadLicence(pInfo.getPCID(),logSession->getDriverName());
// 
// 		if(info.getLicenceID()!=0)
// 		{
// 			engine->readLicenseFrom(info.getLicenceFilePath());
// 		
// 			KeyGen *key=new KeyGen();
// 
// 			if(engine->details.keyList.contains(key->slNumber(),Qt::CaseSensitive) && engine->details.licenseType=="Recorder")
// 			{
// 				if(engine->details.validFrom.daysTo(QDate::currentDate())<=engine->details.validFrom.daysTo(engine->details.validUntil))
// 				{
// 					licenseStatus=true;
// 					if(engine->details.license=="Demo")
// 						SHOW_INFOMESSAGE(tr("Evidencer License"),QString("<b>").append(tr("Evidencer demo license"))+"</b>","Valid for only "+QString::number(QDate::currentDate().daysTo(engine->details.validUntil))+" days",tr("Ok"));
// 				}
// 				else
// 				{
// 					licenseStatus=false;
// 					SHOW_CRITICAL_ERROR(tr("Evidencer License"),"<b>"+tr("Your license has expired")+"</b>",tr("Licensing for this product has expired!"),tr("Ok"));
// 				}
// 			}
// 			else
// 			{
// 				licenseStatus=false;
// 				SHOW_CRITICAL_ERROR(tr("Evidencer License"),"<b>"+tr("Invalid License Key")+"</b>",tr("Your license key for this product is invalid!"),tr("Ok"));
// 			}
// 
// 			delete key;
// 		}
// 		else
// 		{
// 
// 			licenseStatus=false;
// 		}
// 
// 		delete pUtil;
// 		delete fetch;	
// 	}
// 	catch(DBExeption e)//catching db exeptions
// 	{
// 		if(e==ExecutionFailed)
// 		{
// 			PRINT_ERROR("insertion failed");	
// 		}
// 		if(e==UnableToConnect)
// 		{
// 			PRINT_ERROR("Unable to connect to db");	
// 		}	
// 		if(e==EmptyResultSet)
// 		{
// 			PRINT_WARNING("Empty result set");
// 		}
// 	}
// 	catch(...)//catching unknown exceptions
// 	{
// 		PRINT_ERROR("UNKnown ERROR");
// 	}
// 
// 	delete util;
// 
// 	return licenseStatus;
// }
bool DLCRMain::canLoadSettings()
{
	AppSettingsUtil *util=new AppSettingsUtil(this);
	HashManager *generate=new HashManager(this);
	AudioDeviceUtil *audioUtil=new AudioDeviceUtil(this);
	VideoDeviceUtil *videoUtil=new VideoDeviceUtil(this);

	try
	{
		AppSettingsInfo info=util->loadAppSettings(logSession->getPCID(),logSession->getDriverName());
		QList<VideoDeviceinfo> videoList=videoUtil->selectVideoDevice(info.getAppSettingsID(),1,logSession->getDriverName());
		int vlist;
		QStringList vStringList;
		foreach(VideoDeviceinfo vinfo,videoList)
		{
			vStringList<<vinfo.getVideoDeviceName();
		}
		vlist=vStringList.count();
		QList<AudioDeviceinfo> audioList=audioUtil->selectAudioDevice(info.getAppSettingsID(),1,logSession->getDriverName());
		int alist;
		QStringList aStringList;
		foreach(AudioDeviceinfo ainfo,audioList)
		{
			aStringList<<ainfo.getAudioDeviceName();	
		}
		alist=aStringList.count();
		ui.recorder->setChannelCount(alist*2);
		ui.recorder->setvideoDevice(vStringList);
		ui.recorder->setNChannels(vlist);
		ui.widget->setChannels(alist*2);
		QStringList profile;
		QStringList fileName;
		for(int i=0;i<vStringList.count();i++)
		{
			profile<<"video_"+QString::number(i)+".prx";
			QString file=QDir::homePath()+"/tmp_"+QString::number(i)+".avl";
			file=file.replace("/","\\");
			fileName<<file;
		}
		ui.recorder->setProfileFile(profile);
		ui.recorder->setFileName(fileName);		
		splittingDuration=info.getSplittingDuration();
		ui.recorder->setDeviceName(aStringList);
	
		if(NOT ui.recorder->isGraphBuildSucess())
		{
			SHOW_CRITICAL_ERROR(tr("ERROR!!!"),tr("TAPE Failed to Start."),tr("Check your Recording devices and the media Settings are provided correctly..."),tr("Ok"));
			
			delete generate;
			delete util;
			return false;
		}
		
		logSession->setSharedPath(info.getSharedServerPath());
		logSession->setServerIP(info.getSharedServerIP());
		logSession->setServerAdminName(info.getServerUserName());
		logSession->setServerAdminPassword(generate->decrypts(info.getServerUserPassword()));

		logSession->setFTPServer(info.getFTPHostName());
		logSession->setFTPPort(info.getFTPPortNo());
		logSession->setFTPAdmin(info.getFTPUserName());
		logSession->setFTPPassword(generate->decrypts(info.getFTPUserPassword()));
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

	delete generate;
	delete util;
	return true;
}
void DLCRMain::sendMailToAdmin(int percentage_)
{

	MailSettingsUtil *util=new MailSettingsUtil(this);
	MailDetailsUtil *mUtil=new MailDetailsUtil(this);

	QString settingsPath=QDir::homePath()+"/Application Data/Evidencer/Settings/UserConfig.ini";

	QSettings settings(settingsPath,QSettings::IniFormat);

	if(percentage_>70 && percentage_<=80)
	{
		if(settings.value("70TO80","NotSend").toString()=="Send")
		{
			return;
		}
	}
	else if(percentage_>80 && percentage_<=90)
	{
		if(settings.value("80TO90","NotSend").toString()=="Send")
		{
			return;
		}
	}

	try
	{
		MailSettingsInfo info=util->loadMailSettings(1,logSession->getDriverName());
		HashManager *generate=new HashManager(this);		
		QString passWord=generate->decrypts(info.getFromPassword());
		delete generate;
		
		QStringList toMailList=info.getToMailIDs().split(",");

		for(int i=0;i<toMailList.length();i++)
		{
			QStringList mailDetails;
			mailDetails<<info.getSMTPServerName();
			mailDetails<<info.getFromMailID();
			mailDetails<<info.getFromUserName();
			mailDetails<<"stpl«";
			mailDetails<<passWord;
			mailDetails<<QString::number(info.getPortNo());
			mailDetails<<toMailList[i];
			mailDetails<<logSession->getPCName();
			QProcess *callSendMail = new QProcess(this);
			callSendMail->start("IMCHK.exe", mailDetails);		
			callSendMail->waitForStarted();
			if(callSendMail->error()==QProcess::FailedToStart)
			{
				SHOW_INFOMESSAGE(tr("ERROR!!!"),tr("Some files are missing in ")+QDir::currentPath(),tr("Please re-install the application to fix the problem."),tr("Ok"));
				qApp->processEvents();
				return;
			}
			callSendMail->waitForFinished(10000);
			QByteArray status=callSendMail->readAll();
			QString sendStatus(status);
			if(sendStatus.contains("true",Qt::CaseInsensitive))
			{
				MailDetailsInfo mInfo;
				mInfo.setFromMailID(info.getFromMailID());
				mInfo.setToMailID(toMailList[i]);
				mInfo.setMailDescription("Disk space used has exceeded 70% Check point");
				mInfo.setPCID(logSession->getPCID());
				mInfo.setSendedDate(sharedObj->getCurrentDateTime());
				mUtil->createMailDetail(mInfo,logSession->getDriverName());

				if(percentage_>70 && percentage_<=80)
				{
					settings.setValue("70TO80","Send");				
				}
				else if(percentage_>80 && percentage_<=90)
				{
					settings.setValue("80TO90","Send");				
				}
			}
			else
			{
				if(settings.value("MailMessage","1").toInt()==1)
				{
					createMailMessage();
					errorMessage->exec();
				}
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

	delete mUtil;
	delete util;
}
void DLCRMain::createMailMessage()
{
	errorMessage=new QDialog(this);	
	if (errorMessage->objectName().isEmpty())
	errorMessage->setObjectName(QString::fromUtf8("errorMessage"));
	errorMessage->resize(318, 88);
	horizontalLayoutWidget = new QWidget(errorMessage);
	horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
	horizontalLayoutWidget->setGeometry(QRect(20, 50, 281, 25));
	horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
	horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
	horizontalLayout->setContentsMargins(0, 0, 0, 0);
	chkBoxStatus = new QCheckBox(horizontalLayoutWidget);
	chkBoxStatus->setObjectName(QString::fromUtf8("chkBoxStatus"));

	horizontalLayout->addWidget(chkBoxStatus);

	horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	horizontalLayout->addItem(horizontalSpacer);

	pbtnOk = new QPushButton(horizontalLayoutWidget);
	pbtnOk->setObjectName(QString::fromUtf8("pbtnOk"));

	horizontalLayout->addWidget(pbtnOk);

	horizontalLayoutWidget_2 = new QWidget(errorMessage);
	horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
	horizontalLayoutWidget_2->setGeometry(QRect(20, 20, 281, 20));
	horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
	horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
	horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
	lblMessage = new QLabel(horizontalLayoutWidget_2);
	lblMessage->setObjectName(QString::fromUtf8("lblMessage"));
	QFont font;
	font.setPointSize(9);
	font.setBold(true);
	font.setWeight(75);
	lblMessage->setFont(font);
	horizontalLayout_2->addWidget(lblMessage);
	retranslateMailMessage(errorMessage);

	connect(pbtnOk,	SIGNAL(clicked()),this,SLOT(messageClose()));
}

void DLCRMain::messageClose()
{
	QString settingsPath=QDir::homePath()+"/Application Data/Evidencer/Settings/UserConfig.ini";

	QSettings settings(settingsPath,QSettings::IniFormat);

	if (chkBoxStatus->isChecked())
	{
		settings.setValue("MailMessage","0");
	}
	errorMessage->close();
}

void DLCRMain::retranslateMailMessage(QDialog *errorMessage)
{
	errorMessage->setWindowTitle(QApplication::translate("errorMessage", "Dialog", 0, QApplication::UnicodeUTF8));
	chkBoxStatus->setText(QApplication::translate("errorMessage", "Don't show this again", 0, QApplication::UnicodeUTF8));
	pbtnOk->setText(QApplication::translate("errorMessage", "Ok", 0, QApplication::UnicodeUTF8));
	lblMessage->setText(QApplication::translate("errorMessage", "Administrator mail settings is not configured.", 0, QApplication::UnicodeUTF8));
} 

void DLCRMain::startEvent()
{		
	DiskAnalizer *analizer=new DiskAnalizer(this);
	int percentage=analizer->getPercentageUsed();
	QString homePath =QDir::homePath();
	QString drive=homePath.left(3);
	double bytesFree = analizer->getFreeSpace(drive);
	double gbFree=(bytesFree/1048576)/1024;

	if (gbFree<0.5)
	{
		SHOW_CRITICAL_ERROR(tr("Limited disk space"),tr("The available space on your hard disk is limited."),tr("Recorder can't start with this limited disk space."),tr("Ok"));
		sendMailToAdmin(percentage);
		delete analizer;
		return;
	}
	else if(percentage>70)
	{
		sendMailToAdmin(percentage);
	}
	else
	{
		QString settingsPath=QDir::homePath()+"/Application Data/Evidencer/Settings/UserConfig.ini";

		QSettings settings(settingsPath,QSettings::IniFormat);
		settings.setValue("70TO80","NotSend");
		settings.setValue("80TO90","NotSend");
	}
	delete analizer;

	tempDetails details;
	details.eventid=logSession->getEventID();
	details.eventtypeid=logSession->getCurrentEventType();
	details.pcid=logSession->getPCID();
	details.roomid=logSession->getCurrentRoom();
	details.sessionid=logSession->getSessionID();
	details.userid=logSession->getUserID();

	tempNotes notes;
	notes.eventTime=sharedObj->getCurrentDateTime().addSecs(-1).toString("dd/MM/yyyy hh:mm:ss AP");
	//notes.filePath=sharedObj->filterToServerPath(logSession->getCurrentFilePath(),logSession->getSharedPath());
	notes.richEventSpeaker="Event Started "+logSession->getEventNo();
	notes.richEventNotes="Auto generated field";
	notes.internalReference="--";
	notes.externalReference="--";
	notes.visibility=0;
	QStringList toServerpath;
	toServerpath=sharedObj->filterToServerPath(logSession->getCurrentFilePath(),logSession->getSharedPath());
	printf("\Entered path...\n");
	tempDB->createTempTables();
	printf("\Created Tables...\n");
	for (int i=0;i<toServerpath.count();i++)
	{
		if(tempDB->startCase(details,notes,toServerpath[i],1)!=true) return;	
	}
	
	printf("\Entered tempDB...\n");
	ui.recorder->spiltTimer(splittingDuration);

	ui.recorder->setFileName(logSession->getCurrentFilePath());
	ui.dockWidget->setHidden(false);

	if(ui.recorder->runGraph())
	{
		ui.activeEditor->createEditor();
		ui.activeEditor->insertRow(0);

		shortcuts->refreshDialog();	
		shortcuts->applyShortcuts(); 

		ui.tabEvents->setHidden(false);
		ui.dockWidget->setHidden(false);

		ui.tabEvents->setCurrentIndex(0);

		ui.actionPause_Resume->setEnabled(true);
		ui.actionNew_OpenCase->setEnabled(false);
		ui.actionBackup_Utility->setEnabled(false);
		ui.actionAdvanced_Search->setEnabled(false);
		ui.actionStop_Case->setEnabled(true);
		ui.actionStop_Case->blockSignals(false);
		ui.actionSave_Notes->setEnabled(true);
		ui.actionImport->setEnabled(true);
		ui.actionInsert->setEnabled(true);
		ui.actionSimilar_Sessions->setEnabled(true);
		ui.menuFormat_Style->setEnabled(true);
		ui.activeEditor->ui.btnBold->setEnabled(true);
		ui.activeEditor->ui.btnItalic->setEnabled(true);
		ui.activeEditor->ui.btnUnderline->setEnabled(true);
		ui.activeRetriever->ui.btnBold->setEnabled(true);
		ui.activeRetriever->ui.btnItalic->setEnabled(true);
		ui.activeRetriever->ui.btnUnderline->setEnabled(true);
		ui.actionNew_Notes->setEnabled(true);
		ui.actionScheduler->setEnabled(false);
		ui.actionRecorderSettings->setEnabled(false);
		ui.actionSpell_Check_Tool->setEnabled(true);
		ui.actionSpellChecker->setEnabled(true);


		if(logSession->canRetrieveEvent()) ui.actionRetrieve->setEnabled(true);
		//if(logSession->canAttachFile()) ui.actionAttach_File->setEnabled(true);
		if(logSession->canDownloadFile()) ui.actionDownload_file->setEnabled(true);

		ui.actionRecorder->setEnabled(true);
		ui.actionLog_Editor->setEnabled(true);
		ui.actionLog_Editor->setChecked(true);
		ui.actionRecorder->setChecked(true);
		ui.actionText_Highlight->setEnabled(true);

		if(ui.tabEvents->widget(1)!=NULL) ui.tabEvents->removeTab(1);

		ui.StatusFrame->setVisible(true);

		recordingStatus=true;

		ui.actionPause_Resume->setIcon(icoPause);
		ui.actionPause_Resume->setText("Pause Case");
		ui.recorder->setPaused(false);

		ui.lblPositionNow->setText("00:00:00");
		myTimer->start(1000);

		history->insertHistory(sharedObj->getCurrentDateTime(),logSession->getEventNo(),"Started");

		trayIcon->setToolTip(logSession->getEventNo()+" Recording...");
	}
	else
	{
		SHOW_CRITICAL_ERROR(tr("ERROR!!!"),tr("An unknown error happened..."),tr("CRS unable to start recording"),tr("Ok"));
		if(tempDB->clearTempTables())
			printf("\nTemporary SQLite tables are truncated successfully.");
		else
			printf("\nUnknown error generated while truncate temporary SQLite tables!!!");
	}

}
void DLCRMain::restartRecording()
{

	printf("\nRestart Recording Begin .. DLCRMain::restartRecording()\n");

	int splitCount=2;
	ui.actionStop_Case->setEnabled(false);
	this->update();

//	QString filePath=logSession->getEventHomePath()+"/"+sharedObj->getEventFileName(1,logSession->getEventNo());
//	logSession->setCurrentFilePath(filePath);

	int videoCount=getVideoCount();
	QStringList filePath=sharedObj->getEventFileName(videoCount,logSession->getEventNo());
	QStringList filepaths;
	for (int i=0;i<videoCount;i++)
	{
		filepaths<<logSession->getEventHomePath()+"/"+filePath[i];
	}
	logSession->setCurrentFilePath(filepaths);
	DiskAnalizer *analizer=new DiskAnalizer(this);
	int percentage=analizer->getPercentageUsed();
	QString homePath =QDir::homePath();
	QString drive=homePath.left(3);
	double bytesFree = analizer->getFreeSpace(drive);
	double gbFree=(bytesFree/1048576)/1024;

	if (gbFree<0.1)
	{
		SHOW_CRITICAL_ERROR(tr("Limited disk space"),tr("The available space on your hard disk is limited."),tr("Recorder can't continue with this limited disk space."),tr("Ok"));
		actionStop_Case_clicked();
		sendMailToAdmin(percentage);
		delete analizer;
		return;
	}
	delete analizer;

	tempNotes notes;
	notes.eventTime=sharedObj->getCurrentDateTime().toString("dd/MM/yyyy hh:mm:ss AP");
//	notes.filePath=sharedObj->filterToServerPath(logSession->getCurrentFilePath(),logSession->getSharedPath());
	notes.richEventSpeaker="Event session started "+logSession->getEventNo();
	notes.richEventNotes="Auto generated field";
	notes.internalReference="--";
	notes.externalReference="--";
	notes.visibility=0;
	QStringList toServerpath;
	toServerpath=sharedObj->filterToServerPath(logSession->getCurrentFilePath(),logSession->getSharedPath());

	for (int i=0;i<toServerpath.count();i++)
	{
		tempDB->insertNotes(notes,toServerpath[i],splitCount);
	}
// 	tempDB->insertNotes(notes);

	if(ui.recorder->reStart(filepaths))
	{
		ui.actionStop_Case->setEnabled(true);
		splitCount++;
	}
	else
	{
		SHOW_CRITICAL_ERROR(tr("ERROR!!!"),tr("An unknown error happened..."),tr("Press Ok to stop processing"),tr("Ok"));
		actionStop_Case_clicked();
	}
	
	printf("\nback from Recording Begin ..DLCRMain::restartRecording() \n");
}
void DLCRMain::actionStop_Case_clicked()
{	
	ui.actionStop_Case->setEnabled(false);
	ui.actionStop_Case->blockSignals(true);

	if(!ui.recorder->stopGraph())
	{
		SHOW_CRITICAL_ERROR(tr("ERROR!!!"),tr("An unknown error happened..."),tr("Unable to stop session"),tr("Ok"));
		PRINT_WARNING("An unknown error happened...");
		ui.actionStop_Case->blockSignals(false);
		ui.actionStop_Case->setEnabled(true);
		return;
	}

//	ui.recorder->stopSpiltTimer();
	tempNotes notes;
	notes.eventTime=sharedObj->getCurrentDateTime().toString("dd/MM/yyyy hh:mm:ss AP");
//	notes.filePath=sharedObj->filterToServerPath(logSession->getCurrentFilePath(),logSession->getSharedPath());
	notes.richEventSpeaker="Event stopped "+logSession->getEventNo();
	notes.richEventNotes="Auto generated field";
	notes.internalReference="--";
	notes.externalReference="--";
	notes.visibility=0;
	QStringList toServerpath;
	toServerpath=sharedObj->filterToServerPath(logSession->getCurrentFilePath(),logSession->getSharedPath());

	for (int i=0;i<toServerpath.count();i++)
	{
		tempDB->insertNotes(notes,toServerpath[i],0);
	}
	//tempDB->insertNotes(notes);

 	if(tempDB->commitAllData()!=true)
 	{
 		SHOW_INFOMESSAGE(tr("ERROR!!!"),tr("Database server connection lost"),tr("Notes are safely saved in local database"),tr("Ok"));
 	}
	ui.activeEditor->clear();
	
	ui.dockWidget->setHidden(true);	
	ui.StatusFrame->setVisible(false);
	ui.tabEvents->setHidden(true);	

	ui.actionNew_OpenCase->setEnabled(true);
	ui.actionBackup_Utility->setEnabled(true);
	ui.actionAdvanced_Search->setEnabled(true);
	ui.actionStop_Case->setEnabled(false);
	ui.actionSave_Notes->setEnabled(false);
	ui.actionBold->setEnabled(false);
	ui.actionItalics->setEnabled(false);
	ui.actionUnderline->setEnabled(false);
	ui.actionRetrieve->setEnabled(false);
	ui.actionAttach_File->setEnabled(false);
	ui.actionDownload_file->setEnabled(false);

	ui.actionLog_Editor->setChecked(false);
	ui.actionRecorder->setChecked(false);
	ui.actionLog_Editor->setEnabled(false);
	ui.actionRecorder->setEnabled(false);

	ui.menuInsert_Row->setEnabled(false);
	ui.actionDelete_Row->setEnabled(false);
	ui.actionImport->setEnabled(false);
	ui.actionCut->setEnabled(false);
	ui.actionCopy->setEnabled(false);
	ui.actionPaste->setEnabled(false);
	ui.actionGoto->setEnabled(false);
	ui.actionFindNotes->setEnabled(false);
	ui.actionReplace->setEnabled(false);
	ui.actionInsert->setEnabled(false);
	ui.actionFiles->setEnabled(false);
	ui.actionImages->setEnabled(false);
	ui.actionBooks->setEnabled(false);
	ui.actionLinks->setEnabled(false);
	ui.actionSimilar_Sessions->setEnabled(false);
	ui.menuFormat_Style->setEnabled(false);
	ui.actionPause_Resume->setEnabled(false);
	ui.actionNew_Notes->setEnabled(false);
	ui.actionScheduler->setEnabled(true);
	ui.actionRecorderSettings->setEnabled(true);
	ui.actionPrint_Case_Log->setEnabled(false);
	ui.actionPrint_Priview->setEnabled(false);
	ui.actionSpell_Check_Tool->setEnabled(false);
	ui.actionSpellChecker->setEnabled(false);

	ui.menuExportNotes->setEnabled(false);
	ui.actionPrintNotes->setEnabled(false);
	ui.actionPrint_Preview->setEnabled(false);
	ui.actionText_Highlight->setEnabled(false);

	recordingStatus=false;

	history->insertHistory(sharedObj->getCurrentDateTime(),logSession->getEventNo(),"Stopped");

	myTimer->stop();
	
	ScheduleDuration->stop();
	ScheduleStop->stop();
	ScheduleProcess->stop();

	trayIcon->setToolTip(tr("CRS Recorder"));

}
void DLCRMain::retrieveEvent(int eventID_,int roomID_)
{
	EventNoteUtil *util=new EventNoteUtil(this);

	try
	{
		ui.tabEvents->setTabEnabled(0,false);

		if(ui.tabEvents->widget(1)==NULL) ui.tabEvents->insertTab(1,ui.tabRetriever,tr("Retrieved Case Notes"));

		ui.activeRetriever->createEditor();

		QList<EventNoteInfo> allEventNotes=util->selectAllEventNotes(eventID_,roomID_,logSession->getDriverName());

		ui.activeRetriever->insertRows(allEventNotes);

		if(logSession->canEditRetrieved())
		{
			ui.activeRetriever->setEditable(true);	
			if(logSession->canAttachFile()) ui.actionAttach_File->setEnabled(true);
			if(logSession->canDownloadFile()) ui.actionDownload_file->setEnabled(true);
		}
		else 
		{
			ui.activeRetriever->setEditable(false);	
			ui.actionAttach_File->setEnabled(false);
			ui.actionDownload_file->setEnabled(false);
		}

		shortcuts->refreshDialog();	
		shortcuts->applyShortcuts();

		ui.tabEvents->setHidden(false);

		ui.tabEvents->setCurrentIndex(1);
		tabEvents_currentChanged(ui.tabEvents->currentIndex());

		ui.StatusFrame->setVisible(true);
		ui.actionText_Highlight->setEnabled(true);

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
}
void DLCRMain::actionEasy_Notes_clicked()
{
	shortcuts->refreshDialog();	
 	shortcuts->show();
}
void DLCRMain::actionOnlineSupport_clicked()
{
	QDesktopServices::openUrl(QUrl("http://evidencer.com"));
}

void DLCRMain::actionOnlineUpdate_clicked()
{
	QDesktopServices::openUrl(QUrl("http://evidencer.com"));
}

void DLCRMain::actionExit_Recorder_clicked()
{
	this->close();
}
void DLCRMain::actionAboutDLCR_clicked()
{
	AboutCRS *crsDialog=new AboutCRS(this);

	crsDialog->exec();

	delete crsDialog;
}
void DLCRMain::actionAttachFile_clicked()
{
	FilesDetails *file=new FilesDetails(this);

	if(ui.tabEvents->currentIndex()!=0)
	{
		int row=ui.activeRetriever->getRetrieveRow();
		QString rowtime=ui.activeRetriever->getCurrentRetrieveTime(row);
		file->setRowTime(rowtime);
		file->setTab(1);
	}
	else
	{
		int row=ui.activeEditor->getCurrentRow();
		QString rowtime=ui.activeEditor->getCurrentRowTime(row);
		file->setRowTime(rowtime);
		file->setTab(0);		
	}

	delete file;
	ui.menuInsert_Row->setEnabled(false);
	ui.actionDelete_Row->setEnabled(false);
	ui.actionFiles->setEnabled(false);
	ui.actionImages->setEnabled(false);
	ui.actionBooks->setEnabled(false);
	ui.actionLinks->setEnabled(false);
	ui.actionCut->setEnabled(false);
	ui.actionCopy->setEnabled(false);
	ui.actionPaste->setEnabled(false);
	ui.actionAttach_File->setEnabled(false);
	this->setDisabled(false);

}

void DLCRMain::actionDownloadFile_clicked()
{
	FileDownloader *download=new FileDownloader(this);

	if(ui.tabEvents->currentIndex()!=0)
	{
		download->refreshDialog(logSession->getRetrievedID());
	}
	else
	{
		download->refreshDialog(logSession->getEventID());		
	}

	delete download;
}
void DLCRMain::closeEvent(QCloseEvent *event_)
{

	if (recordingStatus!=false)
	{
		QMessageBox msgBox(this);
		msgBox.setWindowTitle(tr("Warning!"));
		QString Message(tr("Recording is going on , Please stop it and proceed!"));
		msgBox.setIcon(QMessageBox::Information);
		msgBox.setText(Message);
		QPushButton *okButton = msgBox.addButton(tr("Ok"), QMessageBox::ActionRole);
		msgBox.exec();
		if (msgBox.clickedButton() == okButton)
		{
			msgBox.close();
			exitMessageBox.close();
			event_->ignore();	
			return;
		}
	}

	exitMessageBox.exec();
	if (exitMessageBox.clickedButton() == cancelButton)
	{
		exitMessageBox.close();
		event_->ignore();	
	}
	else if (exitMessageBox.clickedButton() == trayButton)
	{
		if (trayIcon->isVisible())
		{
			QMessageBox msgBox(this);
			msgBox.setWindowTitle(tr("TAPE System Tray"));
			QString Message(tr("TAPE will keep running in the system tray."));
			msgBox.setIcon(QMessageBox::Information);
			msgBox.setText(Message);
			QPushButton *okButton = msgBox.addButton(tr("Ok"), QMessageBox::ActionRole);
			msgBox.exec();
			if (msgBox.clickedButton() == okButton)
			{
				msgBox.close();
			}
		}

		this->hide();
		event_->ignore();
	}	
	else
	{
		exitMessageBox.close();
//		ui.recorder->stopGraph();
		this->close();
	}
}
void DLCRMain::actionSaveToServer_clicked()
{
	if(tempDB->commitData()!=true)
	{
		SHOW_INFOMESSAGE(tr("ERROR!!!"),tr("Database server connection lost"),tr("Notes are safely saved in local database"),tr("Ok"));
	}
	else
	{
		history->insertHistory(sharedObj->getCurrentDateTime(),logSession->getEventNo(),"Saved");
	}
}
void DLCRMain::actionFTPUploader_clicked()
{
	FTPManager *ftp=new FTPManager(this);

	ftpDetails details;
	details.hostName=logSession->getFTPServer();
	details.userName=logSession->getFTPAdmin();
	details.password=logSession->getFTPPassword();
	details.portNo=logSession->getFTPPort();

	ftp->refresh(details);

	ftp->exec();

	delete ftp;
}
void DLCRMain::actionLoginHistory_clicked()
{
	history->setUser(logSession->getUserName());

	history->exec();
}
void DLCRMain::actionHelp_clicked()
{
// 	Recorderhelp *h=new Recorderhelp(this);
// 	h->exec();
// 	delete h;
}
void DLCRMain::actionAdvanced_Search_clicked()
{
	bool commitStatus=true;

	if(tempDB->connectDatabase()==false) 
	{
		SHOW_MESSAGE(tr("DLCRS ERROR"),tr("Database connection failed!!!"),tr("Ok"));
		PRINT_ERROR("Failed to create SQLLite Database connection!!!");
			return;
	}

	if(tempDB->checkDataExistence()!=false)
	{
		commitStatus=tempDB->commitAllData();
	}	

	if(commitStatus!=false)
	{
		AdvancedSearch *search=new AdvancedSearch(this);
		search->exec();
		delete search;
	}
}
void DLCRMain::actionDiskAnalizer_click()
{
	DiskAnalizer *analizer=new DiskAnalizer(this);
	analizer->exec();
	delete analizer;
}
void DLCRMain::setAudioChannelCount(int count_)
{
	audioCount=count_;
}
int DLCRMain::getAudioChannelCount()
{
	return audioCount;
}
int DLCRMain::getVideoCount()
{
	VideoDeviceUtil *videoUtil=new VideoDeviceUtil(this);
	AppSettingsUtil *util=new AppSettingsUtil(this);

	AppSettingsInfo info=util->loadAppSettings(logSession->getPCID(),logSession->getDriverName());
	QList<VideoDeviceinfo> videoList=videoUtil->selectVideoDevice(info.getAppSettingsID(),1,logSession->getDriverName());

	QStringList vStringList;
	foreach(VideoDeviceinfo vinfo,videoList)
	{
		vStringList<<vinfo.getVideoDeviceName();

	}
	vCount=vStringList.count();
	return vCount;
}

void DLCRMain::actionNew_Notes_clicked()
{
	if(ui.tabEvents->currentIndex()==0)
	{
		if (ui.activeEditor->ui.tblEventEditor->rowCount()==0)
		{
			ui.activeEditor->ui.tblEventEditor->insertRow(0);
			ui.activeEditor->ui.tblEventEditor->activate(0);
		}
		else
		{
			int row=ui.activeEditor->ui.tblEventEditor->rowCount();
			ui.activeEditor->ui.tblEventEditor->insertRow(row);
			ui.activeEditor->ui.tblEventEditor->activate(row);
		}	
	}
	else
	{
		if (ui.activeRetriever->ui.tblEventRetriever->rowCount()==0)
		{
			ui.activeRetriever->ui.tblEventRetriever->insertRow(0);
			ui.activeRetriever->ui.tblEventRetriever->activate(0);
		}
		else
		{
			int row=ui.activeRetriever->ui.tblEventRetriever->rowCount();
			ui.activeRetriever->ui.tblEventRetriever->insertRow(row);
			ui.activeRetriever->ui.tblEventRetriever->activate(row);
		}	
	}
	ui.menuInsert_Row->setEnabled(false);
	ui.actionDelete_Row->setEnabled(false);
	ui.actionFiles->setEnabled(false);
	ui.actionImages->setEnabled(false);
	ui.actionBooks->setEnabled(false);
	ui.actionLinks->setEnabled(false);
	ui.actionCut->setEnabled(false);
	ui.actionCopy->setEnabled(false);
	ui.actionPaste->setEnabled(false);
	ui.actionAttach_File->setEnabled(false);

}
void DLCRMain::actionAfter_Rowclicked()
{
	if(ui.tabEvents->currentIndex()==0)
	{
		if (ui.activeEditor->ui.tblEventEditor->rowCount()==0)
		{
			ui.activeEditor->insertAfterRow(0);
		}
		else
		{
			int row=ui.activeEditor->getActiveRow();
			ui.activeEditor->insertAfterRow(row);
		}	
	}
	else
	{
		int row=ui.activeRetriever->getRetrieveRow();
		ui.activeRetriever->insertAfterRow(row);
	}
	ui.menuInsert_Row->setEnabled(false);
	ui.actionDelete_Row->setEnabled(false);
	ui.actionFiles->setEnabled(false);
	ui.actionImages->setEnabled(false);
	ui.actionBooks->setEnabled(false);
	ui.actionLinks->setEnabled(false);
	ui.actionCut->setEnabled(false);
	ui.actionCopy->setEnabled(false);
	ui.actionPaste->setEnabled(false);	
	ui.actionAttach_File->setEnabled(false);
}
void DLCRMain::actionBefore_Rowclicked()
{
	if(ui.tabEvents->currentIndex()==0)
	{
		if (ui.activeEditor->ui.tblEventEditor->rowCount()==0)
		{
			ui.activeEditor->insertBeforeRow(0);
		}
		else
		{
			int row=ui.activeEditor->getActiveRow();
			ui.activeEditor->insertBeforeRow(row);
		}	
	}
	else
	{
		int row=ui.activeRetriever->getRetrieveRow();
		ui.activeRetriever->insertBeforeRow(row);
	}
	ui.menuInsert_Row->setEnabled(false);
	ui.actionDelete_Row->setEnabled(false);
	ui.actionFiles->setEnabled(false);
	ui.actionImages->setEnabled(false);
	ui.actionBooks->setEnabled(false);
	ui.actionLinks->setEnabled(false);
	ui.actionCut->setEnabled(false);
	ui.actionCopy->setEnabled(false);
	ui.actionPaste->setEnabled(false);
	ui.actionAttach_File->setEnabled(false);
	
}
void DLCRMain::actionDelete_Row_clicked()
{
	if(ui.tabEvents->currentIndex()==0)
	{
		int row=ui.activeEditor->getActiveRow();
		ui.activeEditor->deleteNotes(row);
		ui.activeEditor->remove(row);
	}
	else
	{
		int row=ui.activeRetriever->getRetrieveRow();
		int eventnoteID=ui.activeRetriever->ui.tblEventRetriever->item(ui.activeRetriever->ui.tblEventRetriever->currentRow(),0)->data(Qt::UserRole).toInt();
		ui.activeRetriever->deleteFromRow(eventnoteID);
		ui.activeRetriever->ui.tblEventRetriever->removeRow(row);
	}
	ui.menuInsert_Row->setEnabled(false);
	ui.actionDelete_Row->setEnabled(false);
	ui.actionFiles->setEnabled(false);
	ui.actionImages->setEnabled(false);
	ui.actionBooks->setEnabled(false);
	ui.actionLinks->setEnabled(false);
	ui.actionCut->setEnabled(false);
	ui.actionCopy->setEnabled(false);
	ui.actionPaste->setEnabled(false);
	ui.actionAttach_File->setEnabled(false);
}
void DLCRMain::actionImport_clicked()
{
	if(ui.tabEvents->currentIndex()!=0)
	{
		ImportNotes *import=new ImportNotes(this);
		import->setTabIndex(1);
		delete import;
	}
	else
	{
		ImportNotes *import=new ImportNotes(this);
		import->setTabIndex(0);
		delete import;
	}
}

void DLCRMain::actionFindNotes_clicked()
{
	if(ui.tabEvents->currentIndex()!=0)
	{
		Find *fin=new Find(this);
		fin->exec();
		findStr=fin->findText();
		ui.activeRetriever->searchTxt(findStr);
		delete fin;
	}
	else
	{
		Find *fin=new Find(this);
		fin->exec();
		findStr=fin->findText();
		ui.activeEditor->searchTxt(findStr);
		delete fin;
	}	
}

void DLCRMain::actionInsert_clicked()
{

}
void DLCRMain::actionFiles_clicked()
{
	FilesDetails *file=new FilesDetails(this);

	if(ui.tabEvents->currentIndex()!=0)
	{
		int row=ui.activeRetriever->getRetrieveRow();
		QString rowtime=ui.activeRetriever->getCurrentRetrieveTime(row);
		file->setRowTime(rowtime);
		file->setTab(1);
	}
	else
	{
		int row=ui.activeEditor->getCurrentRow();
		QString rowtime=ui.activeEditor->getCurrentRowTime(row);
		file->setRowTime(rowtime);
		file->setTab(0);		
	}

	delete file;
	ui.menuInsert_Row->setEnabled(false);
	ui.actionDelete_Row->setEnabled(false);
	ui.actionFiles->setEnabled(false);
	ui.actionImages->setEnabled(false);
	ui.actionBooks->setEnabled(false);
	ui.actionLinks->setEnabled(false);
	ui.actionCut->setEnabled(false);
	ui.actionCopy->setEnabled(false);
	ui.actionPaste->setEnabled(false);
	ui.actionAttach_File->setEnabled(false);

}
void DLCRMain::actionImages_clicked()
{
	ImageDetails *image=new ImageDetails(this);

	if(ui.tabEvents->currentIndex()!=0)
	{
		int row=ui.activeRetriever->getRetrieveRow();
		QString rowtime=ui.activeRetriever->getCurrentRetrieveTime(row);
		image->setActiveRowTime(rowtime);
		image->setTab(1);
	}
	else
	{
		int row=ui.activeEditor->getCurrentRow();
		QString rowtime=ui.activeEditor->getCurrentRowTime(row);
		image->setActiveRowTime(rowtime);
		image->setTab(0);		
	}

	delete image;
	
	ui.menuInsert_Row->setEnabled(false);
	ui.actionDelete_Row->setEnabled(false);
	ui.actionFiles->setEnabled(false);
	ui.actionImages->setEnabled(false);
	ui.actionBooks->setEnabled(false);
	ui.actionLinks->setEnabled(false);
	ui.actionCut->setEnabled(false);
	ui.actionCopy->setEnabled(false);
	ui.actionPaste->setEnabled(false);
	ui.actionAttach_File->setEnabled(false);
}
void DLCRMain::actionBooks_clicked()
{
	BookDetails *book=new BookDetails(this);

	if(ui.tabEvents->currentIndex()!=0)
	{	
		int row=ui.activeRetriever->getRetrieveRow();
		QString rowtime=ui.activeRetriever->getCurrentRetrieveTime(row);
		book->setRowTime(rowtime);
		book->setTab(1);		
	}
	else
	{
		int row=ui.activeEditor->getCurrentRow();
		QString rowtime=ui.activeEditor->getCurrentRowTime(row);
		book->setRowTime(rowtime);
		book->setTab(0);		
	}
	delete book;

	ui.menuInsert_Row->setEnabled(false);
	ui.actionDelete_Row->setEnabled(false);
	ui.actionFiles->setEnabled(false);
	ui.actionImages->setEnabled(false);
	ui.actionBooks->setEnabled(false);
	ui.actionLinks->setEnabled(false);
	ui.actionCut->setEnabled(false);
	ui.actionCopy->setEnabled(false);
	ui.actionPaste->setEnabled(false);
	ui.actionAttach_File->setEnabled(false);
}
void DLCRMain::actionLinks_clicked()
{
	LinkDetails *link=new LinkDetails(this);

	if(ui.tabEvents->currentIndex()!=0)
	{
		int row=ui.activeRetriever->getRetrieveRow();
		QString rowtime=ui.activeRetriever->getCurrentRetrieveTime(row);
		link->setRowTime(rowtime);
		link->setTab(1);
	}
	else
	{
		int row=ui.activeEditor->getCurrentRow();
		QString rowtime=ui.activeEditor->getCurrentRowTime(row);
		link->setRowTime(rowtime);
		link->setTab(0);		
	}

	delete link;

	ui.menuInsert_Row->setEnabled(false);
	ui.actionDelete_Row->setEnabled(false);
	ui.actionFiles->setEnabled(false);
	ui.actionImages->setEnabled(false);
	ui.actionBooks->setEnabled(false);
	ui.actionLinks->setEnabled(false);
	ui.actionAttach_File->setEnabled(false);
}
void DLCRMain::actionCut_clicked()
{
	if(ui.tabEvents->currentIndex()!=0)
	{
		QString str;
		QList<QStringList> selectedRows;
		QList<int> selectedIndex;
		for(int i=0;i<ui.activeRetriever->ui.tblEventRetriever->rowCount();i++)
		{
			if(ui.activeRetriever->ui.tblEventRetriever->item(i,1)->isSelected())
			{
				QStringList tmpList;
				tmpList<<ui.activeRetriever->ui.tblEventRetriever->item(i,0)->text();
				tmpList<<ui.activeRetriever->ui.tblEventRetriever->item(i,1)->text();
				tmpList<<ui.activeRetriever->ui.tblEventRetriever->item(i,2)->text();
				tmpList<<ui.activeRetriever->ui.tblEventRetriever->item(i,3)->text();
				tmpList<<ui.activeRetriever->ui.tblEventRetriever->item(i,4)->text();
				selectedRows<<tmpList;
				selectedIndex<<i;
			}
		}
		selectedRowCount=selectedRows.count();
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
				ui.activeRetriever->ui.tblEventRetriever->removeRow(selectedIndex.at(i));
			}
			else
			{
				ui.activeRetriever->ui.tblEventRetriever->removeRow(selectedIndex.at(i)-1);
			}

		}
	}
	else
	{
		QString clipboardText;
		int row=ui.activeEditor->getActiveRow();
		int column=ui.activeEditor->getCurrentColumn();
		if (column==0)
		{
			return;
		}
		QString copystr=ui.activeEditor->ui.tblEventEditor->item(row,column)->text();
		QApplication::clipboard()->setText(copystr);
		ui.activeEditor->ui.tblEventEditor->item(row,column)->setText("");
		clipboardText=QApplication::clipboard()->text();
		ui.activeEditor->setClipBoardText(clipboardText);
	}	
}
void DLCRMain::actionCopy_clicked()
{
	if (ui.tabEvents->currentIndex()!=0)
	{
		QString str;
		QList<QStringList> selectedRows;
		QList<int> selectedIndex;
		for(int i=0;i<ui.activeRetriever->ui.tblEventRetriever->rowCount();i++)
		{
			if(ui.activeRetriever->ui.tblEventRetriever->item(i,1)->isSelected())
			{
				QStringList tmpList;
				tmpList<<ui.activeRetriever->ui.tblEventRetriever->item(i,0)->text();
				tmpList<<ui.activeRetriever->ui.tblEventRetriever->item(i,1)->text();
				tmpList<<ui.activeRetriever->ui.tblEventRetriever->item(i,2)->text();
				tmpList<<ui.activeRetriever->ui.tblEventRetriever->item(i,3)->text();
				tmpList<<ui.activeRetriever->ui.tblEventRetriever->item(i,4)->text();
				selectedRows<<tmpList;
				selectedIndex<<i;
			}
		}
		selectedRowCount=selectedRows.count();
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
	else
	{
		QString clipboardText;
		int row=ui.activeEditor->getActiveRow();
		int column=ui.activeEditor->getCurrentColumn();
		if (column==0)
		{
			return;
		}
		QString copystr=ui.activeEditor->ui.tblEventEditor->item(row,column)->text();
		QApplication::clipboard()->setText(copystr);
		clipboardText=QApplication::clipboard()->text();
		ui.activeEditor->setClipBoardText(clipboardText);
	}	
}
void DLCRMain::actionPaste_clicked()
{
	if (ui.tabEvents->currentIndex()!=0)
	{
		
		QList<QStringList> selectedRows;
		QList<int> selectedIndex;
		for(int i=0;i<ui.activeRetriever->ui.tblEventRetriever->rowCount();i++)
		{
			if(ui.activeRetriever->ui.tblEventRetriever->item(i,1)->isSelected())
			{
				QStringList tmpList;
				tmpList<<ui.activeRetriever->ui.tblEventRetriever->item(i,0)->text();
				tmpList<<ui.activeRetriever->ui.tblEventRetriever->item(i,1)->text();
				tmpList<<ui.activeRetriever->ui.tblEventRetriever->item(i,2)->text();
				tmpList<<ui.activeRetriever->ui.tblEventRetriever->item(i,3)->text();
				tmpList<<ui.activeRetriever->ui.tblEventRetriever->item(i,4)->text();
				selectedRows<<tmpList;
				selectedIndex<<i;
			}
		}
		if (selectedRows.count()!=selectedRowCount)
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
				ui.activeRetriever->ui.tblEventRetriever->item(row,j)->setText(columns[j]);	
			}
		}
	}
	else
	{
		int row=ui.activeEditor->getActiveRow();
		int column=ui.activeEditor->getCurrentColumn();
		if (column==0)
		{
			return;
		}
		ui.activeEditor->ui.tblEventEditor->item(row,column)->setText(QApplication::clipboard()->text());
	}
}

void DLCRMain::actionSimilar_Sessions_clicked()
{

}

void DLCRMain::actionRequest_Lognotes_clicked()
{

}

void DLCRMain::actionLanguage_Preference_clicked(LanguageMenu *menu)
{
	QString actionStr;
	actionStr=menu->text();
	translator=new QTranslator();

	LanguageUtil *lutil=new LanguageUtil(this);
	SystemFetch *fetch=new SystemFetch();
	PCUtil *pUtil=new PCUtil(this);

	LanguageInfo linfo;
	try
	{
		PCInfo pInfo;
		pInfo=pUtil->loadPCDetails(fetch->getSystemID(),logSession->getDriverName());

		linfo=lutil->loadLanguagePath(actionStr,pInfo.getPCID(),"Recorder",logSession->getDriverName());
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

	if(linfo.getLanguage()=="Arabic")
	{
		QString path=linfo.getQMPath().replace("/","\\");
		translator->load(path);
		qApp->installTranslator(translator);
		qApp->setLayoutDirection(Qt::RightToLeft);  		
		this->setLayoutDirection(Qt::RightToLeft);
		ui.retranslateUi(this);
	}
	else
	{
		QString path=linfo.getQMPath().replace("/","\\");
		translator->load(path);
		qApp->installTranslator(translator);
		qApp->setLayoutDirection(Qt::LeftToRight);  		
		this->setLayoutDirection(Qt::LeftToRight);
		ui.retranslateUi(this);
	}
	

	delete lutil;
	delete pUtil;
	delete fetch;
}

void DLCRMain::actionManage_Dictionary_clicked()
{
	Dictionary *dic=new Dictionary(this);
	dic->exec();
	delete dic;
}

void DLCRMain::actionManage_Language_clicked()
{
	Languages *lan=new Languages(this);
	lan->exec();
	delete lan;

	loadLanguage();
}

void DLCRMain::actionScheduler_clicked()
{
	Scheduler *schedule=new Scheduler(this);
	schedule->exec();
	delete schedule;
}

void DLCRMain::actionBold_Format_clicked(bool stats)
{
	if (ui.tabEvents->currentIndex()!=0)
	{
		ui.activeRetriever->actionBold_Status(stats);
	}
	else
	{
		ui.activeEditor->actionBold_Status(stats);		
	}	
}

void DLCRMain::actionItalics_Format_clicked(bool stats)
{
	if (ui.tabEvents->currentIndex()!=0)
	{	
		ui.activeRetriever->actionItalics_Status(stats);
	}
	else
	{
		ui.activeEditor->actionItalics_Status(stats);	
	}
}

void DLCRMain::actionUnderLine_clicked(bool stats)
{
	if (ui.tabEvents->currentIndex()!=0)
	{
		ui.activeRetriever->actionUnderLine_Status(stats);
	}
	else
	{
		ui.activeEditor->actionUnderLine_Status(stats);	
	}
}

void DLCRMain::actionText_Color_clicked()
{
	if (ui.tabEvents->currentIndex()!=0)
	{
		ui.activeRetriever->actionText_Highlight_Clicked();
	}
	else
	{
		ui.activeEditor->action_Text_Color_selection();	
	}
}

void DLCRMain::actionGoto_clicked()
{
	if (ui.tabEvents->currentIndex()!=0)
	{
		if (ui.activeRetriever->ui.tblEventRetriever->rowCount()==0)
		{
			QMessageBox::information(this, tr("INFORMATION"),
			tr("There is no row in your editor"));
			return;
		}
		else
		{
			Goto *go=new Goto(this);
			go->setTabIndex(1);
		}
	}
	else
	{
		if (ui.activeEditor->ui.tblEventEditor->rowCount()==0)
		{
			QMessageBox::information(this, tr("INFORMATION"),
			tr("There is no row in your editor"));
			return;
		}
		else
		{
			Goto *go=new Goto(this);
			go->setTabIndex(0);
		}
	}	
}

void DLCRMain::toGoToEditorCount(int num)
{
	 if (num>ui.activeEditor->ui.tblEventEditor->rowCount())
	 {
		 QMessageBox::information(this, tr("INFORMATION"),
		 tr("The given GOTO Count is exceeded your editor row count !!"));
		 return;
	 }
	 else
	 {
		ui.activeEditor->ui.tblEventEditor->setCurrentCell(num-1,QItemSelectionModel::Select);
	 }
}

void DLCRMain::toGoToRetrieverCount(int num)
{
	if (num>ui.activeRetriever->ui.tblEventRetriever->rowCount())
	{
		QMessageBox::information(this, tr("INFORMATION"),
		tr("The given GOTO Count is exceeded your editor row count !!"));
		return;
	}
	else
	{
		ui.activeRetriever->ui.tblEventRetriever->setCurrentCell(num-1,QItemSelectionModel::Select);
	}
}

void DLCRMain::actionReplace_clicked()
{
	if(ui.tabEvents->currentIndex()!=0)
	{	
		Replace *rep=new Replace(this);
		rep->loadSelectedText(findStr);
		rep->exec();
		QString replaceTxt=rep->replaceTxt();
		QString searchtext=rep->searchTxt();
		ui.activeRetriever->searchTxtReplace(replaceTxt,searchtext);
		delete rep;	
	}
	else
	{
		Replace *rep=new Replace(this);
		rep->loadSelectedText(findStr);
		rep->exec();
		QString replaceTxt=rep->replaceTxt();
		QString searchtext=rep->searchTxt();
		ui.activeEditor->searchTxtReplace(replaceTxt,searchtext);
		delete rep;	
	}	
}

void DLCRMain::startSchedulerTimer()
{
	ScheduleTimer->start(1000);
}
void DLCRMain::actionSearch_clicked()
{
	bool commitStatus=true;

	if(tempDB->connectDatabase()==false) 
	{
		SHOW_MESSAGE(tr("DLCRS ERROR"),tr("Database connection failed!!!"),tr("Ok"));
		PRINT_ERROR("Failed to create SQLLite Database connection!!!");
		return;
	}

	if(tempDB->checkDataExistence()!=false)
	{
		commitStatus=tempDB->commitAllData();
	}	

	if(commitStatus!=false)
	{
		AdvancedSearch *search=new AdvancedSearch(this);
		search->exec();
		delete search;
	}
}
DLCRMain::~DLCRMain()
{
	delete explore;
	delete sharedObj;
	delete shortcuts;
	delete history;
}
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/