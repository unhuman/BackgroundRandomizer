#if !defined(AFX_DLGREGISTER_H__7AE8DCA1_AEC1_11D5_8394_00E0296DE191__INCLUDED_)
#define AFX_DLGREGISTER_H__7AE8DCA1_AEC1_11D5_8394_00E0296DE191__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRegister.h : header file
//

#include "Registry.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgRegister dialog

class CDlgRegister : public CDialog
{
// Construction
public:
	enum {EXPIRED, LICENSED, TRIAL, OLDSTYLE};	
	enum {REGCODE_INVALID, REGCODE_VALID, REGCODE_OLDSTYLE};

	CDlgRegister(CWnd* pParent = NULL,
				 int iNewStyle = FALSE);   // standard constructor
	int CheckRegistrationStatus();

	CString m_strLevelOneReg;

// Dialog Data
	//{{AFX_DATA(CDlgRegister)
	enum { IDD = IDD_DIALOG_REGISTER };
	CStatic	m_staticWebSite;
	CStatic	m_staticRegister;
	CStatic	m_staticEmail;
	CString	m_strUserName;
	CString	m_strLicenseCode;
	CString	m_strExpiration;
	CString	m_strStaticNotice;
	CString	m_strRegCost;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRegister)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRegister)
	afx_msg void OnStaticRegister();
	afx_msg void OnStaticEmail();
	afx_msg void OnStaticWebsite();
	afx_msg void OnButtonCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void GetSetRegistry(BOOL bGetFromRegistry = TRUE);
	int RegCode(CString strUser, CString& strPassword);
	int m_iRegistrationStatus;
	int m_iTrialDaysLeft;
	int m_iNewStyle;

public:
	BOOL CheckInValidTrial()
	{
		BOOL bInValidTrial = FALSE;
		COXRegistry regRegistration;
		CString strWinDir(_T(""));

		GetWindowsDirectory(strWinDir.GetBuffer(MAX_PATH), MAX_PATH);
		strWinDir.ReleaseBuffer();
		if (!strWinDir.GetLength())
		{
			exit(99);
		}

		if (strWinDir.GetAt(strWinDir.GetLength()-1) != _T('\\'))
			strWinDir += _T('\\');

		if (regRegistration.Connect(COXRegistry::keyLocalMachine))
		{
			CString strRegKey(_T(""));
			CString strWinTime(_T(""));
			CString strDDTime(_T(""));
			CString strRegTime(_T(""));
			strRegKey.LoadString(IDS_REGISTRY_KEY);

			strRegKey.LoadString(IDS_REGISTRY_KEY);
			if (regRegistration.CheckAndOpenKeyForWrite(strRegKey))
			{
				// Get the 3 times (registry, Win.ini, and DD.ini)
				time_t tmtNow(0);
				time(&tmtNow);
				DWORD dwTime = tmtNow;
				GetPrivateProfileString(_T("BackgroundRandomizer"),
										_T("Configuration"), _T(""),
										strWinTime.GetBuffer(MAX_PATH), MAX_PATH,
										strWinDir + _T("WIN.INI"));
				strWinTime.ReleaseBuffer();
				GetPrivateProfileString(_T("BackgroundRandomizer"),
										_T("InstallTime"), _T(""),
										strDDTime.GetBuffer(MAX_PATH), MAX_PATH,
										strWinDir + _T("BackgroundRandomizer.INI"));
				strDDTime.ReleaseBuffer();

				// This is the first time this thing has been run, set
				// up registry, WIN.INI, and BackgroundRandomizer.INI
				if (!regRegistration.GetValue(_T("FirstRunTime"), dwTime) &&
					(strWinTime == _T("")) && (strDDTime == _T("")))
				{					
					regRegistration.SetValue(_T("FirstRunTime"), tmtNow);
										
					strWinTime.Format("%x", tmtNow);
					strDDTime = strWinTime;
					WritePrivateProfileString(_T("BackgroundRandomizer"),
											  _T("Configuration"),
											  strWinTime,
											  strWinDir + _T("WIN.INI"));
					WritePrivateProfileString(_T("BackgroundRandomizer"),
											  _T("InstallTime"),
											  strDDTime,
											  strWinDir + _T("BackgroundRandomizer.INI"));
				}

				// Okay, now all the numbers have to match up.
				// If they don't, require the user to register.
				strRegTime.Format("%x", dwTime);
				
				if ((strWinTime == strDDTime) && (strWinTime == strRegTime))
				{						
					// Ensure the user didn't set the clock back 8^)
					CTime tmInstall(dwTime);
					CTime tmNow(tmtNow);
					if (tmNow >= tmInstall)
					{
						CTimeSpan tmDiff(tmNow - tmInstall);
						int iTempDaysRemaining = 30 - tmDiff.GetDays();
						if (iTempDaysRemaining >= 0)
						{
							bInValidTrial = TRUE;
						}
					}
				}
			}
			regRegistration.Disconnect();
		}
		return bInValidTrial;
	}

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGREGISTER_H__7AE8DCA1_AEC1_11D5_8394_00E0296DE191__INCLUDED_)
