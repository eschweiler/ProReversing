#pragma once
#include <string>

using namespace std;

class ProTechnique
{
public:
	ProTechnique();
	~ProTechnique();

	virtual bool enable() = 0;
	virtual bool disable() = 0;
	virtual bool isAntiTechniqueStillWorking() = 0;
	virtual const string name() = 0;
};
