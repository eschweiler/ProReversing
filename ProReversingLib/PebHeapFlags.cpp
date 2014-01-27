#include "PebHeapFlags.h"

using namespace System;

PebHeapFlags::PebHeapFlags(void)
	:flagPtr_(getFlagPtr(System::getPebProcessHeap())),
	forceFlagPtr_(getForceFlagPtr(System::getPebProcessHeap()))
{}


PebHeapFlags::~PebHeapFlags(void)
{}




ULONG* PebHeapFlags::getForceFlagPtr(BYTE* heap)
{
	
	#ifdef _WIN64
		if(System::isAtleastVista())
			return (ULONG*) (heap + 0x74);
        else
            return (ULONG*) (heap + 0x18);
	#else
		if (System::isAtleastVista())
			return (ULONG*) (heap + 0x44);
		else
			return (ULONG*) (heap + 0x10);
	#endif

}


ULONG* PebHeapFlags::getFlagPtr(BYTE* heap)
{
	#ifdef _WIN64
		if(System::isAtleastVista())
			return (ULONG*) (heap + 0x70);
        else
		    return (ULONG*) (heap + 0x14);
	#else
		if (System::isAtleastVista())
			return (ULONG*) (heap + 0x40);
		else
			return (ULONG*) (heap + 0x0c);
	#endif

}


bool PebHeapFlags::disable()
{
	*flagPtr_ = originalFlagData_;
	*forceFlagPtr_ = originalForceFlagData_;

	return true;
}


bool PebHeapFlags::enable()
{
	originalFlagData_ = *flagPtr_;
	originalForceFlagData_ = *forceFlagPtr_;

	*flagPtr_ &= HEAP_GROWABLE;
	*forceFlagPtr_ = 0;

	return true;
}


bool PebHeapFlags::isAntiTechniqueStillWorking()
{
	HANDLE heap = HeapCreate(0, 0x10, 0x1000);

	ULONG* flags = getFlagPtr((BYTE*) heap);
	
	bool found = (*flags & (HEAP_GROWABLE)) != 0;

	ULONG* forceFlags = getForceFlagPtr((BYTE*) heap);

	found |= *forceFlags != 0;

	HeapDestroy(heap);

	return found;
}


const string PebHeapFlags::name()
{
	return "PebHeapFlags";
}
