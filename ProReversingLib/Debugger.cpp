#include "Debugger.h"

#include <iostream>

using namespace std;

Debugger::Debugger(void)
{}


Debugger::~Debugger(void)
{}


DWORD Debugger::OnCreateProcess(DEBUG_EVENT& debugEvent)
{
	return DBG_CONTINUE;
}

pair<bool, DWORD> Debugger::OnExitProcess(DEBUG_EVENT& debugEvent)
{
	pair<bool, DWORD> res(false, DBG_CONTINUE);
	return res;
}

DWORD Debugger::OnLoadDll(DEBUG_EVENT& debugEvent)
{
	return DBG_CONTINUE;
}

DWORD Debugger::OnUnloadDll(DEBUG_EVENT& debugEvent)
{
	return DBG_CONTINUE;
}

DWORD Debugger::OnCreateThread(DEBUG_EVENT& debugEvent)
{
	return DBG_CONTINUE;
}

DWORD Debugger::OnExitThread(DEBUG_EVENT& debugEvent)
{
	return DBG_CONTINUE;
}

pair<bool, DWORD> Debugger::OnRip(DEBUG_EVENT& debugEvent)
{
	pair<bool, DWORD> res(false, DBG_CONTINUE);
	return res;
}

DWORD Debugger::OnDebugString(DEBUG_EVENT& debugEvent)
{
	OUTPUT_DEBUG_STRING_INFO& event = debugEvent.u.DebugString;
	size_t length = (event.fUnicode ? 2 : 1 ) * event.nDebugStringLength;
	
	string debugString;
	debugString.resize(length);
	SIZE_T numBytesRead;

	ReadProcessMemory(processInfo_.hProcess, event.lpDebugStringData, const_cast<char*>(debugString.c_str()), length, &numBytesRead);
	
	if (event.fUnicode)
		printf("[debug string] %S", debugString.c_str());
	else
		printf("[debug string] %s", debugString.c_str());

	return DBG_CONTINUE;
}

DWORD Debugger::OnException(DEBUG_EVENT& debugEvent)
{
	EXCEPTION_DEBUG_INFO& event = debugEvent.u.Exception;
	printf("[exception] 0x%08x at address 0x%08x\n", event.ExceptionRecord.ExceptionCode, event.ExceptionRecord.ExceptionAddress);

	if (event.ExceptionRecord.ExceptionCode == STATUS_BREAKPOINT)
	{
		printf("[exception] breakpoint found, continuing execution\n");
		return DBG_CONTINUE;
	}

	return DBG_EXCEPTION_NOT_HANDLED;
}

bool Debugger::debugLoop()
{
	bool done = false;

	while (!done)
	{
		DEBUG_EVENT debugEvent;
		if (!WaitForDebugEvent(&debugEvent, INFINITE))
			throw "Debug loop aborted";

		DWORD continueFlag = DBG_CONTINUE;
		switch (debugEvent.dwDebugEventCode)
		{
		case CREATE_PROCESS_DEBUG_EVENT:
			{
				continueFlag = OnCreateProcess(debugEvent);
				CloseHandle(debugEvent.u.CreateProcessInfo.hFile);
				break;
			}
		case EXIT_PROCESS_DEBUG_EVENT:
			{
				pair<bool, DWORD> res = OnExitProcess(debugEvent);
				done = res.first;
				continueFlag = res.second;
				break;
			}
		case LOAD_DLL_DEBUG_EVENT:
			{
				continueFlag = OnLoadDll(debugEvent);
				CloseHandle(debugEvent.u.LoadDll.hFile);
				break;
			}
		case UNLOAD_DLL_DEBUG_EVENT:
			{
				continueFlag = OnUnloadDll(debugEvent);
				break;
			}
		case CREATE_THREAD_DEBUG_EVENT:
			{
				continueFlag = OnCreateThread(debugEvent);
				break;
			}
		case EXIT_THREAD_DEBUG_EVENT:
			{	
				continueFlag = OnExitThread(debugEvent);
				break;
			}
		case RIP_EVENT:
			{
				pair<bool, DWORD> res = OnRip(debugEvent);
				done = res.first;
				continueFlag = res.second;
				break;
			}
		case OUTPUT_DEBUG_STRING_EVENT:
			{
				continueFlag = OnDebugString(debugEvent);
				break;
			}
		case EXCEPTION_DEBUG_EVENT:
			{
				continueFlag = OnException(debugEvent);
				break;
			}

		default:
			printf("debug event %08x\n", debugEvent.dwDebugEventCode);
			break;
		}

		if (!ContinueDebugEvent(debugEvent.dwProcessId, debugEvent.dwThreadId, continueFlag))
			throw "Error continuing debug event";

	}

	return true;

}

void Debugger::setProcessInfo( const PROCESS_INFORMATION& other )
{
	memcpy(&processInfo_, &other, sizeof PROCESS_INFORMATION);
}

