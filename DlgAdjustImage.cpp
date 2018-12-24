// DlgAdjustImage.cpp : implementation file
//

#include "stdafx.h"
#include "backgroundrandomizer.h"
#include "DlgAdjustImage.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAdjustImage dialog


CDlgAdjustImage::CDlgAdjustImage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAdjustImage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdjustImage)
	m_strFilename = _T("");
	//}}AFX_DATA_INIT
}

CDlgAdjustImage::CDlgAdjustImage(CString strFilename, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAdjustImage::IDD, pParent)
{	
	m_strFilename = strFilename;	
}

void CDlgAdjustImage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdjustImage)
	DDX_Control(pDX, IDC_BUTTON_ROTATE_COUNTER, m_btnRotateCounter);
	DDX_Control(pDX, IDC_BUTTON_ROTATE, m_btnRotate);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_STATIC_PREVIEW, m_bitmapAdjustImage);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_strFilename);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAdjustImage, CDialog)
	//{{AFX_MSG_MAP(CDlgAdjustImage)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE, OnButtonRotate)
	ON_BN_CLICKED(IDC_BUTTON_MIRROR, OnButtonMirror)
	ON_BN_CLICKED(IDC_BUTTON_FLIP, OnButtonFlip)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE_COUNTER, OnButtonRotateCounter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAdjustImage message handlers

void CDlgAdjustImage::ApplyImageChanges()
{
	// create a CBitmap object from the data within the CWinBmp object
	m_grfcvtAdjustImage.ConvertPaintlibToCBitmap(&m_bitmapAdjustImageBitmap);

	// now put that bitmap into what we are viewing
	m_bitmapAdjustImage.SetBitmap(m_bitmapAdjustImageBitmap.operator HBITMAP());
}

void CDlgAdjustImage::OnButtonRotate() 
{
	// Rotate image 90° clockwise	
	m_grfcvtAdjustImage.ManipulateImage(BGRAND_ROTATE_CLOCKWISE90);
	ApplyImageChanges();
}

void CDlgAdjustImage::OnButtonRotateCounter() 
{
	// Rotate image 90° counter clockwise (= 270° clockwise)
	m_grfcvtAdjustImage.ManipulateImage(BGRAND_ROTATE_CLOCKWISE270);
	ApplyImageChanges();
}

void CDlgAdjustImage::OnButtonMirror() 
{
	// Mirror Image
	m_grfcvtAdjustImage.ManipulateImage(BGRAND_MIRROR);
	ApplyImageChanges();
}

void CDlgAdjustImage::OnButtonFlip() 
{
	// Flip Image
	m_grfcvtAdjustImage.ManipulateImage(BGRAND_FLIP);
	ApplyImageChanges();
}

BOOL CDlgAdjustImage::OnInitDialog() 
{
	m_btnRotate.AutoLoad(IDC_BUTTON_ROTATE, this);
	m_btnRotateCounter.AutoLoad(IDC_BUTTON_ROTATE_COUNTER, this);

	CDialog::OnInitDialog();

	// put the bitmaps into the buttons	
	m_btnRotate.LoadBitmaps(IDI_ICON_ROTATE_CLOCK);
	m_btnRotateCounter.LoadBitmaps(IDI_ICON_ROTATE_COUNTER);

	// Figure out how big the bitmap window is
	RECT rectBitmap;
	m_bitmapAdjustImage.GetWindowRect(&rectBitmap);

	// Resize to fit
	m_grfcvtAdjustImage.SizePic(m_strFilename, 0,
								(float)rectBitmap.right-rectBitmap.left+1,
								(float)rectBitmap.bottom-rectBitmap.top+1,
								FALSE, TRUE, TRUE, 0, 0);

	// Set the four corners to black so the background looks nice
	PLPixel32 pixelBlack(0,0,0);
	int iWidth = m_grfcvtAdjustImage.m_Bmp.GetWidth();
	int iHeight = m_grfcvtAdjustImage.m_Bmp.GetHeight();
	m_grfcvtAdjustImage.m_Bmp.SetPixel(0, 0, pixelBlack);
	m_grfcvtAdjustImage.m_Bmp.SetPixel(iWidth-1, 0, pixelBlack);
	m_grfcvtAdjustImage.m_Bmp.SetPixel(iWidth-1, iHeight-1, pixelBlack);
	m_grfcvtAdjustImage.m_Bmp.SetPixel(0, iHeight-1, pixelBlack);

	// Update display
	ApplyImageChanges();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

