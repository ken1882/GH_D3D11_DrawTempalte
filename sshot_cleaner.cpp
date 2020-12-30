#include "main.h"
#include "util.h"
#include "sshot_cleaner.h"

BOOL WINAPI hkBitBlt(HDC hdcDst, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop){
	LOG(L"BitBlt called");

	bool bBitBltResult;

	// Capture uncleaned screenshot data
	bBitBltResult = sshotCleaner.BitBltTramp(hdcDst, x, y, cx, cy, hdcSrc, x1, y1, rop);

	// Take the uncleaned screenshot data we just took, run it through DIBits and save to disk
	sshotCleaner.SaveBitBltResult(hdcDst, hdcSrc, cx, cy, L"_regular");

	if(sshotCleaner.bOverlay){
		auto hOverlayWindow = FindWindowW(sshotCleaner.overlayClassName.c_str(), sshotCleaner.overlayWindowName.c_str());

		if(hOverlayWindow){
			auto swResult = ShowWindow(hOverlayWindow, SW_HIDE); //requires admin

			// Capture the clean screenshot data
			bBitBltResult = sshotCleaner.BitBltTramp(hdcDst, x, y, cx, cy, hdcSrc, x1, y1, rop);

			// Take the cleaned screenshot data we just took, run it through DIBits and save to disk
			sshotCleaner.SaveBitBltResult(hdcDst, hdcSrc, cx, cy, L"_cleaned");

			swResult = ShowWindow(hOverlayWindow, SW_SHOW);

			LOG(L"Overlay Found - Screenshot Cleaned");
		}
		else{
			//if no overlay, we already sent the regular screenshot
			LOG(L"Overlay not found - Uncleaned(possibly dirty) Screenshot Sent");
		}
	}

	//if not an overlay and using internal drawing
	else if(!sshotCleaner.bOverlay && *sshotCleaner.pDrawEnabled == true){
		// Disable internal drawing
		*sshotCleaner.pDrawEnabled = false;
		Sleep(10);//

		// Capture the clean screenshot data
		bBitBltResult = sshotCleaner.BitBltTramp(hdcDst, x, y, cx, cy, hdcSrc, x1, y1, rop);

		// Take the cleaned screenshot data we just took, run it through DIBits and save to disk
		sshotCleaner.SaveBitBltResult(hdcDst, hdcSrc, cx, cy, L"_cleaned");

		// Re-enable internal drawing
		*sshotCleaner.pDrawEnabled = true;
		LOG(L"Internal Drawing - Screenshot Cleaned");
	}

	else LOG(L"Drawing not detected, Uncleaned(possibly dirty) Screenshot Sent");

	return bBitBltResult;
}

SShotCleaner::SShotCleaner(){}

SShotCleaner::SShotCleaner(std::wstring gameWindowTitle, std::wstring overlayClassName, std::wstring overlayWindowName){
	this->bOverlay = true;
	this->gameWindowTitle = gameWindowTitle;
	this->overlayClassName = overlayClassName;
	this->overlayWindowName = overlayWindowName;
}

SShotCleaner::SShotCleaner(std::wstring gameWindowTitle, bool* pDrawEnabled){
	this->bOverlay = false;
	this->pDrawEnabled = pDrawEnabled;
	this->gameWindowTitle = gameWindowTitle;
}

