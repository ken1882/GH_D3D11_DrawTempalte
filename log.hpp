#ifndef __LOG_H__
#define __LOG_H__

#include "main.h"

#ifdef _DEBUG
#define LOG(text) Log.Log(text)

class Logger{
public:

	FILE* f = nullptr;
	std::wofstream oFile;

	Logger(){
		AllocConsole();
		freopen_s(&f, "CONOUT$", "a", stdout);
		std::wcout << std::wstring(100, '\n');
		oFile = std::wofstream(GetLogFilePath());
	}

	void Log(std::wstring str){
		std::wstring time = GetTimeString();
		std::wcout << time << L" - " << str << std::endl;
		oFile << time << L" - " << str << std::endl;
	}

	std::wstring GetLogFilePath(){
		//Get Desktop folder
		wchar_t* wc_path = nullptr;
		SHGetKnownFolderPath(FOLDERID_Desktop, 0, 0, &wc_path);
		if (GetLastError() == 1008) SetLastError(0); //get rid of meaningless error
		std::filesystem::path path(wc_path);
		CoTaskMemFree(wc_path);

		//Get executable name
		char szExePath[MAX_PATH];
		GetModuleFileNameA(GetModuleHandle(NULL), szExePath, MAX_PATH);
		std::filesystem::path exePath(szExePath);

		path /= exePath.filename().replace_extension("") += L".log";

		return path.c_str();
	}

	std::wstring GetTimeString(){
		auto now = std::chrono::system_clock::now();
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
		std::time_t now_c = std::chrono::system_clock::to_time_t(now);
		std::tm now_tm;
		localtime_s(&now_tm, &now_c);
		std::wstringstream wss;
		wss << std::put_time(&now_tm, L"%mm-%dd-%Hh-%Mm-%Ss");
		wss << '-' << std::setfill(L'0') << std::setw(3) << ms.count() << L"ms";
		return wss.str();
	}

	~Logger(){
		if (f) fclose(f);
		oFile.close();
		FreeConsole();
	}
};

static Logger Log;

#else
#define LOG(...)
#endif

#endif // __LOG_H__
