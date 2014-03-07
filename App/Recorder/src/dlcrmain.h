/*Owner & Copyrights: Vance King Saxbe. A.*/#ifndef DLCRMAIN_H
#define DLCRMAIN_H

#include <QMainWindow>
#include "ui_dlcrmain.h"
#include <QPushButton>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QDir>
#include <QTimer>
#include <QTranslator>
#include <QWidget>
#include <QDialog>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QSpacerItem>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QDesktopServices>
#include "loginhistory.h"
#include "dlcrshared.h"
#include "loginusersession.h"
#include "explorecase.h"
#include "exploresession.h"
#include "sqlitedatabase.h"
#include "easynotes.h"
#include "languagemenu.h"
//#include "licenseengine.h"

class DLCRMain : public QMainWindow
{
	Q_OBJECT

public:

	DLCRMain(QWidget *parent = 0);
	~DLCRMain();
	
	Ui::DLCRMainClass ui;

	//ExploreCase *explore;
	ExploreSession *explore;
	SQLiteDataBase *tempDB;
	EasyNotes *shortcuts;
	//LicenseEngine *engine;
		
	QPushButton		*trayButton;
	QPushButton		*cancelButton;
	QPushButton		*closeButton;
	QSystemTrayIcon *trayIcon;
	QMenu			*trayIconMenu;
	QAction			*minimizeAction;
	QAction			*maximizeAction;
	QAction			*restoreAction;
	QAction			*quitAction;
	QMessageBox		exitMessageBox;
	QIcon icoPlay,icoPause;

	void loadLanguage();
	void createTray();
	void createExitMsg();
	bool canLoadSettings();
	//bool isValidLicense(); 
	void startEvent();
	void retrieveEvent(int,int);
	void setAudioChannelCount(int);
	int getAudioChannelCount();
	int getVideoCount();
	bool eventFilter(QObject *, QEvent *);
	void startSchedulerTimer();
	void stopEvent();
	void toGoToEditorCount(int);
	void toGoToRetrieverCount(int);
	
protected:

	void closeEvent(QCloseEvent *event);

private:	

	LoginUserSession *logSession;
	DLCRShared *sharedObj;
	LoginHistory *history;
	LanguageMenu *actionLanguage[100];

	QTranslator *translator; 

	bool recordingStatus;
	QTimer *myTimer;
	QTimer *ScheduleTimer,*ScheduleProcess,*ScheduleStop,*ScheduleDuration;
	QWidget *horizontalLayoutWidget;
	QHBoxLayout *horizontalLayout;
	QCheckBox *chkBoxStatus;
	QSpacerItem *horizontalSpacer;
	QPushButton *pbtnOk;
	QWidget *horizontalLayoutWidget_2;
	QHBoxLayout *horizontalLayout_2;
	QLabel *lblMessage;
	QDialog *errorMessage;
	int splittingDuration;
	int audioCount;
	int vCount;
	QString findStr;
	void sendMailToAdmin(int);
	void createMailMessage();
	void retranslateMailMessage(QDialog *);

	QString SEventNo,sEventName,sEventType,sEventDescription;
	int sEventID,schedulersID,selectedRowCount;

public slots:

	void VolumeValue(int,int);
	void checked_Bold(bool);
	void checked_Italics(bool);
	void checked_UnderLine(bool);
	void checked_KeyPress();
	void checked_cellPress();
	void checked_RowPress();
	void checked_RetreiveRowPress();
	void checked_RowDoublePress();
	void checked_cellDoublePress();


private slots:

	void updateAudioLevels(int,int);
	void restartRecording();
	void currentRecorderTime();
	void scheduler_Time();
	void scheduler_Processs();
	void scheduler_Stop();
	void scheduler_Duration();

	void actionNew_OpenCase_clicked();
	void actionStop_Case_clicked();
	void actionSaveToServer_clicked();
	void actionExit_Recorder_clicked();
	void actionLog_Editor_clicked();
	void actionRecorder_clicked();
	void actionSpell_Check_Tool_clicked();
	void actionEasy_Notes_clicked();
	void actionRetrieve_clicked();
	void actionAttachFile_clicked();
	void actionDownloadFile_clicked();	
	void actionOnlineSupport_clicked();
	void actionOnlineUpdate_clicked();
	void actionAboutDLCR_clicked();
	void actionHelp_clicked();
	void actionFTPUploader_clicked();
	void actionLoginHistory_clicked();
	void actionAdvanced_Search_clicked();
	void actionDiskAnalizer_click();
	void buttonPlayPause_clicked(bool);
	void actionBackUputility_clicked();
	void actionRecorderSettings_clicked();
	void tabEvents_closeRequested(int);
	void tabEvents_currentChanged(int);
	void messageClose();
	void actionNew_Notes_clicked();
	void actionAfter_Rowclicked();
	void actionBefore_Rowclicked();
	void actionDelete_Row_clicked();
	void actionImport_clicked();
	//void actionFormat_Style_clicked();
	void actionCut_clicked();
	void actionCopy_clicked();
	void actionPaste_clicked();
	void actionGoto_clicked();
	void actionFindNotes_clicked();
	void actionReplace_clicked();
	void actionInsert_clicked();
	void actionFiles_clicked();
	void actionImages_clicked();
	void actionBooks_clicked();
	void actionLinks_clicked();
	void actionSimilar_Sessions_clicked();
	void actionRequest_Lognotes_clicked();
	void actionLanguage_Preference_clicked(LanguageMenu*);
	void actionManage_Dictionary_clicked();
	void actionUnderLine_clicked(bool);
	void actionBold_Format_clicked(bool);
	void actionItalics_Format_clicked(bool);
	void actionManage_Language_clicked();
	void actionScheduler_clicked();
	void actionText_Highlight_Triggred();
	void actionText_Color_clicked();
	void actionSearch_clicked();
};

#endif // DLCRMAIN_H
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/