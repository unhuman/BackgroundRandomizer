// BackgroundRandomizer.h : main header file for the BACKGROUNDRANDOMIZER application
//

#if !defined(AFX_BACKGROUNDRANDOMIZER_H__1D6DA0D3_9D86_11D5_8394_444553540000__INCLUDED_)
#define AFX_BACKGROUNDRANDOMIZER_H__1D6DA0D3_9D86_11D5_8394_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "BackgroundRandomizerDlg.h"
#include "SystemTray.h"
#include "DlgRegister.h"

/////////////////////////////////////////////////////////////////////////////
// CBackgroundRandomizerApp:
// See BackgroundRandomizer.cpp for the implementation of this class
//

class CBackgroundRandomizerApp : public CWinApp
{
public:
	int m_iHiddenRegistrationValid;
	int m_iHiddenRegistrationValid2;
	CBackgroundRandomizerApp();	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBackgroundRandomizerApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CBackgroundRandomizerApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void LaunchAboutWindow();
	void LaunchRegisterWindow(CDlgRegister* pDlgRegister);
	void FreeMutex();

protected:
	CWnd m_wndParent;
	CString m_sClassName;

private:
	HANDLE m_hMutex;	
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BACKGROUNDRANDOMIZER_H__1D6DA0D3_9D86_11D5_8394_444553540000__INCLUDED_)
