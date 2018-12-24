// BackgroundRandomizer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "BackgroundRandomizer.h"
#include "BgRandPopupAbout.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBackgroundRandomizerApp

BEGIN_MESSAGE_MAP(CBackgroundRandomizerApp, CWinApp)
	//{{AFX_MSG_MAP(CBackgroundRandomizerApp)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBackgroundRandomizerApp construction

CBackgroundRandomizerApp::CBackgroundRandomizerApp()
{
	m_iHiddenRegistrationValid = 0;
	m_iHiddenRegistrationValid2 = 0;
	m_hMutex = NULL;	
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBackgroundRandomizerApp object

CBackgroundRandomizerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CBackgroundRandomizerApp initialization

BOOL CBackgroundRandomizerApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	m_sClassName = AfxRegisterWndClass(NULL,NULL);
	m_wndParent.CreateEx(NULL, m_sClassName, "", NULL, 0, 0, 10, 10, NULL, NULL);

	// Creating a mutex only allows one instance of startup
	m_hMutex = CreateMutex(NULL, FALSE, "unHUmanBackgroundRandomizerStartup");
	// If the mutex doesn't exist, we can go ahead and startup the background app
	if ( GetLastError() != ERROR_ALREADY_EXISTS ) 
	{
		CBgRandPopupAbout dlgRandPopupAbout;
		m_pMainWnd = &dlgRandPopupAbout;
		dlgRandPopupAbout.DoModal();
	}
	else // We're going to exit because the app already exists
	{
		// Reset our mutex handle
		m_hMutex = NULL;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CBackgroundRandomizerApp::FreeMutex()
{
	// Release the mutex
	if (m_hMutex != NULL)
	{
		ReleaseMutex(m_hMutex);
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}
}

int CBackgroundRandomizerApp::ExitInstance() 
{
	FreeMutex();
	return CWinApp::ExitInstance();
}

void CBackgroundRandomizerApp::LaunchAboutWindow()
{
	// Creating a mutex only allows one instance of startup	
	HANDLE hMutexAbout = CreateMutex(NULL, FALSE, "unHUmanBackgroundRandomizerAbout");
	// if the mutex doesn't exist, we can go ahead and create the app
	if ( GetLastError() != ERROR_ALREADY_EXISTS ) 
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
		ReleaseMutex(hMutexAbout);
		CloseHandle(hMutexAbout);
	}
	else // bring the app to the front
	{
		// Find the other windows and display it.
		CWnd *pWndPrev = NULL;
		CWnd *pWndChild = NULL;
		if (pWndPrev = CWnd::FindWindow(NULL, "About Background Randomizer"))
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

void CBackgroundRandomizerApp::LaunchRegisterWindow(CDlgRegister* pDlgRegister)
{
	if (!pDlgRegister)
		return;

	// Creating a mutex only allows one instance of startup	
	HANDLE hMutexRegister = CreateMutex(NULL, FALSE, "unHUmanBackgroundRandomizerRegister");
	// if the mutex doesn't exist, we can go ahead and create the app
	if ( GetLastError() != ERROR_ALREADY_EXISTS ) 
	{
		pDlgRegister->DoModal();
		ReleaseMutex(hMutexRegister);
		CloseHandle(hMutexRegister);
	}
	else // bring the app to the front
	{
		// Find the other windows and display it.
		CWnd *pWndPrev = NULL;
		CWnd *pWndChild = NULL;
		if (pWndPrev = CWnd::FindWindow(NULL, "Background Randomizer Registration"))
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
