#include "stdafx.h"		// standard MFC include
#include "GraphicsConverter.h"
#include "PLFilterResizeBilinear.h"
#include "PLFilterCrop.h"
#include "PLFilterFlip.h"
#include "PLFilterMirror.h"
#include "HRUPLFilterRotate90.h"
#include "HRUPLFilterRotate180.h"
#include "HRUPLFilterRotate270.h"
#include "PLBmpEnc.h"
#include "PLJpegEnc.h"
#include "PLFileSink.h"
#include "PLWinBMP.h"


CGraphicsConverter::CGraphicsConverter()
{
}

CGraphicsConverter::~CGraphicsConverter()
{
	FreeBMP();
}

void CGraphicsConverter::FreeBMP()
{
}

BOOL CGraphicsConverter::LoadImage(CString strJPGFilename, int iBitness /* = 0 */)
{
	// Clear out the bitmap
	FreeBMP();

	// Create a decoder.
	PLAnyPicDecoder Decoder;

	// Tell the decoder to fill 
	// the bitmap object with data.
	try
	{
		Decoder.MakeBmpFromFile(strJPGFilename, &m_Bmp, iBitness);
	}
	catch(...)
	{
		FreeBMP();
		return FALSE;
	}

	return TRUE;
}

BOOL CGraphicsConverter::SaveImage(CString strBMPFilename)
{	
	BOOL bSaved = FALSE;
	try 
	{
		PLFileSink fileOutput;
		if (fileOutput.Open(strBMPFilename, m_Bmp.GetMemUsed()) != -1)
		{
			if (strBMPFilename.Right(4) == ".bmp")
			{
				// Create an encoder.
				PLBmpEncoder Encoder;
				Encoder.SaveBmp(&m_Bmp, &fileOutput);
				bSaved = TRUE;
			} 
			else if (strBMPFilename.Right(4) == ".jpg")
			{
				// Create an encoder.
				PLJPEGEncoder Encoder;
				Encoder.SetQuality(50);
				Encoder.SetOptimizeCoding(true);
				Encoder.SaveBmp(&m_Bmp, &fileOutput);
				bSaved = TRUE;
			}

			fileOutput.Close();
		}
	}
	catch(...)
	{
		return FALSE;
	}

	return bSaved;
}

BOOL CGraphicsConverter::ManipulateImage(DWORD dwManipulateFlags)
{
	try
	{
		// Rotate
		DWORD dwClockwiseNinetyDegreeIncrements = dwManipulateFlags & BGRAND_ROTATE_MAP;

		float fRotationRadians = (float)dwClockwiseNinetyDegreeIncrements*90;
		fRotationRadians *= ((float)3.141509434/(float)180.0);

		PLPixel32 pixel(0, 0, 0, 0);
		switch (dwClockwiseNinetyDegreeIncrements)
		{
			case BGRAND_ROTATE_CLOCKWISE90:
				m_Bmp.ApplyFilter(HRUPLFilterRotate90());
				break;
			case BGRAND_ROTATE_CLOCKWISE180:
				m_Bmp.ApplyFilter(HRUPLFilterRotate180());
				break;
			case BGRAND_ROTATE_CLOCKWISE270:
				m_Bmp.ApplyFilter(HRUPLFilterRotate270());
				break;
		}		

		// Flip
		if (dwManipulateFlags & BGRAND_FLIP)
			m_Bmp.ApplyFilter(PLFilterFlip());

		// Mirror
		if (dwManipulateFlags & BGRAND_MIRROR)
			m_Bmp.ApplyFilter(PLFilterMirror());
	}
	catch(...)
	{
		FreeBMP();
		return FALSE;
	}

	return TRUE;
}

BOOL CGraphicsConverter::ResizeImage(int iHoriz, int iVert)
{
	try
	{
		// Resize
		m_Bmp.ApplyFilter(PLFilterResizeBilinear(iHoriz, iVert));

		// Get the screen resolution
		int iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		int iScreenHeight = GetSystemMetrics(SM_CYSCREEN);

		// Now extract the center portion
		int iStartX = 0;
		int iStartY = 0;
		if (iHoriz > iScreenWidth)
			iStartX = (iHoriz - iScreenWidth) / 2;
		if (iVert > iScreenHeight)
			iStartY = (iVert - iScreenHeight) / 2;

		if (iHoriz || iVert)
		{
			m_Bmp.ApplyFilter(PLFilterCrop(max(iStartX, 0), max(iStartY, 0),
										   min(iStartX+iScreenWidth-1, iHoriz-1), 
										   min(iStartY+iScreenHeight-1, iVert-1)));
		}

		return TRUE;
	}
	catch(...)
	{
		FreeBMP();
		return FALSE;
	}
}

