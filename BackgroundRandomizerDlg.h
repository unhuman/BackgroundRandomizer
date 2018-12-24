// BackgroundRandomizerDlg.h : header file
//

#if !defined(AFX_BACKGROUNDRANDOMIZERDLG_H__1D6DA0D5_9D86_11D5_8394_444553540000__INCLUDED_)
#define AFX_BACKGROUNDRANDOMIZERDLG_H__1D6DA0D5_9D86_11D5_8394_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>
#include "ColorStaticST.h"
#include "DlgManageBlockedImages.h"

enum { GET_FILE_LIST = 0, LOAD_WORKING, LOAD_INIT, SAVE_SETTINGS };

#define STARTUP_NAME					"unHUmanBackgroundRandomizer"
#define DONT_DUPLICATE_IMAGES			"DontDuplicateImages"
#define DIRECTORIES						"Directories"
//#define BLOCKED_FILES					from CDlgManageBlockedImages
#define MATCH_SCREEN_ORIENTATION		"MatchScreenOrientation"
#define AUTO_ROTATE_TO_MATCH_SCREEN		"AutoRotateMatchScreen"
#define RESIZE_TO_DESKTOP_DIMENSIONS	"ResizeToDesktopDimensions"
#define MAINTAIN_ASPECT_RATIO			"MaintainAspectRatio"
#define OVERSIZE_MODE					"OversizeMode"
#define CHANGE_INTERVAL					"ChangeInterval"


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CColorStaticST	m_staticEmail;
	CColorStaticST	m_staticRegister;
	CColorStaticST	m_staticWebSite;
	CString	m_strScreenMode;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnStaticWebSite();
	afx_msg void OnStaticEmail();
	afx_msg void OnStaticRegister();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CBackgroundRandomizerDlg dialog

class CBackgroundRandomizerDlg : public CDialog
{
// Construction
public:
	CBackgroundRandomizerDlg(CWnd* pParent = NULL);	// standard constructor	

// Dialog Data
	//{{AFX_DATA(CBackgroundRandomizerDlg)	
	enum { IDD = IDD_BACKGROUNDRANDOMIZER_DIALOG };
	CComboBox	m_comboAutoRotateToMatchScreen;
	CComboBox	m_comboAspectRatioMode;
	CButton	m_btnMaintainAspectRatio;
	CButton	m_btnRemove;
	CListBox	m_lstboxDirectories;
	BOOL	m_bEnabled;
	BOOL	m_bDontDuplicateImages;
	BOOL	m_bMaintainAspectRatio;
	BOOL	m_bResizeToDesktopDimensions;
	int		m_iOversizeMode;
	BOOL	m_bMatchScreenOrientation;
	int		m_iChangeInterval;
	int		m_iAutoRotateToMatchScreen;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBackgroundRandomizerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CBackgroundRandomizerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDone();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonRemove();
	afx_msg void OnListDirectories();
	afx_msg void OnCheckResizeToFillDesktop();
	afx_msg void OnCheckMaintainAspectRatio();
	afx_msg void OnCancel();
	afx_msg void OnButtonManageBlockedImages();
	afx_msg void OnCheckMatchScreenOrientation();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:	
	CDlgManageBlockedImages m_dlgBlockedImages;
	CStringList* GetFilesAvailable();
	CStringList* FindFileList(CStringArray* pStrArrayDirectories, CStringArray* pStrArrayBlockFiles);
private:		
	CStringList* SaveDataToRegistry(int iSave = SAVE_SETTINGS);	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BACKGROUNDRANDOMIZERDLG_H__1D6DA0D5_9D86_11D5_8394_444553540000__INCLUDED_)
