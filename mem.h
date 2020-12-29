#ifndef __MEM_H__
#define __MEM_H__

#include "main.h"

namespace mem{
	void Patch(BYTE* dst, BYTE* src, unsigned int size);
	void PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess);
	void Nop(BYTE* dst, unsigned int size);
	void NopEx(BYTE* dst, unsigned int size, HANDLE hProcess);
	uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);
	bool SetDebugPrivilege(bool Enable);
	bool IsElevated();
}

bool IsGoodPtr(void* ptr);


#endif // __MEM_H__
