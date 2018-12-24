#if !defined(AFX_DLGMANAGEBLOCKEDIMAGES_H__ABF08AE4_B363_11D5_8394_00E0296DE191__INCLUDED_)
#define AFX_DLGMANAGEBLOCKEDIMAGES_H__ABF08AE4_B363_11D5_8394_00E0296DE191__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgManageBlockedImages.h : header file
//

#include "GraphicsConverter.h"

#define BLOCKED_FILES	"BlockedFiles"

/////////////////////////////////////////////////////////////////////////////
// CDlgManageBlockedImages dialog

class CDlgManageBlockedImages : public CDialog
{
// Construction
public:
	CDlgManageBlockedImages(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgManageBlockedImages)
	enum { IDD = IDD_DIALOG_MANAGE_BLOCKED_IMAGES };
	CStatic	m_bitmapPreview;
	CButton	m_btnAllowBlockedImage;
	CListBox	m_lbBlockedImages;
	CString	m_strBlockedImage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgManageBlockedImages)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgManageBlockedImages)
	afx_msg void OnButtonAllowImageAgain();
	afx_msg void OnSelchangeListBlockedImages();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CGraphicsConverter m_grfcvtMiniWindow;
	CBitmap m_bitmapPreviewBitmap;
public:
	CStringArray m_strarrayBlockedImages;
	void AddBlockedImage(CString strImageBlock);
	void LoadSaveSettings(BOOL bLoad, BOOL bHasLock = FALSE);

private:	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMANAGEBLOCKEDIMAGES_H__ABF08AE4_B363_11D5_8394_00E0296DE191__INCLUDED_)