int CGraphicsConverter::SizePic(CString strInput, DWORD dwImageFlags,
								float fDesiredWidth, float fDesiredHeight,
								BOOL bRequireMatchingAspectRatio,
								BOOL bResizeToFillDesiredSpace,
								BOOL bMaintainAspectRatio,
								int iAutoRotateToMatchAspectRatio,
								int iSizeMode)
{	
	BOOL bSizePicWorked = FALSE;
	if (LoadImage(strInput, 0))
	{
		if (!ManipulateImage(dwImageFlags))
			return PROCESS_PICK_FAILED; 

		float fCurrWidth;
		float fCurrHeight;
		for (int i = 0; i < 2; i++)
		{
			// Get the current dimentions
			fCurrWidth = (float)m_Bmp.GetWidth();
			fCurrHeight = (float)m_Bmp.GetHeight();

			// If any of the dimensions are invalid, fail
			if ((fCurrWidth <= 0) || (fCurrHeight <= 0) || 
				(fDesiredWidth < 0) || (fDesiredHeight <= 0))
				return PROCESS_PICK_FAILED;

			// If the aspect ratios must match for the image
			// to be placed on the screen, ensure they do
			if (bRequireMatchingAspectRatio)
			{
				float fImageAspect = fCurrWidth / fCurrHeight;
				float fScreenAspect = fDesiredWidth / fDesiredHeight;

				if ((fImageAspect < 1) && (fScreenAspect > 1) ||
					(fImageAspect > 1) && (fScreenAspect < 1))
				{
					switch (iAutoRotateToMatchAspectRatio)
					{
						case 1: // Clockwise
							if (!ManipulateImage(BGRAND_ROTATE_CLOCKWISE90))
								return PROCESS_PICK_FAILED;
							continue;
							break;
						case 2: // Counterclockwise
							if (!ManipulateImage(BGRAND_ROTATE_CLOCKWISE270))
								return PROCESS_PICK_FAILED;
							continue;
							break;
						default:
							FreeBMP();
							return PROCESS_PICK_INVALID_ASPECT;
					}				
				}
			}
			// skip the loop
			break;
		}
		
		if (bResizeToFillDesiredSpace)
		{			
			// Maintain aspect ratio preserves the actual
			// aspect ratio of the image onto the screen
			// May cause empty space on screen
			// or oversizing of the image if 
			if (bMaintainAspectRatio)
			{
				// Calculate the scaling ratios
				float fHorizontalScale = fDesiredWidth/fCurrWidth;
				float fVerticalScale = fDesiredHeight/fCurrHeight;

				float fUseSizeMode = fHorizontalScale;

				switch (iSizeMode)
				{
					// Choose the appropriate scale
					case RESIZE_FIT_ENTIRE_BITMAP_IN_SPACE:
						if (fCurrHeight * fHorizontalScale < fDesiredHeight + 1)
							fUseSizeMode = fHorizontalScale;
						else
							fUseSizeMode = fVerticalScale;
						break;
					case RESIZE_GUARANTEE_SPACE_USED:
						if (fCurrHeight * fHorizontalScale < fDesiredHeight + 1)
							fUseSizeMode = fVerticalScale;
						else
							fUseSizeMode = fHorizontalScale;
						break;
					case RESIZE_AVERAGE:
						fUseSizeMode = (fHorizontalScale + fVerticalScale) / 2;
						break;
					default:
						return FALSE;
				}

				// Based on the resize calculations, resize
				fDesiredWidth = fCurrWidth * fUseSizeMode;
				fDesiredHeight = fCurrHeight * fUseSizeMode;
			}

			bSizePicWorked = ResizeImage((int)fDesiredWidth, (int)fDesiredHeight);
			if (!bSizePicWorked)
				FreeBMP();
		}
	}

	return (bSizePicWorked) ? PROCESS_PICK_SUCCESS : PROCESS_PICK_FAILED;
}


int CGraphicsConverter::ProcessPic(CString strInput,
								   CString strOutput,
								   DWORD dwImageFlags,
								   BOOL bRequireMatchingAspectRatio,
								   BOOL bResizeToDesktopDimensions,
								   BOOL bMaintainAspectRatio,
								   int iAutoRotateToMatchAspectRatio,
								   int iSizeMode)
{	
	// Get the screen resolution
	float fDesiredWidth = (float)GetSystemMetrics(SM_CXSCREEN);
	float fDesiredHeight = (float)GetSystemMetrics(SM_CYSCREEN);

	int iRetCode = SizePic(strInput, dwImageFlags, 
						   fDesiredWidth, fDesiredHeight,
						   bRequireMatchingAspectRatio, bResizeToDesktopDimensions,
						   bMaintainAspectRatio, iAutoRotateToMatchAspectRatio, iSizeMode);
	if (iRetCode != PROCESS_PICK_SUCCESS)
		return iRetCode;
	else // SUCCESS
	{
		// Handle the save
		if (SaveImage(strOutput))
			iRetCode = PROCESS_PICK_SUCCESS;
		else
			iRetCode =  PROCESS_PICK_FAILED_SAVE;
	}
	FreeBMP();

	return iRetCode;
}

BOOL CGraphicsConverter::ConvertPaintlibToCBitmap(CBitmap* pBitmap)
{
	// create a CBitmap object from the data within the CWinBmp object
	BITMAPINFOHEADER& bmiHeader=*m_Bmp.GetBMI();
	BITMAPINFO& bmInfo=*(BITMAPINFO*)m_Bmp.GetBMI();
	LPVOID lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors + 
		                  bmInfo.bmiHeader.biClrUsed) + 
			((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	CClientDC dc(NULL);
	HBITMAP hBmp=CreateDIBitmap(dc.m_hDC, &bmiHeader, CBM_INIT, lpDIBBits,
		                        &bmInfo, DIB_RGB_COLORS);

	pBitmap->Detach();
	pBitmap->Attach(hBmp);

	return TRUE;
}