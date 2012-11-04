#pragma once
#include <Windows.h>
#include "ProTechnique.h"


class NtGlobalFlag: public ProTechnique
{
public:
	NtGlobalFlag();
	~NtGlobalFlag();

	virtual bool enable();
	virtual bool disable();

	virtual bool isAntiTechniqueStillWorking();
	virtual const string name();

private:
	ULONG32 originalData_;
	ULONG32* ntGlobalFlagPtr_;

	ULONG32* getNtGlobalFlagPtr();

};
