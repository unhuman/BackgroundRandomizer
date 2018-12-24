// BackgroundRandomizerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BackgroundRandomizer.h"
#include "BackgroundRandomizerDlg.h"
#include "DesktopInterface.h"
//#include "SHLObj.h"
#include "Registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLOR_STATIC_LINK_COLOR		RGB(0, 0, 238) // Blue

CCriticalSection g_critReg;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strScreenMode = (CDesktopInterface::IsActiveDesktop()) ? "Active Desktop" : "Classic Desktop";
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_STATIC_EMAIL, m_staticEmail);
	DDX_Control(pDX, IDC_STATIC_REGISTER, m_staticRegister);
	DDX_Control(pDX, IDC_STATIC_WEB_SITE, m_staticWebSite);
	DDX_Text(pDX, IDC_STATIC_SCREEN_MODE, m_strScreenMode);
	//}}AFX_DATA_MAP
}

void CAboutDlg::OnStaticWebSite() 
{
	ShellExecute(m_hWnd,NULL,"http://www.unhuman.com",NULL,NULL,SW_SHOWNORMAL);
}

void CAboutDlg::OnStaticEmail() 
{
	ShellExecute(m_hWnd,NULL,"mailto:human@unhuman.com?subject=Background Randomizer",NULL,NULL,SW_SHOWNORMAL);	
}

void CAboutDlg::OnStaticRegister() 
{
	ShellExecute(m_hWnd,NULL,"https://www.regnow.com/softsell/nph-softsell.cgi?item=2259-5",NULL,NULL,SW_SHOWNORMAL);	
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_staticEmail.SetTextColor(COLOR_STATIC_LINK_COLOR);
	m_staticRegister.SetTextColor(COLOR_STATIC_LINK_COLOR);
	m_staticWebSite.SetTextColor(COLOR_STATIC_LINK_COLOR);	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_STATIC_WEB_SITE, OnStaticWebSite)
	ON_BN_CLICKED(IDC_STATIC_EMAIL, OnStaticEmail)
	ON_BN_CLICKED(IDC_STATIC_REGISTER, OnStaticRegister)
	ON_BN_CLICKED(IDC_STATIC_CREDIT_CARDS, OnStaticRegister)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBackgroundRandomizerDlg dialog

