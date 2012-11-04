#include "PebBeingDebugged.h"
#include "System.h"

PebBeingDebugged::PebBeingDebugged(void)
{
}


PebBeingDebugged::~PebBeingDebugged(void)
{
}

bool PebBeingDebugged::enable()
{
	BYTE* peb = System::getPeb();
	originalData_ = peb[2]; // BeingDebugged flag at offset 2 for all windows versions
	peb[2] = FALSE;

	return true;
}

bool PebBeingDebugged::disable()
{
	BYTE* peb = System::getPeb();
	peb[2] = originalData_;

	return true;
}


bool PebBeingDebugged::isAntiTechniqueStillWorking()
{
	return IsDebuggerPresent() ? true : false;
}


const string PebBeingDebugged::name()
{
	return "PebBeingDebugged";
}
