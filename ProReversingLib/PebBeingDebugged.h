#pragma once
#include <Windows.h>
#include "ProTechnique.h"

class PebBeingDebugged: public ProTechnique
{
public:
	PebBeingDebugged();
	~PebBeingDebugged();

	virtual bool enable();
	virtual bool disable();

	virtual bool isAntiTechniqueStillWorking();
	virtual const string name();

private:
	BYTE originalData_;
};
