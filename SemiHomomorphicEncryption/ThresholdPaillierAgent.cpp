#include "ThresholdPaillierAgent.h"

CThresholdPaillierAgent::CThresholdPaillierAgent(void)
{
}

CThresholdPaillierAgent::~CThresholdPaillierAgent(void)
{
}

int CThresholdPaillierAgent::GetDecryptSubKey23(ZZ subK[3], ZZ subKInv[3])
{
	ZZ zzRandForK, zzRandForKInv;

	if(!boolPriKeyOK)
	{
		Paillier_SetErrorMsg("CThresholdPaillierAgent: use GetDecryptSubKey23 before PriKey setup");
		return 0;
	}
	
	RandomBnd(zzRandForK, zzN);
	RandomBnd(zzRandForKInv, zzN);

	add(subK[0], zzK, zzRandForK);
	add(subKInv[0], zzKInv, zzRandForKInv); if(subKInv[0] >= zzN) subKInv[0] -= zzN;
	add(subK[1], subK[0], zzRandForK);
	add(subKInv[1], subKInv[0], zzRandForKInv); if(subKInv[1] >= zzN) subKInv[1] -= zzN;
	add(subK[2], subK[0], subK[1]);
	add(subKInv[2], subKInv[1], zzRandForKInv); if(subKInv[2] >= zzN) subKInv[2] -= zzN;

	return 1;
}