#ifndef __SSHOT_CLEANER_H__
#define __SSHOT_CLEANER_H__

#include "main.h"
#include "hook.h"
#include "log.hpp"

class BMP_T{
public:
	BITMAPFILEHEADER bmfHeader{ 0 };
	BITMAPINFOHEADER bi{ 0 };
	BYTE* bmp{ nullptr };
	DWORD dwBmpSize{ 0 };

	BMP_T();

	BMP_T(int width, int height);

	~BMP_T();
};

struct MonitorCallbackStruct{
	MONITORINFOEXW* targetMonitor;
};

//Required to grab Desktop HDC for correct monitor
BOOL CALLBACK MonitorEnumProcCallback(_In_  HMONITOR hMonitor, _In_  HDC DevC, _In_  LPRECT lprcMonitor, _In_  LPARAM dwData);

using tBitBlt = BOOL(WINAPI*)(HDC hdcDst, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop);
BOOL WINAPI hkBitBlt(HDC hdcDst, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop);

class SShotCleaner{
public:
	bool bInitComplete = false;
	bool bEnabled = false;
	bool bOverlay = false;
	bool* pDrawEnabled = nullptr;
	std::wstring gameWindowTitle, overlayClassName, overlayWindowName;

	class Hook BitBltHook;
	tBitBlt oBitBlt = nullptr;
	tBitBlt BitBltTramp = nullptr;

	SShotCleaner();
	SShotCleaner(std::wstring gameWindowTitle, std::wstring overlayClassName, std::wstring overlayWindowName); //overlays
	SShotCleaner(std::wstring gameWindowTitle, bool* pDrawEnabled); //internal drawing

	std::wstring GetFileName();
	std::wstring GetTimeString();

	//After BitBlt() is called, use GetDIBits to save the screenshot
	void SaveBitBltResult(HDC hdcDst, HDC hdcSrc, int width, int height, std::wstring fileSuffix);

	//Used for testing the screenshot cleaner before deployment
	void SaveTestScreenshot(HWND hWnd);

	bool Init();
	void Enable();
	void Disable();
	void Toggle();

	~SShotCleaner();
};

extern SShotCleaner sshotCleaner;

/*
//GetDIBits might be useful to hook in similar situations
typedef int (WINAPI* tGetDIBits)(HDC hdc, HBITMAP hbm, UINT start, UINT cLines, LPVOID lpvBits, LPBITMAPINFO lpbmi, UINT usage);
tGetDIBits oGetDIBits = nullptr;
tGetDIBits GetDiBitsTramp = nullptr;
*/

#endif // __SSHOT_CLEANER_H__
