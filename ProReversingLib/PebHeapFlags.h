#pragma once
#include "ProTechnique.h"
#include "System.h"

class PebHeapFlags: public ProTechnique
{
public:
	PebHeapFlags();
	~PebHeapFlags();

	virtual bool enable();
	virtual bool disable();

	virtual bool isAntiTechniqueStillWorking();
	virtual const string name();

private:
	BYTE originalData_;
	ULONG* flagPtr_;
	ULONG* forceFlagPtr_;
	ULONG originalFlagData_;
	ULONG originalForceFlagData_;

	ULONG* getFlagPtr(BYTE* heap);
	ULONG* getForceFlagPtr(BYTE* heap);
};
