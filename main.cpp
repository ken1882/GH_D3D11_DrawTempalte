#include "main.h"
#include "rd3d11.h"
#include "sshot_cleaner.h"

rD3D11 rd11;

DWORD WINAPI MainThread(HINSTANCE hModule){

    return 0;
}


HRESULT __stdcall hkPresent(IDXGISwapChain* pThis, UINT SyncInterval, UINT Flags){
	if (!rd11.pDevice || rd11.pSwapchain != pThis){
		rd11.InitD3DDraw(pThis);
	}

	//enable this to test or debug viewport
	rd11.TestRender();

	return rd11.oPresentTramp(pThis, SyncInterval, Flags);
}

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){
    switch (fdwReason){
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hinstDLL);
            CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hinstDLL, 0, nullptr);
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}
