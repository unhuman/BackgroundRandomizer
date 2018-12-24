#if !defined(AFX_DLGADJUSTIMAGE_H__1066F32D_A3E3_4CE8_81E1_ECD1FBE79214__INCLUDED_)
#define AFX_DLGADJUSTIMAGE_H__1066F32D_A3E3_4CE8_81E1_ECD1FBE79214__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAdjustImage.h : header file
//

#include "GraphicsConverter.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgAdjustImage dialog

class CDlgAdjustImage : public CDialog 
{	
// Construction
private:
	CDlgAdjustImage(CWnd* pParent = NULL);   // standard constructor
public:	
	CDlgAdjustImage(CString strFilename, CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CDlgAdjustImage)
	enum { IDD = IDD_DIALOG_ADJUST_IMAGE_PROPERTIES };
	CBitmapButton	m_btnRotateCounter;
	CBitmapButton	m_btnRotate;
	CButton	m_btnOk;
	CButton	m_btnCancel;
	CStatic	m_bitmapAdjustImage;
	CString	m_strFilename;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAdjustImage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ApplyImageChanges();

	CGraphicsConverter m_grfcvtAdjustImage;
	CBitmap m_bitmapAdjustImageBitmap;

	// Generated message map functions
	//{{AFX_MSG(CDlgAdjustImage)
	afx_msg void OnButtonRotate();
	afx_msg void OnButtonMirror();
	afx_msg void OnButtonFlip();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRotateCounter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGADJUSTIMAGE_H__1066F32D_A3E3_4CE8_81E1_ECD1FBE79214__INCLUDED_)
