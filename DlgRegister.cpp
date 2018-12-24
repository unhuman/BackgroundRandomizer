// DlgRegister.cpp : implementation file
//

#include "stdafx.h"
#include "backgroundrandomizer.h"
#include "DlgRegister.h"
#include "..\Registration\RegCode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRegister dialog


CDlgRegister::CDlgRegister(CWnd* pParent /*=NULL*/, int iNewStyle /*= FALSE */)
	: CDialog(CDlgRegister::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRegister)
	m_strUserName = _T("");
	m_strLicenseCode = _T("");
	m_strExpiration = _T("");
	m_strStaticNotice = _T("");
	m_strRegCost = _T("");
	//}}AFX_DATA_INIT

	m_strRegCost.LoadString(IDS_STRING_REGCOST_NEW);
	m_strExpiration.LoadString(IDS_STRING_EXPIRATION_TRIAL);

	m_iRegistrationStatus = EXPIRED;
	m_iTrialDaysLeft = -1;
	m_iNewStyle = iNewStyle;

	m_strLevelOneReg = "";

	GetSetRegistry(TRUE);	
}


void CDlgRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRegister)
	DDX_Control(pDX, IDC_STATIC_WEBSITE, m_staticWebSite);
	DDX_Control(pDX, IDC_STATIC_REGISTER, m_staticRegister);
	DDX_Control(pDX, IDC_STATIC_EMAIL, m_staticEmail);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_LICENSE, m_strLicenseCode);
	DDX_Text(pDX, IDC_STATIC_EXPIRATION, m_strExpiration);
	DDX_Text(pDX, IDC_STATIC_NOTICE, m_strStaticNotice);
	DDX_Text(pDX, IDC_STATIC_REGCOST, m_strRegCost);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRegister, CDialog)
	//{{AFX_MSG_MAP(CDlgRegister)
	ON_BN_CLICKED(IDC_STATIC_REGISTER_CARDS, OnStaticRegister)
	ON_BN_CLICKED(IDC_STATIC_EMAIL, OnStaticEmail)
	ON_BN_CLICKED(IDC_STATIC_WEBSITE, OnStaticWebsite)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRegister message handlers
int CDlgRegister::CheckRegistrationStatus()
{
	return m_iRegistrationStatus;
}

void CDlgRegister::OnStaticRegister() 
{
	ShellExecute(m_hWnd,NULL,"http://www.unhuman.com",NULL,NULL,SW_SHOWNORMAL);	
}

void CDlgRegister::OnStaticEmail() 
{
	ShellExecute(m_hWnd,NULL,"mailto:human@unhuman.com?subject=Background Randomizer",NULL,NULL,SW_SHOWNORMAL);		
}

void CDlgRegister::OnStaticWebsite() 
{
	ShellExecute(m_hWnd,NULL,"https://www.regnow.com/softsell/nph-softsell.cgi?item=2259-5",NULL,NULL,SW_SHOWNORMAL);
}

void CDlgRegister::OnButtonCancel() 
{
	m_strUserName = "";
	CDialog::OnCancel();
}

void CDlgRegister::OnOK() 
{
	UpdateData();

	// Make trimmed off copies of the username and licensecode
	CString strTestUserName(m_strUserName);
	strTestUserName.TrimLeft();
	strTestUserName.TrimRight();
	m_strLicenseCode.TrimLeft();
	m_strLicenseCode.TrimRight();
	CString strTestLicenseCode(m_strLicenseCode);

	// Write the data back to the screen - bug in 2.0
	UpdateData(FALSE);

	int iResult = RegCode(strTestUserName, strTestLicenseCode);
	if ((iResult == REGCODE_VALID))
	{	
		//We're licensed
		m_iRegistrationStatus = LICENSED;
		GetSetRegistry(FALSE);

		AfxMessageBox(IDS_THANK_YOU);

		CDialog::OnOK();
	}
	else
	{
		AfxMessageBox(IDS_INVALID_USERNAME_LICENSE);		
	}
}