CBackgroundRandomizerDlg::CBackgroundRandomizerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBackgroundRandomizerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBackgroundRandomizerDlg)
	m_bEnabled = FALSE;
	m_bDontDuplicateImages = TRUE;
	m_bMaintainAspectRatio = FALSE;
	m_bResizeToDesktopDimensions = FALSE;
	m_iOversizeMode = 0;
	m_bMatchScreenOrientation = TRUE;
	m_iChangeInterval = 0;
	m_iAutoRotateToMatchScreen = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	SaveDataToRegistry(LOAD_INIT);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBackgroundRandomizerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBackgroundRandomizerDlg)
	DDX_Control(pDX, IDC_COMBO_AUTO_ROTATE_TO_MATCH_SCREEN, m_comboAutoRotateToMatchScreen);
	DDX_Control(pDX, IDC_COMBO_ASPECT_RATIO_MODE, m_comboAspectRatioMode);	
	DDX_Control(pDX, IDC_CHECK_MAINTAIN_ASPECT_RATIO, m_btnMaintainAspectRatio);
	DDX_Control(pDX, IDC_BUTTON_REMOVE, m_btnRemove);
	DDX_Control(pDX, IDC_LIST_DIRECTORIES, m_lstboxDirectories);
	DDX_Check(pDX, IDC_CHECK_ENABLE_RANDOMIZATION, m_bEnabled);
	DDX_Check(pDX, IDC_CHECK_DONT_DUPLICATE_IMAGES, m_bDontDuplicateImages);
	DDX_Check(pDX, IDC_CHECK_MAINTAIN_ASPECT_RATIO, m_bMaintainAspectRatio);
	DDX_Check(pDX, IDC_CHECK_RESIZE_TO_FILL_DESKTOP, m_bResizeToDesktopDimensions);
	DDX_CBIndex(pDX, IDC_COMBO_ASPECT_RATIO_MODE, m_iOversizeMode);
	DDX_Check(pDX, IDC_CHECK_MATCH_SCREEN_ORIENTATION, m_bMatchScreenOrientation);
	DDX_CBIndex(pDX, IDC_COMBO_BACKGROUND_INTERVAL, m_iChangeInterval);
	DDX_CBIndex(pDX, IDC_COMBO_AUTO_ROTATE_TO_MATCH_SCREEN, m_iAutoRotateToMatchScreen);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBackgroundRandomizerDlg, CDialog)
	//{{AFX_MSG_MAP(CBackgroundRandomizerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, OnDone)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	ON_LBN_SELCHANGE(IDC_LIST_DIRECTORIES, OnListDirectories)
	ON_BN_CLICKED(IDC_CHECK_RESIZE_TO_FILL_DESKTOP, OnCheckResizeToFillDesktop)
	ON_BN_CLICKED(IDC_CHECK_MAINTAIN_ASPECT_RATIO, OnCheckMaintainAspectRatio)
	ON_BN_CLICKED(IDC_BUTTON_MANAGE_BLOCKED_IMAGES, OnButtonManageBlockedImages)
	ON_LBN_SETFOCUS(IDC_LIST_DIRECTORIES, OnListDirectories)
	ON_BN_CLICKED(ID_CANCEL, OnCancel)
	ON_BN_CLICKED(IDC_CHECK_MATCH_SCREEN_ORIENTATION, OnCheckMatchScreenOrientation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBackgroundRandomizerDlg message handlers

BOOL CBackgroundRandomizerDlg::OnInitDialog()
{		
	CDialog::OnInitDialog();

	SaveDataToRegistry(LOAD_WORKING);
	UpdateData(FALSE);
	OnCheckResizeToFillDesktop();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	OnCheckMatchScreenOrientation();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBackgroundRandomizerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBackgroundRandomizerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBackgroundRandomizerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CBackgroundRandomizerDlg::OnDone() 
{
	UpdateData();
	SaveDataToRegistry();
	OnOK();
}

void CBackgroundRandomizerDlg::OnCancel() 
{
	// Load the old settings from the registry and cancel
	SaveDataToRegistry(LOAD_WORKING);
	OnOK();
}

CStringList* CBackgroundRandomizerDlg::GetFilesAvailable()
{
	return SaveDataToRegistry(GET_FILE_LIST);
}

CStringList* CBackgroundRandomizerDlg::FindFileList(CStringArray* pStrArrayDirectories, CStringArray* pStrArrayBlockFiles)
{
	// If there's no items, don't pick anything
	int iDirectories = pStrArrayDirectories->GetSize();
	if (!iDirectories)
		return NULL;

	// Convert the block file list array to a string list	
	int iBlockFiles = pStrArrayBlockFiles->GetSize();
	CStringList strlistBlockFiles((iBlockFiles) ? iBlockFiles : 1);
	CString strBlockFile;
	for (int iBlockX=0; iBlockX<iBlockFiles; iBlockX++)
	{
		strlistBlockFiles.AddTail(pStrArrayBlockFiles->GetAt(iBlockX));
	}

	// Create the return array
	CStringList* pStrListImageFiles = new CStringList;
	CString strDirectory;
	CFileFind finder;	
	BOOL bWorking;
	for (int i=0; i<iDirectories; i++)
	{
		strDirectory = pStrArrayDirectories->GetAt(i);
		
		CFileFind finder;
		// Find the BMP files
		bWorking = finder.FindFile(strDirectory + "\\*.bmp");
		CString strFileCheck;
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			if (!finder.IsDirectory())
			{
				strFileCheck = strDirectory + "\\" + finder.GetFileName();
				if (!strlistBlockFiles.Find(strFileCheck))
					pStrListImageFiles->AddHead(strFileCheck);
			}
		}
		// Find the jpg files
		bWorking = finder.FindFile(strDirectory + "\\*.jpg");
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			if (!finder.IsDirectory())
			{
				strFileCheck = strDirectory + "\\" + finder.GetFileName();
				if (!strlistBlockFiles.Find(strFileCheck))
					pStrListImageFiles->AddHead(strFileCheck);
			}
		}
	}

	// Only return stuff if there is stuff to return
	if (pStrListImageFiles->GetCount() > 0)
		return pStrListImageFiles;
	else
	{
		delete pStrListImageFiles;
		return NULL;
	}

}

