#pragma once

#include <SDKDDKVer.h>
#include <Windows.h>
#include <utility>
#include <stdexcept>

#include "Process.h"

using namespace std;


class Debugger
{
public:
	Debugger(void);
	~Debugger(void);

	void setProcessInfo(const PROCESS_INFORMATION& other);
	
	bool debugLoop();

private:

	virtual DWORD OnCreateProcess(DEBUG_EVENT& debugEvent);
	virtual pair<bool, DWORD> OnExitProcess(DEBUG_EVENT& debugEvent);
	virtual DWORD OnLoadDll(DEBUG_EVENT& debugEvent);
	virtual DWORD OnUnloadDll(DEBUG_EVENT& debugEvent);
	virtual DWORD OnCreateThread(DEBUG_EVENT& debugEvent);
	virtual DWORD OnExitThread(DEBUG_EVENT& debugEvent);
	virtual pair<bool, DWORD> OnRip(DEBUG_EVENT& debugEvent);
	virtual DWORD OnDebugString(DEBUG_EVENT& debugEvent);
	virtual DWORD OnException(DEBUG_EVENT& debugEvent);

	STARTUPINFO startupInfo_;
	PROCESS_INFORMATION processInfo_;

};