void CDlgRegister::GetSetRegistry(BOOL bGetFromRegistry /*TRUE*/)
{
	COXRegistry regRegistration;
	BOOL bSuccess(FALSE);
	CString strUser(_T(""));
	CString strCode(_T(""));	
	CString strWinDir(_T(""));

	GetWindowsDirectory(strWinDir.GetBuffer(MAX_PATH), MAX_PATH);
	strWinDir.ReleaseBuffer();
	if (!strWinDir.GetLength())
	{
		AfxMessageBox(IDS_FAILURE_GETWINDIR);
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
		if (regRegistration.CheckAndOpenKeyForWrite(strRegKey))
		{	if (bGetFromRegistry)
			{
				bSuccess = regRegistration.GetValue(_T("UserName"), m_strUserName);
				if (bSuccess)
					bSuccess = regRegistration.GetValue(_T("RegistrationCode"), m_strLicenseCode);

				//If there's valid info in the registry
				int iResult;
				if (bSuccess && 
					((iResult = RegCode(m_strUserName, m_strLicenseCode)) != REGCODE_INVALID))
				{
					m_iRegistrationStatus = (iResult == REGCODE_OLDSTYLE) ? OLDSTYLE : LICENSED;
				}
				else //this copy is *NOT* registered - date check time
				{
					//Invalid stuff in registry - blank it out
					m_strUserName = _T("");
					m_strLicenseCode = _T("");

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
					
					if ((strWinTime != strDDTime) || (strWinTime != strRegTime))
					{						
						m_iRegistrationStatus = EXPIRED;
					}
					else
					{
						// Check the time - if it's over 30 days,
						// expire, otherwise, we're still in a trial
						m_iRegistrationStatus = EXPIRED;

						// Ensure the user didn't set the clock back 8^)
						CTime tmInstall(dwTime);
						CTime tmNow(tmtNow);
						if (tmNow >= tmInstall)
						{
							CTimeSpan tmDiff(tmNow - tmInstall);
							m_iTrialDaysLeft = 30 - tmDiff.GetDays();
							if (m_iTrialDaysLeft >= 0)
							{
								((CBackgroundRandomizerApp*)AfxGetApp())->m_iHiddenRegistrationValid = 1;
								m_iRegistrationStatus = TRIAL;
							}
						}
					}
					// If the license expired, write garbage back out
					// so the program won't be able to be used again
					if (m_iRegistrationStatus == EXPIRED)
					{
						// Populate INI files with random values
						// so registration never works, even if clock
						// is set back
						srand(tmtNow);
						strRegTime.Format("%x", rand() + 2353829);
						WritePrivateProfileString(_T("BackgroundRandomizer"),
												  _T("Configuration"),
												  strRegTime,
												  strWinDir + _T("WIN.INI"));
						strRegTime.Format("%x", rand() + 53324853);
						WritePrivateProfileString(_T("BackgroundRandomizer"),
												  _T("InstallTime"),
												  strRegTime,
												  strWinDir + _T("BackgroundRandomizer.INI"));
					}
				}
			}
			else
			{
				UpdateData();
				regRegistration.SetValue("UserName", m_strUserName);
				regRegistration.SetValue("RegistrationCode", m_strLicenseCode);
			}
		}
		regRegistration.Disconnect();
	}
}


int CDlgRegister::RegCode(CString strUser, CString& strPassword)
{
	// Generate the registration code - 9/25/2001
	unsigned long L0 = 0x9842377AL;
	unsigned long L1 = 0xA823CDE8L;
	unsigned long L2 = 0x9423AB92L;
	unsigned long L3 = 0x1349742FL;
	unsigned long L4 = 0xCAC94FA8L;
	CString strTempRegCode(_T(""));
	CRegCode codeRegistration(L0, L1, L2, L3, L4);
	if (!codeRegistration.GenerateRegCode(strUser, strTempRegCode))
		return REGCODE_INVALID;
	
	// If this is the NewStyle, then allow case identical strings,
	// but not an identical string
	if (m_iNewStyle)
	{
		((CBackgroundRandomizerApp*)AfxGetApp())->m_iHiddenRegistrationValid = 0;
		CString strPasswordLower(strPassword);
		strPasswordLower.MakeLower();
		CString strTempRegCodeLower(strTempRegCode);
		strTempRegCodeLower.MakeLower();

		if (strPasswordLower == strTempRegCodeLower)
		{
			// Store away the level one registration
			m_strLevelOneReg = strTempRegCode;

			((CBackgroundRandomizerApp*)AfxGetApp())->m_iHiddenRegistrationValid = (strTempRegCode == strPassword) ? 0 : 1;
		}
	}

	// Validate the password for the return code	
	if ((strTempRegCode == strPassword) && m_iNewStyle)
		return REGCODE_OLDSTYLE;

	// If this is the NewStyle, then allow case identical strings
	if (m_iNewStyle)
	{
		CString strPasswordLower(strPassword);
		strPasswordLower.MakeLower();
		strTempRegCode.MakeLower();

		if (strPasswordLower == strTempRegCode)
			return REGCODE_VALID;
	}
	
	return REGCODE_INVALID;
}

int CDlgRegister::DoModal() 
{
	if ((m_iRegistrationStatus == EXPIRED) ||
		(m_iRegistrationStatus == OLDSTYLE))
		m_strStaticNotice.LoadString(IDS_EXPIRED_NOTICE);
	else if (m_iTrialDaysLeft >= 0) // TRIAL
		m_strStaticNotice.Format(IDS_TRIAL_NOTICE, m_iTrialDaysLeft);
	else 
		m_strStaticNotice = "Thank you for registering Background Randomizer.";
	
	return CDialog::DoModal();
}
