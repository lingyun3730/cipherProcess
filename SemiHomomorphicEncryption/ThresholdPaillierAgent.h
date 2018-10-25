#ifndef Macro_ThresholdPaillierAgent_H
#define Macro_ThresholdPaillierAgent_H 1

#include "PaillierPrivate.h"

class CThresholdPaillierAgent : public CPaillierPrivate
{
public:
	CThresholdPaillierAgent(void);
	~CThresholdPaillierAgent(void);
public:
	int GetDecryptSubKey23(ZZ subK[3], ZZ subKInv[3]);
};

#endif//#ifndef Macro_ThresholdPaillierAgent_H

