#include "stdafx.h"
#include "DesktopInterface.h"
#include "Registry.h"

#define BGRAND_WALLPAPER		"Wallpaper"
#define BGRAND_WALLPAPER_TILE	"TileWallpaper"
#define BGRAND_WALLPAPER_STYLE	"WallpaperStyle"

bool CDesktopInterface::IsWindows98OrLater()
{
	return true;
}

bool CDesktopInterface::IsActiveDesktop()
{
    CoInitialize(NULL);

	bool bActiveDesktop = false;	

	if (IsWindows98OrLater())
	{
		// IActiveDesktop is available, so use it to see what mode we are in
		HRESULT hr;
		IActiveDesktop *pIAD;
		wchar_t str[256] ={0};

		hr = CoCreateInstance (CLSID_ActiveDesktop, 
							   NULL, 
							   CLSCTX_INPROC_SERVER, 
							   IID_IActiveDesktop, 
							   (void**) &pIAD);
		
		if (SUCCEEDED(hr))
		{
			COMPONENTSOPT co;
			co.dwSize = sizeof(co);
			hr = pIAD->GetDesktopItemOptions(&co, 0);
			
			if (SUCCEEDED(hr) && co.fActiveDesktop)
			{
				// Active desktop enabled
				bActiveDesktop = true;
			}

			pIAD->Release();
		}
	}

    CoUninitialize();
	return bActiveDesktop;
}

void CDesktopInterface::GetWallpaper(CString& strWallpaper, BOOL bActiveDesktop)
{
	strWallpaper = "";

	if (bActiveDesktop)
	{
		if (SUCCEEDED(CoInitialize(NULL)))
		{
			HRESULT hr;
			IActiveDesktop *pIAD;
			wchar_t wszActiveDesktopWallpaper[256] = {0};

			hr = CoCreateInstance (CLSID_ActiveDesktop,
								   NULL,
								   CLSCTX_INPROC_SERVER,
								   IID_IActiveDesktop,
								   (void**) &pIAD);
		
			if (SUCCEEDED(hr))
			{
				hr = pIAD->GetWallpaper(wszActiveDesktopWallpaper, 256, 0);

				if (SUCCEEDED(hr))
				{
					char* pExamineBuffer = (char*)wszActiveDesktopWallpaper;
					while (*pExamineBuffer)
					{
						strWallpaper += *pExamineBuffer;
						pExamineBuffer += 2;
					}
				}

				pIAD->Release();
			}

            CoUninitialize();
        }
        else
        {
			MessageBox(NULL, _T("Error - Problem with COM desktop interface."), _T("Background Randomizer Error"), MB_OK);
        }
	}
	else
	{
		COXRegistry regSettings;
		if (regSettings.Connect(COXRegistry::keyCurrentUser))
		{
			if (regSettings.CheckAndOpenKeyForWrite("Control Panel\\Desktop"))
			{
				regSettings.GetValue(BGRAND_WALLPAPER, strWallpaper);
			}
			regSettings.Disconnect();
		}
	}
}

void CDesktopInterface::SetWallpaper(CString strWallpaper, BOOL bActiveDesktop)
{
	if (bActiveDesktop)
	{
		if (SUCCEEDED(CoInitialize(NULL)))
		{
			HRESULT hr;
			IActiveDesktop *pIAD;
			wchar_t wszActiveDesktopWallpaper[256] ={0};

			if (!strWallpaper.IsEmpty())
			{
				LPCTSTR p1 = strWallpaper.GetBuffer(strWallpaper.GetLength()+1);
				LPWSTR p2 = &wszActiveDesktopWallpaper[0];    //now just assign value from ASCII to WIDE
				while (*p1)
				{
					*p2++ = (wchar_t) *p1++;
				}
				strWallpaper.ReleaseBuffer();
				// Now we have a WIDE variable str value from CString string 

				hr = CoCreateInstance (CLSID_ActiveDesktop,
									   NULL,
									   CLSCTX_INPROC_SERVER,
									   IID_IActiveDesktop,
									   (void**) &pIAD);
			
				if (SUCCEEDED(hr))
				{
					// Fake out the switching of wallpaper
					// options to force a redraw
					WALLPAPEROPT wallOpt;
					wallOpt.dwSize = sizeof(WALLPAPEROPT);
					wallOpt.dwStyle = WPSTYLE_TILE;
					pIAD->SetWallpaperOptions(&wallOpt, 0);
					wallOpt.dwStyle = WPSTYLE_CENTER;
					pIAD->SetWallpaperOptions(&wallOpt, 0);

					// Set the wallpaper to what's desired
					hr = pIAD->SetWallpaper(wszActiveDesktopWallpaper, 0);
					if (SUCCEEDED(hr))
					{
						hr = pIAD->ApplyChanges(AD_APPLY_ALL);
					}
					pIAD->Release();
				}
			}

            CoUninitialize();
        }
        else
        {
			MessageBox(NULL, _T("Error - Problem with COM desktop interface."), _T("Background Randomizer Error"), MB_OK);
        }
	}
	else
	{
		COXRegistry regSettings;
		if (regSettings.Connect(COXRegistry::keyCurrentUser))
		{
			if (regSettings.CheckAndOpenKeyForWrite("Control Panel\\Desktop"))
			{
				SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, strWallpaper.GetBuffer(strWallpaper.GetLength()+1), SPIF_SENDCHANGE);
				strWallpaper.ReleaseBuffer();

				regSettings.SetValue(BGRAND_WALLPAPER, strWallpaper);
				// set the other wallpaper settings
				regSettings.SetValue(BGRAND_WALLPAPER_TILE, "0");
				regSettings.SetValue(BGRAND_WALLPAPER_STYLE, "0");
			}
			regSettings.Disconnect();
		}
		HWND hwnd_DesktopWindow = ::GetDesktopWindow();
		::PostMessage(hwnd_DesktopWindow, WM_SETTINGCHANGE, NULL, NULL);
		::RedrawWindow(hwnd_DesktopWindow, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
	}
}