bool SShotCleaner::Init(){
	if(bOverlay){
		//if process is elevated, or elevation succeeded
		if(mem::SetDebugPrivilege(true)){
			LOG(L"Process Is elevated");

#ifdef _WIN64
			BitBltHook = Hook("BitBlt", "Gdi32.dll", (BYTE*)hkBitBlt, (uintptr_t*)&BitBltTramp, 15);
#else
			BitBltHook = Hook("BitBlt", "Gdi32.dll", (BYTE*)hkBitBlt, (uintptr_t*)&BitBltTramp, 5);
#endif

			bInitComplete = true;
			LOG(L"SShotCleaner Init Completed");
			return true;
		}

		//if elevation failed
		else{
			LOG(L"Process is not elevated, game must be run as admin to clean overlay screenshots");
			LOG(L"Elevation failed, your cleanshots will not be clean if this message appears");
			LOG(L"SShotCleaner Init Failed, SShotCleaner Disabled");
			bInitComplete = false;
			return false;
		}
	}

	//if internal drawing, elevation not required
#ifdef _WIN64
	BitBltHook = Hook("BitBlt", "Gdi32.dll", (BYTE*)hkBitBlt, (uintptr_t*)&BitBltTramp, 15);
#else
	BitBltHook = Hook("BitBlt", "Gdi32.dll", (BYTE*)hkBitBlt, (uintptr_t*)&BitBltTramp, 5);
#endif

	bInitComplete = true;
	LOG(L"SShotCleaner Init Completed");

	return true;
}

void SShotCleaner::Enable(){
	BitBltHook.Enable();
	//sync Hook & SShotcleaner to have same trampoline pointers
	BitBltTramp = (tBitBlt)*BitBltHook.pTrampoline;
	bEnabled = true;
	LOG(L"SShotCleaner Enabled");
}

void SShotCleaner::Disable(){
	BitBltHook.Disable();
	bEnabled = false;
	LOG(L"SShotCleaner Disabled");
}

void SShotCleaner::Toggle(){
	if(bEnabled){ Disable(); }
	else{ Enable(); }
}

std::wstring SShotCleaner::GetTimeString(){
	auto now = std::chrono::system_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
	auto now_c = std::chrono::system_clock::to_time_t(now);
	std::tm now_tm;
	localtime_s(&now_tm, &now_c);
	std::wstringstream wss;

	wss << std::put_time(&now_tm, L"%mm-%dd-%Hh-%Mm-%Ss");
	wss << '-' << std::setfill(L'0') << std::setw(3) << ms.count() << L"ms";

	return wss.str();
}

std::wstring SShotCleaner::GetFileName(){
	wchar_t* path = nullptr;
	SHGetKnownFolderPath(FOLDERID_Desktop, 0, 0, &path);
	if(GetLastError() == 1008){ SetLastError(0); } //get rid of meaningless error

	std::wstring wpath(std::wstring(path) + L"\\screenshots\\");

	CoTaskMemFree(path);
	CreateDirectoryW(wpath.c_str(), NULL);
	auto timetext = GetTimeString();

	return wpath + timetext + L".bmp";
}

