#ifndef GraphicsConverter
#define GraphicsConverter

#include "planydec.h"
#include "plwinbmp.h"

#define RESIZE_FIT_ENTIRE_BITMAP_IN_SPACE	0
#define RESIZE_GUARANTEE_SPACE_USED			1
#define RESIZE_AVERAGE						2

#define PROCESS_PICK_SUCCESS			0
#define PROCESS_PICK_FAILED				-1
#define PROCESS_PICK_FAILED_SAVE		-2
#define PROCESS_PICK_INVALID_ASPECT		-3

// Flags for Rotation, Flipping, and Mirroring
#define BGRAND_ROTATE_CLOCKWISE90	0x01 
#define BGRAND_ROTATE_CLOCKWISE180	0x02
#define BGRAND_ROTATE_CLOCKWISE270	0x03
#define BGRAND_ROTATE_MAP			0x0F
#define BGRAND_FLIP					0x10
#define BGRAND_MIRROR				0x20


class CGraphicsConverter
{
	friend class CDlgAdjustImage;
	friend class CDlgManageBlockedImages;

public:
	CGraphicsConverter();
	~CGraphicsConverter();

	void FreeBMP();

	int SizePic(CString strInput, DWORD dwImageFlags,
				float fDesiredWidth, float fDesiredHeight,
				BOOL bRequireMatchingAspectRatio, BOOL bResizeToFillDesiredSpace,
				BOOL bMaintainAspectRatio, int iAutoRotateToMatchAspectRatio, 
				int iSizeMode);
	
	int ProcessPic(CString strInput, CString strOutput,
				   DWORD dwImageFlags,
				   BOOL bRequireMatchingAspectRatio,
				   BOOL bResizeToFillDesktop,
				   BOOL bMaintainAspectRatio,
				   int iAutoRotateToMatchAspectRatio,
				   int iMode);

	BOOL ConvertPaintlibToCBitmap(CBitmap* pBitmap);
private:		
	// Used by process pic
	BOOL LoadImage(CString strJPGFilename, int iBitness = 0);
	BOOL SaveImage(CString strBMPFilename);
	BOOL ResizeImage(int iHoriz, int iVert);
	BOOL ManipulateImage(DWORD dwManipulateFlags);
	BOOL FlipImage();
	BOOL MirrorImage();

	PLWinBmp m_Bmp;
};

#endif