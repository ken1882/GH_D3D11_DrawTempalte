#ifndef __MAIN_H__
#define __MAIN_H__

// Exclude rarely-used stuff from Windows head
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <ShlObj.h>

/*  To use this exported function of dll, include this header
 *  in your project.
 */

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

DWORD WINAPI MainThread(HINSTANCE);

#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
