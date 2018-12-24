#if !defined(AFX_BGRANDPOPUPABOUT_H__115F4BC3_AE12_11D5_8394_00E0296DE191__INCLUDED_)
#define AFX_BGRANDPOPUPABOUT_H__115F4BC3_AE12_11D5_8394_00E0296DE191__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BgRandPopupAbout.h : header file
//

#include <afxmt.h>
#include "DlgRegister.h"

/////////////////////////////////////////////////////////////////////////////
// CBgRandPopupAbout dialog

class CBgRandPopupAbout : public CDialog
{
// Construction
public:
	CBgRandPopupAbout(CWnd* pParent = NULL);   // standard constructor
	~CBgRandPopupAbout();

// Dialog Data
	//{{AFX_DATA(CBgRandPopupAbout)
	enum { IDD = IDD_DIALOG_BG_RAND_PARENT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBgRandPopupAbout)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	CDlgRegister* m_pDlgRegister;

// Implementation
public:
	UINT BackgroundThread(LPVOID pParam);
protected:

	// Generated message map functions
	//{{AFX_MSG(CBgRandPopupAbout)
	virtual BOOL OnInitDialog();
	afx_msg void OnMenuChangeImageNow();
	afx_msg void OnMenuTerminate();
	afx_msg void OnMenuSettings();
	afx_msg void OnMenuAbout();
	afx_msg void OnClose();
	afx_msg void OnMenuRegistration();
	afx_msg void OnMenuDontShowImageAgain();
	afx_msg void RotateCurrentImageClockwise90();
	afx_msg void FlipCurrentImageVertically();
	afx_msg void RotateCurrentImageCounterclockwise90();
	afx_msg void OnMenuRotateCurrentImageFlip();
	afx_msg void OnMenuRotateCurrentImageMirror();
	afx_msg void OnMenuSlideShow();
	afx_msg void OnMenuAdjustCurrentImage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:	
	enum { NO_REFRESH=0, REFRESH_NEW_PIC, REFRESH_ROTATE_CURR_PIC };

	BOOL CreateSystemTrayIcon();
	DWORD GetChangeInterval();
	void SetOutputBitmap(CStringList* pStrListFiles = NULL, int iRandChoice = -1);
	void HandleBackgroundPick(CStringList* pStrListFiles);
	void GetWallpaper(CString& strWallpaper);
	void SetWallpaper(CString strWallpaper, BOOL bActiveDesktop);
	bool IsActiveDesktop();
	void HandleUpdateCurrentPicFlags(DWORD dwManipulateFlags);
	void UpdateCurrentPicData(CString& strCurrPic, DWORD dwManipulateFlags);
	DWORD GetCurrentPicData(CString& strCurrPic);
	void SetCurrentPicData(CString& strCurrPic, DWORD dwManipulateFlags);
	void SetCurrentPic(CString& strCurrPic);
	CString GetCurrentPic();
	LRESULT OnTaskbarCreated(WPARAM wp, LPARAM lp);	

	CBackgroundRandomizerDlg m_dlgSettings;
	CCriticalSection m_critCurrPic;
	CString m_strCurrentPic;
	CSystemTray m_TrayIcon;	
	BOOL m_bThreadStarted;
	BOOL m_bThreadRunning;
	BOOL m_bRunning;
	int m_iRefreshMode;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BGRANDPOPUPABOUT_H__115F4BC3_AE12_11D5_8394_00E0296DE191__INCLUDED_)
