// BgRandPopupAbout.cpp : implementation file
//

#include "stdafx.h"
#include "BackgroundRandomizer.h"
#include "BgRandPopupAbout.h"
#include "BackgroundRandomizerDlg.h"
#include "Registry.h"
#include "GraphicsConverter.h"
#include "..\Registration\RegCode.h"
#include "DesktopInterface.h"
#include "DlgAdjustImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBgRandPopupAbout* g_pThisDlg = NULL;
int g_iNoDropOut = 1;
const UINT WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));

// Generate internal string for registration mix - 9/25/2001
CString g_strRegCodeLevel2 = "";
unsigned long g_L0 = 0x423ABE84L;
unsigned long g_L1 = 0xFAE9A2B3L;
unsigned long g_L2 = 0xEACDFFEEL;
unsigned long g_L3 = 0x11994422L;
unsigned long g_L4 = 0xEAF9245FL;	
CRegCodeDuplicate g_RegCodeLevel2(g_L0, g_L1, g_L2, g_L3, g_L4);	

#define TIMER_HIDER		235
#define	WM_ICON_NOTIFY	WM_USER+10

// This function hides the window on startup (when the timer pops)
void CALLBACK EXPORT TimerWindowHider(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime)
{
	if (nIDEvent == TIMER_HIDER)
	{
		g_pThisDlg->ShowWindow(SW_HIDE);
		g_pThisDlg->KillTimer(nIDEvent);
	}
}

inline void ProcessMessages()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	    
	{
		 TranslateMessage(&msg);	    
		 DispatchMessage(&msg);	   
	}	 
}

// Update background thread
UINT ThreadUpdateBackground(LPVOID pParam)
{
	return g_pThisDlg->BackgroundThread(pParam);
}


/////////////////////////////////////////////////////////////////////////////
// CBgRandPopupAbout dialog


CBgRandPopupAbout::CBgRandPopupAbout(CWnd* pParent /*=NULL*/)
	: CDialog(CBgRandPopupAbout::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBgRandPopupAbout)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bThreadStarted = FALSE;
	m_bThreadRunning = FALSE;
	m_bRunning = TRUE;
	m_iRefreshMode = REFRESH_NEW_PIC;
	
	m_strCurrentPic = "";

	m_pDlgRegister = new CDlgRegister(NULL, TRUE);
}

CBgRandPopupAbout::~CBgRandPopupAbout()
{
	if (m_pDlgRegister)
		delete m_pDlgRegister;
}

void CBgRandPopupAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBgRandPopupAbout)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBgRandPopupAbout, CDialog)
	//{{AFX_MSG_MAP(CBgRandPopupAbout)
	ON_COMMAND(ID_MENU_CHANGEIMAGENOW, OnMenuChangeImageNow)
	ON_COMMAND(ID_MENU_TERMINATE, OnMenuTerminate)
	ON_COMMAND(ID_MENU_SETTINGS, OnMenuSettings)
	ON_COMMAND(ID_MENU_ABOUT, OnMenuAbout)
	ON_WM_CLOSE()
	ON_COMMAND(ID_MENU_REGISTRATION, OnMenuRegistration)
	ON_COMMAND(ID_MENU_DONTSHOWTHISIMAGEAGAIN, OnMenuDontShowImageAgain)
	ON_COMMAND(ID_MENU_SLIDESHOW, OnMenuSlideShow)
	ON_REGISTERED_MESSAGE(WM_TASKBARCREATED, OnTaskbarCreated)
	ON_COMMAND(ID_MENU_ADJUSTCURRENTIMAGE, OnMenuAdjustCurrentImage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBgRandPopupAbout message handlers

BOOL CBgRandPopupAbout::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Set the global dlg
	g_pThisDlg = this;
	
	// Set the menu's parent
	m_TrayIcon.SetMyParentWindow(this);

	CreateSystemTrayIcon();

	ShowWindow(SW_MINIMIZE);

	// We set a timer to hide the window, since I can't figure
	// out another way to hide it on startup.
	SetTimer(TIMER_HIDER, 0, TimerWindowHider);

	// By default, hide the window if we are licensed
	BOOL bAllowThread = FALSE;
	if (m_pDlgRegister->CheckRegistrationStatus() == CDlgRegister::LICENSED)
	{
		bAllowThread = TRUE;
	}
	else // Check ou the trial
	{
		m_pDlgRegister->DoModal();

		if ((m_pDlgRegister->CheckRegistrationStatus() == CDlgRegister::EXPIRED) ||
			(m_pDlgRegister->CheckRegistrationStatus() == CDlgRegister::OLDSTYLE))
		{	
			::MessageBox(NULL, "Trial Period Over.\n\nBackground Randomizer will now exit.",
						 "Background Randomizer", MB_OK);

			::PostQuitMessage(0);
			// This seems to not be necessary 
			// (and makes an extra bonk on my system)
			// EndDialog(0);

			// We must manually remove the icon for some reason
			m_TrayIcon.RemoveIcon();
			m_TrayIcon.DestroyWindow();
		}
		else // We're in a trial
		{
			bAllowThread = TRUE;
		}
	}

	// Start worker thread if not started
	// Starts later if window is opening as visible (new users)
	if (bAllowThread && !m_bThreadStarted)
	{
		m_bThreadStarted = TRUE;
		m_bThreadRunning = TRUE;
		AfxBeginThread(ThreadUpdateBackground, NULL);
	}

	// We set a timer to hide the window, since I can't figure
	// out another way to hide it on startup.
	SetTimer(TIMER_HIDER, 0, TimerWindowHider);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBgRandPopupAbout::CreateSystemTrayIcon()
{
	if (!m_TrayIcon.Create(NULL,			    // Parent window
						   WM_ICON_NOTIFY,		  // Icon notify message to use
						   _T("Background Randomizer"),  // tooltip
						   AfxGetApp()->LoadIcon(IDR_MAINFRAME), // ID of tray icon
						   IDR_MENU_TRAY_POPUP)) // ID of menu
		return FALSE;

    m_TrayIcon.SetMenuDefaultItem(0, TRUE);
	return TRUE;
}


void CBgRandPopupAbout::SetCurrentPic(CString& strCurrPic)
{
	m_critCurrPic.Lock();
	m_strCurrentPic = strCurrPic;
	m_critCurrPic.Unlock();
}

CString CBgRandPopupAbout::GetCurrentPic()
{
	CString strReturn;

	m_critCurrPic.Lock();
	strReturn = m_strCurrentPic;
	m_critCurrPic.Unlock();

	return strReturn;
}


void CBgRandPopupAbout::OnMenuChangeImageNow() 
{
	if ((m_iRefreshMode == NO_REFRESH) && !m_TrayIcon.GetSlideShow())
	{
		m_TrayIcon.SetIcon(IDR_ICON_CHANGE_REQUEST);
		m_iRefreshMode = REFRESH_NEW_PIC;
	}
}

void CBgRandPopupAbout::OnMenuTerminate() 
{
	static BOOL bInTerminate = FALSE;

	if (!bInTerminate)
	{
		bInTerminate = TRUE;

		// Skip exit if we've already initiated the exit
		if (!m_bRunning)
			return;

		if (::MessageBox(NULL, "Are you sure you want to exit Background Randomizer", "Background Randomizer", MB_YESNO) == IDYES)
		{
			m_bRunning = FALSE;
			// Hide the tray icon
			m_TrayIcon.ShutdownPrepareForSystemExit();
			// Release our mutex
			((CBackgroundRandomizerApp*)AfxGetApp())->FreeMutex();

			while (m_bThreadRunning)
			{
				ProcessMessages();
				Sleep(25);
			}

			OnOK();
			//SendMessage(WM_CLOSE);
		}
		bInTerminate = FALSE;
	}
}

void CBgRandPopupAbout::OnMenuSettings() 
{	
	// Creating a mutex only allows one instance of startup	
	HANDLE hMutexSettings = CreateMutex(NULL, FALSE, "unHUmanBackgroundRandomizerSettings");
	// if the mutex doesn't exist, we can go ahead and create the app
	if ( GetLastError() != ERROR_ALREADY_EXISTS ) 
	{			
		m_dlgSettings.DoModal();
		ReleaseMutex(hMutexSettings);
		CloseHandle(hMutexSettings);
	}
	else // bring the app to the front
	{
		// Find the other windows and display it.
		CWnd *pWndPrev = NULL;
		CWnd *pWndChild = NULL;
		if (pWndPrev = CWnd::FindWindow(NULL, "Background Randomizer Settings"))
		{
			// if so, does it have any popups?
			pWndChild = pWndPrev->GetLastActivePopup();
			
			// If iconic, restore the main window
			if (pWndPrev->IsIconic())
				pWndPrev->ShowWindow(SW_RESTORE);
			
			// Bring the main window or its popup to
			// the foreground		
			pWndChild->SetForegroundWindow();
		}
	}
}

void CBgRandPopupAbout::OnMenuAbout() 
{
	((CBackgroundRandomizerApp*)AfxGetApp())->LaunchAboutWindow();
}

void CBgRandPopupAbout::OnMenuRegistration() 
{
	((CBackgroundRandomizerApp*)AfxGetApp())->LaunchRegisterWindow(m_pDlgRegister);
}

DWORD CBgRandPopupAbout::GetChangeInterval()
{
	DWORD dwRetChangeInterval = 0;	
	COXRegistry regSettings;	
	if (regSettings.Connect(COXRegistry::keyLocalMachine))
	{
		if (regSettings.CheckAndOpenKeyForWrite("SOFTWARE\\unHUman Software\\BackgroundRandomizer"))
		{
			DWORD dwRegValue = 0;
			if (regSettings.GetValue(CHANGE_INTERVAL, dwRegValue) &&
				(dwRegValue > 0))
			{
				dwRetChangeInterval = 0;
				switch (dwRegValue)
				{
					case 0: // Never - Startup Only
						dwRetChangeInterval = 0;
						break;
					case 1: // 15 Mins
						dwRetChangeInterval = 1;
						break;
					case 2: // 30 Mins
						dwRetChangeInterval = 2;
						break;
					case 3: // 1 Hour
						dwRetChangeInterval = 4;
						break;
					case 4: // 2 Hours
						dwRetChangeInterval = 8;
						break;
					case 5: // 4 Hours
						dwRetChangeInterval = 16;
						break;
					case 6: // 8 Hours
						dwRetChangeInterval = 32;
						break;
					case 7: // 24 Hours
						dwRetChangeInterval = 96;
						break;
				}
			}
		}
		regSettings.Disconnect();
	}

	return dwRetChangeInterval;
}

void CBgRandPopupAbout::SetOutputBitmap(CStringList* pStrListFiles /* = NULL */,
										int iRandChoice /* = -1 */)
{
	int iSuccess = PROCESS_PICK_FAILED;
	BOOL bActiveDesktop = CDesktopInterface::IsActiveDesktop();

	int iCount = -1;
	if (pStrListFiles)
	{
		// Ensure we have items to play with
		iCount = pStrListFiles->GetCount();
		if (!iCount)
			return;
	}

	// Set the output bitmap
	CString strScreenFile = AfxGetApp()->m_pszHelpFilePath;			
	int iLastSlash = strScreenFile.ReverseFind('\\');
	strScreenFile = strScreenFile.Left(iLastSlash+1);
	CString strScreenFileAlt = strScreenFile;
	strScreenFile += "BackgroundRandomizerImage";
	strScreenFile += (bActiveDesktop) ? ".jpg" : ".bmp";
	strScreenFileAlt += "BackgroundRandomizerImageAlt";
	strScreenFileAlt += (bActiveDesktop) ? ".jpg" : ".bmp";
	
	CGraphicsConverter grcvtConverter;
	int iRandChoiceOrig = iRandChoice;	
	do
	{	
		if (pStrListFiles)
			SetCurrentPic(pStrListFiles->GetAt(pStrListFiles->FindIndex(iRandChoice)));
		CString strCurrPic = GetCurrentPic();
		iSuccess = grcvtConverter.ProcessPic(strCurrPic, strScreenFile,
											 GetCurrentPicData(strCurrPic),
											 m_dlgSettings.m_bMatchScreenOrientation,
											 m_dlgSettings.m_bResizeToDesktopDimensions, 
											 m_dlgSettings.m_bMaintainAspectRatio,
											 m_dlgSettings.m_iAutoRotateToMatchScreen,
											 m_dlgSettings.m_iOversizeMode);
		// break out if we found a valid object
		if (iSuccess != PROCESS_PICK_INVALID_ASPECT)
			break;

		// Go to the next item
		if (iCount != -1)
			iRandChoice = (++iRandChoice) % iCount;
	} while ((iCount != -1) && (iRandChoice != iRandChoiceOrig));
	
	// If the save failed - try to save as an alt filename
	if (iSuccess == PROCESS_PICK_FAILED_SAVE)
	{
		CString strCurrPic = GetCurrentPic();
		iSuccess = grcvtConverter.ProcessPic(strCurrPic, strScreenFileAlt,
											 GetCurrentPicData(strCurrPic),
											 m_dlgSettings.m_bMatchScreenOrientation,
											 m_dlgSettings.m_bResizeToDesktopDimensions, 
											 m_dlgSettings.m_bMaintainAspectRatio,
											 m_dlgSettings.m_iAutoRotateToMatchScreen,
											 m_dlgSettings.m_iOversizeMode);
	}

	if (iSuccess == PROCESS_PICK_SUCCESS)			
		CDesktopInterface::SetWallpaper(strScreenFile, bActiveDesktop);
	else
		CDesktopInterface::SetWallpaper("", bActiveDesktop);
}

void CBgRandPopupAbout::HandleBackgroundPick(CStringList* pStrListFiles)
{
	// Ensure we have items to play with
	int iCount = pStrListFiles->GetCount();
	if (!iCount)
		return;

	// Seed random number generator
	srand((unsigned)time(NULL));

	int iRandChoice = rand() % iCount;
	
	BOOL bActiveDesktop = CDesktopInterface::IsActiveDesktop();

	// If we are not duplicated, ensure that
	// we have more than 1 picture and handle
	// if we picked the same pic
	if (m_dlgSettings.m_bDontDuplicateImages && (iCount > 1))
	{
		CString strCurrentWallpaper;
		CDesktopInterface::GetWallpaper(strCurrentWallpaper, bActiveDesktop);
		if (strCurrentWallpaper == pStrListFiles->GetAt(pStrListFiles->FindIndex(iRandChoice)))
			iRandChoice = (++iRandChoice) % iCount;
	}	

	SetOutputBitmap(pStrListFiles, iRandChoice);
}

UINT CBgRandPopupAbout::BackgroundThread(LPVOID pParam)
{
	if (!g_RegCodeLevel2.GenerateRegCode(g_pThisDlg->m_pDlgRegister->m_strUserName, g_strRegCodeLevel2))
		g_strRegCodeLevel2 = "";
	CString strMixed = "";
	int iFail = RegMixStrings(g_pThisDlg->m_pDlgRegister->m_strLevelOneReg, g_strRegCodeLevel2, strMixed);

	// If the mixed code is not valid or 
	// (it's empty and the trial is invalid), force a drop out
	if ((strMixed != g_pThisDlg->m_pDlgRegister->m_strLicenseCode) ||
		(!strMixed.GetLength() && !g_pThisDlg->m_pDlgRegister->CheckInValidTrial()))
		g_iNoDropOut--;

	// Need to set up something
	// so that the SystemTrayIcon window has a parent
	// so it works properly and we need a message pump too.	
	DWORD dwFifteenSeconds = 15 * 1000;
	DWORD dwThreeSeconds = 3 * 1000;
	#ifndef _DEBUG
		DWORD dwSleepInterval = dwFifteenSeconds;
	#else
		DWORD dwSleepInterval = dwThreeSeconds;
		DWORD dwIntervalMultiplier = dwFifteenSeconds/dwThreeSeconds;
	#endif
	int iReplayIntervalsNeeded = 0;
	int iReplayIntervalsHad = 0;
	int i;
	BOOL bRunning = TRUE;
	BOOL bRegChecked = FALSE;
	do
	{
		// If this is a registered copy, remove the register from the menu
		if (!bRegChecked && (
			g_pThisDlg->m_pDlgRegister->CheckRegistrationStatus() == CDlgRegister::LICENSED))
		{
			// Don't do this again
			bRegChecked = TRUE;
			m_TrayIcon.SetShowRegistration(FALSE);
		}

		iReplayIntervalsHad = 0;

		// Change the background pic
		m_TrayIcon.SetIcon(IDR_ICON_WORKING);
		if (m_iRefreshMode == REFRESH_NEW_PIC)
		{
			CStringList* pStrListFiles = m_dlgSettings.GetFilesAvailable();
			if (pStrListFiles)
			{
				
				HandleBackgroundPick(pStrListFiles);				
			
				if (pStrListFiles)
					delete pStrListFiles;
			}
		}
		else // work with the current pic
		{
			SetOutputBitmap();
		}
		m_TrayIcon.SetIcon(IDR_MAINFRAME);

		m_iRefreshMode = NO_REFRESH;
		do
		{
			for (i = 0; m_bRunning && !m_iRefreshMode && (i < 60); i++)
			{
				Sleep(dwSleepInterval);

				// Bail out if in SlideShowMode()
				if (m_TrayIcon.GetSlideShow())
				{
					// Force image update - large number
					iReplayIntervalsHad = 10000;
					m_iRefreshMode = REFRESH_NEW_PIC;
					break;
				}
			}
			iReplayIntervalsHad++;
			iReplayIntervalsNeeded = GetChangeInterval();
			#ifdef _DEBUG
				// If our interval is shorter for debug mode,
				// we need to have more intervals to counter
				// the shorter duration between checks
				iReplayIntervalsNeeded *= dwIntervalMultiplier;
			#endif

			// Bail out if something is wrong with the license
			bRunning *= g_iNoDropOut;

		} while (m_bRunning && !m_iRefreshMode && 
				 bRunning && ((CBackgroundRandomizerApp*)AfxGetApp())->m_iHiddenRegistrationValid &&
				 ((iReplayIntervalsNeeded == 0) || 
				  (iReplayIntervalsHad < iReplayIntervalsNeeded)));
	} while (m_bRunning && bRunning);
	m_bThreadRunning = FALSE;	

	AfxEndThread(0);
	return 0;
}

void CBgRandPopupAbout::OnClose() 
{	
	CDialog::OnClose();	

	exit(0);
}

LRESULT CBgRandPopupAbout::OnTaskbarCreated(WPARAM wp, LPARAM lp)
{
	m_TrayIcon.ShowIcon(TRUE);
	return 0;
}

void CBgRandPopupAbout::OnMenuDontShowImageAgain() 
{
	CString strBlockPic = GetCurrentPic();	
	if (strBlockPic.GetLength() && MessageBox("Are you sure you don't want to view this image again?\n\n" + strBlockPic, "Background Randomizer", MB_YESNO) == IDYES)
	{
		m_dlgSettings.m_dlgBlockedImages.AddBlockedImage(strBlockPic);
	}
}

void CBgRandPopupAbout::UpdateCurrentPicData(CString& strCurrPic, DWORD dwManipulateFlags)
{
	DWORD dwCurrRotation = GetCurrentPicData(strCurrPic) & BGRAND_ROTATE_MAP;
	dwCurrRotation += dwManipulateFlags & BGRAND_ROTATE_MAP;
	dwCurrRotation %= 4;

	DWORD dwCurrFlags = dwCurrRotation;

	SetCurrentPicData(strCurrPic, dwCurrFlags);
}

DWORD CBgRandPopupAbout::GetCurrentPicData(CString& strCurrPic)
{
	return 0;
}

void CBgRandPopupAbout::SetCurrentPicData(CString& strCurrPic, DWORD dwManipulateFlags)
{
}

void CBgRandPopupAbout::HandleUpdateCurrentPicFlags(DWORD dwManipulateFlags)
{
	if (m_iRefreshMode == NO_REFRESH)
	{		
		CString strCurrPic = GetCurrentPic();
		if (strCurrPic.GetLength())
		{
			UpdateCurrentPicData(strCurrPic, dwManipulateFlags);
			m_TrayIcon.SetIcon(IDR_ICON_CHANGE_REQUEST);
			m_iRefreshMode = REFRESH_ROTATE_CURR_PIC;
		}
	}
}

void CBgRandPopupAbout::OnMenuSlideShow() 
{
	m_TrayIcon.FlipSlideShow();
}

void CBgRandPopupAbout::OnMenuAdjustCurrentImage() 
{
	CDlgAdjustImage dlgAdjustImage(GetCurrentPic());
	dlgAdjustImage.DoModal();
}
