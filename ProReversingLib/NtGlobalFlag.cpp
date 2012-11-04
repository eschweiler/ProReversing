#include "NtGlobalFlag.h"
#include "System.h"

NtGlobalFlag::NtGlobalFlag(void)
	:ntGlobalFlagPtr_(getNtGlobalFlagPtr())
{}


NtGlobalFlag::~NtGlobalFlag(void)
{}


bool NtGlobalFlag::enable()
{
	originalData_ = *ntGlobalFlagPtr_;
	*ntGlobalFlagPtr_ &= ~0x70;

	return true;
}


bool NtGlobalFlag::disable()
{
	*ntGlobalFlagPtr_ = originalData_;

	return true;
}


ULONG32* NtGlobalFlag::getNtGlobalFlagPtr()
{
	BYTE* peb = System::getPeb();
	#ifdef _WIN64
		return(ULONG32*) (peb + 0xbc);
	#else
		// NtGlobalFlag is at offset 0x68 in all 32 bit versions
		return(ULONG32*) (peb + 0x68);
	#endif
}


bool NtGlobalFlag::isAntiTechniqueStillWorking()
{
	return (*ntGlobalFlagPtr_ & 0x70) != 0;
}


const string NtGlobalFlag::name()
{
	return "NtGlobalFlag";
}
