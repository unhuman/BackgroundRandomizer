// DlgManageBlockedImages.cpp : implementation file
//

#include "stdafx.h"
#include "backgroundrandomizer.h"
#include "DlgManageBlockedImages.h"
#include "Registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCriticalSection g_critReg;

/////////////////////////////////////////////////////////////////////////////
// CDlgManageBlockedImages dialog


CDlgManageBlockedImages::CDlgManageBlockedImages(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgManageBlockedImages::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgManageBlockedImages)
	m_strBlockedImage = _T("");
	//}}AFX_DATA_INIT
}


void CDlgManageBlockedImages::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgManageBlockedImages)
	DDX_Control(pDX, IDC_STATIC_PREVIEW, m_bitmapPreview);
	DDX_Control(pDX, IDC_BUTTON_ALLOW_IMAGE_AGAIN, m_btnAllowBlockedImage);
	DDX_Control(pDX, IDC_LIST_BLOCKED_IMAGES, m_lbBlockedImages);
	DDX_LBString(pDX, IDC_LIST_BLOCKED_IMAGES, m_strBlockedImage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgManageBlockedImages, CDialog)
	//{{AFX_MSG_MAP(CDlgManageBlockedImages)
	ON_BN_CLICKED(IDC_BUTTON_ALLOW_IMAGE_AGAIN, OnButtonAllowImageAgain)
	ON_LBN_SELCHANGE(IDC_LIST_BLOCKED_IMAGES, OnSelchangeListBlockedImages)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgManageBlockedImages message handlers

BOOL CDlgManageBlockedImages::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	LoadSaveSettings(TRUE);
	int iSize = m_strarrayBlockedImages.GetSize();
	
	CString strFileToAddToList;
	for (int i = 0; i<iSize; i++)
		m_lbBlockedImages.AddString(m_strarrayBlockedImages.GetAt(i));
	
	m_btnAllowBlockedImage.EnableWindow(FALSE);

	// hide bitmap window
	m_bitmapPreview.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgManageBlockedImages::OnButtonAllowImageAgain() 
{
	int iCurSel = m_lbBlockedImages.GetCurSel();
	if (iCurSel != -1)
	{
		m_lbBlockedImages.DeleteString(iCurSel);
		m_btnAllowBlockedImage.EnableWindow(FALSE);
		// hide bitmap window
		m_bitmapPreview.ShowWindow(SW_HIDE);
	}	
}

void CDlgManageBlockedImages::OnSelchangeListBlockedImages() 
{
	UpdateData();
	BOOL bSelectedItem = (m_lbBlockedImages.GetCurSel() != -1);
	m_btnAllowBlockedImage.EnableWindow(bSelectedItem);

	if (bSelectedItem)
	{
		CWaitCursor waitCursor;
		CString strFilename;
		m_lbBlockedImages.GetText(m_lbBlockedImages.GetCurSel(), strFilename);		

		// Figure out how big the bitmap window is
		RECT rectBitmap;
		m_bitmapPreview.GetWindowRect(&rectBitmap);

		// Resize to fit
		m_grfcvtMiniWindow.SizePic(strFilename, 0,
								   (float)rectBitmap.right-rectBitmap.left+1,
								   (float)rectBitmap.bottom-rectBitmap.top+1,
								   FALSE, TRUE, TRUE, 0, 0);

		// Set the corner to black so the background looks nice
		PLPixel32 pixelBlack(0,0,0);
		m_grfcvtMiniWindow.m_Bmp.SetPixel(0, 0, pixelBlack);

		// create a CBitmap object from the data within the CWinBmp object
		m_grfcvtMiniWindow.ConvertPaintlibToCBitmap(&m_bitmapPreviewBitmap);

		// now put that bitmap into what we are viewing
		m_bitmapPreview.SetBitmap(m_bitmapPreviewBitmap.operator HBITMAP());

		// Allow the bitmap to be viewed
		m_bitmapPreview.ShowWindow(SW_SHOW);

		// enable the add button
		m_btnAllowBlockedImage.EnableWindow(TRUE);
	}
}

void CDlgManageBlockedImages::OnOK() 
{
	// Populate the member variable with the data within the list
	m_strarrayBlockedImages.RemoveAll();
	int iSize = m_lbBlockedImages.GetCount();
	CString strItem;
	for (int i = 0; i<iSize; i++)
	{
		m_lbBlockedImages.GetText(i, strItem);
		m_strarrayBlockedImages.Add(strItem);
	}

	// Save the settings
	LoadSaveSettings(FALSE);
	
	CDialog::OnOK();
}

void CDlgManageBlockedImages::LoadSaveSettings(BOOL bLoad, BOOL bHasLock /* = FALSE */) 
{
	if (!bHasLock)
		g_critReg.Lock();

	COXRegistry regSettings;
	if (regSettings.Connect(COXRegistry::keyLocalMachine))
	{
		if (regSettings.CheckAndOpenKeyForWrite("SOFTWARE\\unHUman Software\\BackgroundRandomizer"))
		{
			if (!bLoad) // save
				regSettings.SetValue(BLOCKED_FILES, m_strarrayBlockedImages);
			else // load
				regSettings.GetValue(BLOCKED_FILES, m_strarrayBlockedImages);
		}

		regSettings.Disconnect();
	}
	if (!bHasLock)
		g_critReg.Unlock();
}

void CDlgManageBlockedImages::AddBlockedImage(CString strImageBlock)
{
	// Load the settings
	LoadSaveSettings(TRUE);

	// Don't allow duplicates
	for (int i = 0; i <= m_strarrayBlockedImages.GetUpperBound(); i++)
	{
		if (m_strarrayBlockedImages[i] == strImageBlock)
			return;
	}

	// Add the item
	m_strarrayBlockedImages.Add(strImageBlock);

	// Save the settings
	LoadSaveSettings(FALSE);
}