CStringList* CBackgroundRandomizerDlg::SaveDataToRegistry(int iSave /* = SAVE_SETTINGS */)
{	
	g_critReg.Lock();

	COXRegistry regSettings;
	if (regSettings.Connect(COXRegistry::keyLocalMachine))
	{
		if (regSettings.CheckAndOpenKeyForWrite("SOFTWARE\\unHUman Software\\BackgroundRandomizer"))
		{
			CStringArray* pStrArrayDirectories = new CStringArray;
			if (iSave == SAVE_SETTINGS)
			{
				regSettings.SetValue(DONT_DUPLICATE_IMAGES, m_bDontDuplicateImages);
				regSettings.SetValue(MATCH_SCREEN_ORIENTATION, m_bMatchScreenOrientation);				
				regSettings.SetValue(RESIZE_TO_DESKTOP_DIMENSIONS, m_bResizeToDesktopDimensions);
				regSettings.SetValue(MAINTAIN_ASPECT_RATIO, m_bMaintainAspectRatio);
				regSettings.SetValue(AUTO_ROTATE_TO_MATCH_SCREEN, m_iAutoRotateToMatchScreen);
				regSettings.SetValue(OVERSIZE_MODE, m_iOversizeMode);
				regSettings.SetValue(CHANGE_INTERVAL, m_iChangeInterval);
				
				int iCount = m_lstboxDirectories.GetCount();
				CString strDirectory(_T(""));				
				pStrArrayDirectories->SetSize(iCount);			
				for (int i=0; i<iCount; i++)
				{
					m_lstboxDirectories.GetText(i, strDirectory);
					pStrArrayDirectories->SetAt(i, strDirectory);				
				}

				regSettings.SetValue(DIRECTORIES, *pStrArrayDirectories);
				delete pStrArrayDirectories;
				pStrArrayDirectories = NULL;
			}
			else // Some type of load
			{
				DWORD dwRegValue;
				// Get the directories to look for files
				regSettings.GetValue(DIRECTORIES, *pStrArrayDirectories);
				if (iSave == GET_FILE_LIST)
				{
					CStringArray* pStrArrayBlockFiles = new CStringArray;
					regSettings.GetValue(BLOCKED_FILES, *pStrArrayBlockFiles);
					// This is startup, so just return the directories
					g_critReg.Unlock();

					// Find the files that are valid in the directories						
					CStringList* pStrArrayFileList;
					pStrArrayFileList = FindFileList(pStrArrayDirectories, pStrArrayBlockFiles);

					delete pStrArrayDirectories;
					delete pStrArrayBlockFiles;
					return pStrArrayFileList;
				}
				else if (iSave == LOAD_WORKING)
				{
					// We're working, so load the directories into the list box
					int iCount = pStrArrayDirectories->GetSize();				
					for (int i=0; i<iCount; i++)
						m_lstboxDirectories.AddString(pStrArrayDirectories->GetAt(i));
					delete pStrArrayDirectories;
					pStrArrayDirectories = NULL;
				}

				delete pStrArrayDirectories;

				// Get if we can duplicate images
				if (regSettings.GetValue(DONT_DUPLICATE_IMAGES, dwRegValue))
					m_bDontDuplicateImages = dwRegValue;
				// Get Match Screen Orientation
				if (regSettings.GetValue(MATCH_SCREEN_ORIENTATION, dwRegValue))
					m_bMatchScreenOrientation = dwRegValue;
				// Get Auto Rotate to Match Screen Orientation
				if (regSettings.GetValue(AUTO_ROTATE_TO_MATCH_SCREEN, dwRegValue))
					m_iAutoRotateToMatchScreen = dwRegValue;
				// Get Resize To Fill Desktop
				if (regSettings.GetValue(RESIZE_TO_DESKTOP_DIMENSIONS, dwRegValue))
					m_bResizeToDesktopDimensions = dwRegValue;
				// Get Maintain Aspect Ratio
				if (regSettings.GetValue(MAINTAIN_ASPECT_RATIO, dwRegValue))
					m_bMaintainAspectRatio = dwRegValue;
				// Get Allow Oversize To Fill Screen
				if (regSettings.GetValue(OVERSIZE_MODE, dwRegValue))
					m_iOversizeMode = dwRegValue;
				// Get Update Interval
				if (regSettings.GetValue(CHANGE_INTERVAL, dwRegValue))
					m_iChangeInterval = dwRegValue;
			}			
		}

		// Handle the autorun configuration
		if (regSettings.CheckAndOpenKeyForWrite("SOFTWARE\\Microsoft\\windows\\CurrentVersion\\Run"))
		{
			if (iSave == SAVE_SETTINGS)
			{
				if (m_bEnabled)
				{
					// We're saving, so then build an appropriate
					// path to this EXE and set up to load on startup
					CString strPathToExe(_T(""));
					CWinApp* pApp = AfxGetApp(); 
					strPathToExe = pApp->m_pszHelpFilePath;
					int iLastSlash = strPathToExe.ReverseFind('\\');
					strPathToExe = strPathToExe.Left(iLastSlash+1);
					strPathToExe += pApp->m_pszExeName;
					strPathToExe += ".EXE";
					int iPathLen = strPathToExe.GetLength();

					CString strShortPathName(_T(""));
					int iShortNameRetCode = GetShortPathName(strPathToExe, strShortPathName.GetBuffer(iPathLen+1), iPathLen+1);
					strShortPathName.ReleaseBuffer();

					if (iShortNameRetCode >= 0)				
					{
						regSettings.SetValue(STARTUP_NAME, strShortPathName);
					}				
				}
				else
				{
					// We're not enabled, so delete startup info
					regSettings.DeleteValue(STARTUP_NAME);
				}
			}
			else // Load
			{
				// We determine if we're enabled if the startup is in 
				// the registry
				CString strEnabled;
				if (regSettings.GetValue(STARTUP_NAME, strEnabled) && !strEnabled.IsEmpty())				
					m_bEnabled = TRUE;
				else
					m_bEnabled = FALSE;
			}
		}

		regSettings.Disconnect();
	}
	g_critReg.Unlock();
	return NULL;
}

