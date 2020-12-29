#ifndef __RD3D11_HELPERS__
#define __RD3D11_HELPERS__

#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#define safe_release(p) if (p) { p->Release(); p = nullptr; }

using DirectX::XMVECTORF32;
using DirectX::XMFLOAT3;

const XMVECTORF32 red{ 1.0f, 0.0f, 0.0f, 1.0f };
const XMVECTORF32 green{ 0.0f, 1.0f, 0.0f, 1.0f };
const XMVECTORF32 blue{ 0.0f, 0.0f, 1.0f, 1.0f };
const XMVECTORF32 magenta{ 1.0f, 0.0f, 1.0f, 1.0f };
const XMVECTORF32 yellow{ 1.0f, 1.0f, 0.0f, 1.0f };

struct Vertex
{
	XMFLOAT3 pos;
	XMVECTORF32 color;
};

struct HandleData
{
	DWORD pid;
	HWND hWnd;
};

HWND FindMainWindow(DWORD dwPID);
BOOL CALLBACK EnumWindowsCallback(HWND hWnd, LPARAM lParam);

#endif // __RD3D11_HELPERS__
