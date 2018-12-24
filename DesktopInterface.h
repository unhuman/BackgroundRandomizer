#ifndef BACKGROUND_RANDOMIZER_DESKTOP_INTERFACE
#define BACKGROUND_RANDOMIZER_DESKTOP_INTERFACE

class CDesktopInterface
{
private:
	static bool IsWindows98OrLater();
public:
	static bool IsActiveDesktop();
	static void SetWallpaper(CString strWallpaper, BOOL bActiveDesktop);
	static void GetWallpaper(CString& strWallpaper, BOOL bActiveDesktop);
};

#endif // BACKGROUND_RANDOMIZER_DESKTOP_INTERFACE