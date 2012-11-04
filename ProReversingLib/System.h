#pragma once
#ifndef SYSTEM_H
#define SYSTEM_H

#include <Windows.h>
#include <exception>


using namespace std;

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

namespace System
{
	extern "C" BYTE* getPeb();

	static bool isAtleastVista()
	{
		static bool isAtleastVista;
		static bool isSet;
		if (isSet)
			return isAtleastVista;

		OSVERSIONINFO versionInfo = {0};
		versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

		GetVersionEx(&versionInfo);

		isAtleastVista = versionInfo.dwMajorVersion >= 6;
		isSet = true;

		return isAtleastVista;
	}

	static bool isWow64()
	{
		static LPFN_ISWOW64PROCESS isWow64Process;
		static BOOL isWow64;

		if (isWow64Process)
			return isWow64 == TRUE;

		isWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(GetModuleHandle("kernel32"),"IsWow64Process");

		isWow64 = FALSE;

		if (isWow64Process == NULL)	return false;

		if (!isWow64Process(GetCurrentProcess(),&isWow64))	return false;

		return isWow64 == TRUE;
	}

	static BYTE* getPebProcessHeap()
	{
		static BYTE* pebProcessHeap;
		if (pebProcessHeap)
			return pebProcessHeap;

		BYTE* peb = System::getPeb();
		# ifdef _WIN64
			if (!System::isAtleastVista())
				throw exception("unsupported");
			// vista_x64
			pebProcessHeap = (BYTE*) (*(void**) (peb + 0x30));
		#else
			// valid for vista_x86, xp_x86
			pebProcessHeap = (BYTE*) (*(void**) (peb + 0x18));
		#endif

		return pebProcessHeap;
	}
};


/*

constructor selects which os, which environment
os >= vista?
environment: 32 bits within 32 bit os, 32 bits within 64 bit os
winxp.32 (winxp.64)
winvista.32 winvista.64

*/

#endif