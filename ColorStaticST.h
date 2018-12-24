//
//	Class:		CColorStaticST
//
//	Compiler:	Visual C++
//	Tested on:	Visual C++ 5.0 - 6.0
//
//	Version:	See GetVersionC() or GetVersionI()
//
//	Created:	xx/xxxx/1998
//	Updated:	30/July/1999
//
//	Author:		Davide Calabro'		davide_calabro@yahoo.com
//
#ifndef _COLORSTATICST_H
#define _COLORSTATICST_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define COLOR_STATIC_LINK_COLOR		RGB(0, 0, 238) // Blue

class CColorStaticST : public CStatic
{
public:
	CColorStaticST();
    enum {	ST_FLS_SLOW,
			ST_FLS_NORMAL,
			ST_FLS_FAST};

public:

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorStaticST)
	//}}AFX_VIRTUAL

public:
	virtual ~CColorStaticST();

	void SetTextColor(COLORREF crTextColor = 0xffffffff);
	COLORREF GetTextColor();

	void SetBkColor(COLORREF crBkColor = 0xffffffff);
	COLORREF GetBkColor();

	void SetBlinkTextColors(COLORREF crBlinkTextColor1, COLORREF crBlinkTextColor2);
	void StartTextBlink(BOOL bStart = TRUE, UINT nElapse = ST_FLS_NORMAL);

	void SetBlinkBkColors(COLORREF crBlinkBkColor1, COLORREF crBlinkBkColor2);
	void StartBkBlink(BOOL bStart = TRUE, UINT nElapse = ST_FLS_NORMAL);

	void EnableNotify(CWnd* pParent = NULL, UINT nMsg = WM_USER);

	static const short GetVersionI();
	static const char* GetVersionC();

protected:
	//{{AFX_MSG(CColorStaticST)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	UINT m_nTimerId;

	COLORREF m_crTextColor;
	COLORREF m_crBlinkTextColors[2];
	BOOL m_bTextBlink;
	int m_nTextBlinkStep;

	COLORREF m_crBkColor;
	COLORREF m_crBlinkBkColors[2];
	BOOL m_bBkBlink;
	int m_nBkBlinkStep;

	CBrush m_brBkgnd;
	CBrush m_brBlinkBkgnd[2];

	CWnd* m_pParent;
	UINT m_nMsg;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif 