void CBackgroundRandomizerDlg::OnButtonAdd() 
{
	LPMALLOC pMalloc = NULL;
	/* Get's the Shell's default allocator */
	if (::SHGetMalloc(&pMalloc) != NOERROR)
		return;

	CString strDirectoryToAdd(_T(""));

	BROWSEINFO browseInfo;
	LPITEMIDLIST pItemSelected = NULL;

	browseInfo.hwndOwner = this->m_hWnd;
	browseInfo.pidlRoot = NULL;
	browseInfo.pszDisplayName = strDirectoryToAdd.GetBuffer(MAX_PATH);
	browseInfo.lpszTitle = _T("Select a directory");
	browseInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_RETURNFSANCESTORS;
	browseInfo.lpfn = NULL;

	// If the user selected a folder
	if (pItemSelected = ::SHBrowseForFolder(&browseInfo))
	{
		// release buffer - done below also - since it's reused
		strDirectoryToAdd.ReleaseBuffer();

		// Ensure data in memory matches display
		UpdateData();

		// Modify buffer in memory
		::SHGetPathFromIDList(pItemSelected, strDirectoryToAdd.GetBuffer(MAX_PATH));
		strDirectoryToAdd.ReleaseBuffer();
		
		// Set display data to match memory
		UpdateData(FALSE);

		pMalloc->Free(pItemSelected);

		int iCount = m_lstboxDirectories.GetCount();
		CString strDirectoryCheck(_T(""));		
		for (int i=0; i<iCount; i++)
		{
			m_lstboxDirectories.GetText(i, strDirectoryCheck);
			if (strDirectoryCheck == strDirectoryToAdd)
				return;
		}

		m_lstboxDirectories.AddString(strDirectoryToAdd);
	}
	else //Cancel
	{
		//release buffer
		strDirectoryToAdd.ReleaseBuffer();
	}	
}

void CBackgroundRandomizerDlg::OnButtonRemove() 
{
		int iCount = m_lstboxDirectories.GetCount()-1;		
		for (int i=iCount; i>=0; i--)
		{
			if (m_lstboxDirectories.GetSel(i))
				m_lstboxDirectories.DeleteString(i);

		}
	
}

void CBackgroundRandomizerDlg::OnListDirectories() 
{
	m_btnRemove.EnableWindow(m_lstboxDirectories.GetSelCount() > 0);
}

void CBackgroundRandomizerDlg::OnCheckResizeToFillDesktop() 
{
	UpdateData();
	m_btnMaintainAspectRatio.EnableWindow(m_bResizeToDesktopDimensions);
	OnCheckMaintainAspectRatio();
}

void CBackgroundRandomizerDlg::OnCheckMaintainAspectRatio() 
{
	UpdateData();
	m_comboAspectRatioMode.EnableWindow(m_btnMaintainAspectRatio.IsWindowEnabled() && m_bMaintainAspectRatio);	
}

void CBackgroundRandomizerDlg::OnButtonManageBlockedImages() 
{
	m_dlgBlockedImages.DoModal();
}

void CBackgroundRandomizerDlg::OnCheckMatchScreenOrientation() 
{
	UpdateData();
	m_comboAutoRotateToMatchScreen.EnableWindow(m_bMatchScreenOrientation);	
}