void SShotCleaner::SaveBitBltResult(HDC hdcDst, HDC hdcSrc, int width, int height, std::wstring fileSuffix){
	// Setup bitmap structure
	auto hbitmapsrc = CreateCompatibleBitmap(hdcDst, width, height);
	HBITMAP hbitmapdst = (HBITMAP)SelectObject(hdcDst, hbitmapsrc);
	BMP_T bmp_t(width, height);

	//GetDIBits() = Convert screenshot data to bitmap structure
	GetDIBits(hdcSrc, hbitmapdst, 0, (UINT)height, bmp_t.bmp, (BITMAPINFO*)&bmp_t.bi, DIB_RGB_COLORS);

	//Begin Saving File to Disk
	auto path = GetFileName();
	path.insert(path.find(L".bmp"), fileSuffix);

	auto hFile = CreateFileW(path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD dwBytesWritten = 0;
	WriteFile(hFile, (void*)&bmp_t.bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (void*)&bmp_t.bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
	WriteFile(hFile, (void*)bmp_t.bmp, bmp_t.dwBmpSize, &dwBytesWritten, NULL);

	CloseHandle(hFile);
}

BOOL CALLBACK MonitorEnumProcCallback(_In_  HMONITOR hMonitor, _In_  HDC DevC, _In_  LPRECT lprcMonitor, _In_  LPARAM dwData){
	// for each monitor, compare the device name of the monitor with the
	// monitor name of the monitor which hosts our game window

	MonitorCallbackStruct& data = *(MonitorCallbackStruct*)dwData;

	MONITORINFOEXW monInfo;
	monInfo.cbSize = sizeof(monInfo);
	GetMonitorInfo(hMonitor, &monInfo);

	if(!wcscmp(monInfo.szDevice, data.targetMonitor->szDevice)){
		auto hdcSrc = DevC;

		auto width = monInfo.rcMonitor.right - monInfo.rcMonitor.left;
		auto height = monInfo.rcMonitor.bottom - monInfo.rcMonitor.top;

		//device found

		auto hdcDst = CreateCompatibleDC(hdcSrc);
		auto hbitmap = CreateCompatibleBitmap(hdcSrc, width, height);

		SelectObject(hdcDst, hbitmap);

		//We are saving a new test screenshot, to test our hook
		//This call, will trigger our hook, our hook will save 1 cleaned screenshot and 1 dirty screenshot
		auto bResult = BitBlt(hdcDst, 0, 0, width, height, hdcSrc, monInfo.rcMonitor.left, monInfo.rcMonitor.top, SRCCOPY);//RCCOPY | CAPTUREBBLT

		//At this point our cleaned screenshot was saved in the hook
		if(!sshotCleaner.bEnabled){
			sshotCleaner.SaveBitBltResult(hdcDst, hdcSrc, width, height, L"_regular_test");
			LOG(L"SShotCleaner was Disabled, test screenshot saved as is");
		}
	}

	return TRUE;
}

void SShotCleaner::SaveTestScreenshot(HWND hWnd){
	// === Get DC for correct monitor for game window, ensures overlays are captured ===
	auto hMon = MonitorFromWindow(FindWindowW(NULL, sshotCleaner.gameWindowTitle.c_str()), MONITOR_DEFAULTTONEAREST);
	MONITORINFOEXW monInfo;
	monInfo.cbSize = sizeof(monInfo);
	GetMonitorInfoW(hMon, &monInfo);

	MonitorCallbackStruct monCallBackStruct;
	monCallBackStruct.targetMonitor = &monInfo;
	EnumDisplayMonitors(GetDC(NULL), NULL, MonitorEnumProcCallback, (LPARAM)&monCallBackStruct);
}

SShotCleaner::~SShotCleaner(){
	if(bEnabled){ Disable(); }
}

BMP_T::BMP_T(){}

BMP_T::BMP_T(int width, int height){
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = width;
	bi.biHeight = height;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	dwBmpSize = ((width * bi.biBitCount + 31) / 32) * 4 * height;

	// Add the size of the headers to the size of the bitmap to get the total file size
	bmfHeader.bfSize = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	//Offset to where the actual bitmap bits start.
	bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

	//bfType must always be BM for Bitmaps
	bmfHeader.bfType = 0x4D42; //"BM"

	if(bmp){ delete[] bmp; }

	bmp = new BYTE[dwBmpSize];
}

BMP_T::~BMP_T(){
	delete[] bmp;
}

/*
//GetDIBits might be useful to hook in similar situations

tGetDIBits oGetDIBits = nullptr;
tGetDIBits GetDIBitsTramp = nullptr;

int hkGetDIBits(HDC hdc, HBITMAP hbm, UINT start, UINT cLines, LPVOID lpvBits, LPBITMAPINFO lpbmi, UINT usage){
	LOG(L"GetDIBits");

	HWND targetWindow = FindWindow(L"classname", L"windowname");

	if(targetWindow){
		BOOL s1 = ShowWindow(targetWindow, SW_HIDE);
		BOOL result = GetDIBitsTramp(hdc, hbm, start, cLines, lpvBits, lpbmi, usage);
		BOOL s2 = ShowWindow(targetWindow, SW_SHOW);
		return result;
	}

	else return GetDIBitsTramp(hdc, hbm, start, cLines, lpvBits, lpbmi, usage);
}
*/
