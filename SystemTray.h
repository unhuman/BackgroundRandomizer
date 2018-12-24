#ifndef _INCLUDED_SYSTEMTRAY_H_
#define _INCLUDED_SYSTEMTRAY_H_

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CSystemTray window

class CSystemTray : public CWnd
{
// Construction/destruction
public:
    CSystemTray();
    CSystemTray(CWnd* pWnd, UINT uCallbackMessage, LPCTSTR szTip, HICON icon, UINT uID);
    virtual ~CSystemTray();

    DECLARE_DYNAMIC(CSystemTray)

// Operations
public:
	void SetMyParentWindow(CWnd* pWnd) 
	{ 
		m_pParentWnd = pWnd; 
	}

    BOOL GetEnabled() 
	{ 
		return m_bEnabled; 
	}
    BOOL GetVisible() 
	{ 
		return !m_bHidden; 
	}
	void FlipSlideShow() 
	{
		m_bSlideShowChecked = !m_bSlideShowChecked;
	}
	BOOL GetSlideShow()
	{
		return m_bSlideShowChecked;
	}

    // Create the tray icon
    Create(CWnd* pParent, UINT uCallbackMessage, LPCTSTR szTip, HICON icon, UINT uID);

    // Change or retrieve the Tooltip text
    BOOL    SetTooltipText(LPCTSTR pszTooltipText);
    BOOL    SetTooltipText(UINT nID);
    CString GetTooltipText() const;

    // Change or retrieve the icon displayed
    BOOL  SetIcon(HICON hIcon);
    BOOL  SetIcon(LPCTSTR lpszIconName);
    BOOL  SetIcon(UINT nIDResource);
    BOOL  SetStandardIcon(LPCTSTR lpIconName);
    BOOL  SetStandardIcon(UINT nIDResource);
    HICON GetIcon() const;
    void  HideIcon();
    void  ShowIcon(BOOL bForceShow = FALSE);
	void  ShutdownPrepareForSystemExit();
    void  RemoveIcon();
	void  MoveToRight();

    // For icon animation
    BOOL  SetIconList(UINT uFirstIconID, UINT uLastIconID); 
    BOOL  SetIconList(HICON* pHIconList, UINT nNumIcons); 
    BOOL  Animate(UINT nDelayMilliSeconds, int nNumSeconds = -1);
    BOOL  StepAnimation();
    BOOL  StopAnimation();

    // Change menu default item
    void GetMenuDefaultItem(UINT& uItem, BOOL& bByPos);
    BOOL SetMenuDefaultItem(UINT uItem, BOOL bByPos);

	UINT FindMenuItem(CMenu* pMenu, CString strLookText);
	void SetShowRegistration(BOOL bShowRegistration);

    // Change or retrieve the window to send notification messages to
    BOOL  SetNotificationWnd(CWnd* pNotifyWnd);
    CWnd* GetNotificationWnd() const;

    // Default handler for tray notification message
    virtual LRESULT OnTrayNotification(WPARAM uID, LPARAM lEvent);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSystemTray)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
    void Initialize();

    BOOL            m_bEnabled;   // does O/S support tray icon?
    BOOL            m_bHidden;    // Has the icon been hidden?
    NOTIFYICONDATA  m_tnd;
	CWnd* m_pParentWnd;

    CArray<HICON, HICON> m_IconList;
    static UINT  m_nIDEvent;
    UINT         m_uIDTimer;
    int          m_nCurrentIcon;
    COleDateTime m_StartTime;
    int          m_nAnimationPeriod;
    HICON        m_hSavedIcon;
    UINT         m_DefaultMenuItemID;
    BOOL         m_DefaultMenuItemByPos;
	BOOL		 m_bShowRegister;
	BOOL		 m_bSlideShowChecked;

// Generated message map functions
protected:
	//{{AFX_MSG(CSystemTray)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};


#endif

/////////////////////////////////////////////////////////////////////////////
