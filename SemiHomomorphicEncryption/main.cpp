#include "SemiHomoFun.h"
#include <time.h>

using namespace std;
using namespace NTL;

#if 1
#define PAUSE_FOR_AWHILE
int main(void)
{
	unsigned char pswKey[] = "SemiHomoTest";
	unsigned int pswKeyLen = sizeof(pswKey);
	ZZ m_a, m_b, m_f, c0_a, c1_a, c0_b, c1_b, c0_f, c1_f;
	clock_t begin_clock, end_clock;

	string istr;
	string ostr;
	SemiHomoPlain plain;
	SemiHomoCipher cipher;

	unsigned char FloatStringList[] = "+1e2-1.e+2+.23e-2.3e-2.24-2+2.-2.3";
	unsigned char *PrnStr = FloatStringList;
	long p = 0;
	vector<SemiHomoPlain> plainVec;
	vector<SemiHomoPlainPair> plainPairVec;
	vector<SemiHomoCipher> cipherVec;
	vector<SemiHomoCipherPair> cipherPairVec;
	SemiHomoPlain  pa, pb, pm, pc, pr;
	SemiHomoCipher ca, cb, cm, cc, cr;
	ZZ scalar;
	long shiftNum;

	SemiHomoPlainPair plainPair;
	SemiHomoCipherPair cipherPair;

	double d = -4.e+1;

	int i = 0;
	char floatStr[128], *pStr = floatStr;
	struct{const char *zstr; long prec;}ZPA[30]=
	{
		{"0", 0},
		{"53", 1},
		{"353", 1},
		{"530", 1},
		{"3530", 1},
		{"5300", 1},
		{"35300", 1},
		{"53000", 1},
		{"353000", 1},
		{"4353000", 1},
		{"5300", 3},
		{"300", 4},
		{"300", 5},
		{"5300", 4},
		{"5300", 5},
		{"35300", 4},
		{"435300", 4},
		{"5435300", 4},
		{"65435300", 4},
		{"5300", 5},
		{"35300", 5},
		{"435300", 5},
		{"5435300", 5},
		{"65435300", 5},
		{"5300", 6},
		{"35300", 6},
		{"435300", 6},
		{"5435300", 6},
		{"65435300", 6},
		{"-65435300", 6}
	};

	istr = "87Ac630:ab45620:000";
	if(!String2SemiHomoCipher(cipher, istr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, cipher))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "String2SemiHomoCipher/SemiHomoCipher2String:" << endl << istr << endl << ostr << endl << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	istr = " [ ] ";
	if(!String2SemiHomoCipherVector(cipherVec, istr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipherVector2String(ostr, cipherVec))
	{
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cipherVec.clear();
	cout << "String2SemiHomoCipherVector/SemiHomoCipherVector2String:" << endl << istr << endl << ostr << endl << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	istr = " [debf:76abcd50:03, 78cbde3:567530:0, 7887b3:523a530:2, ] ";
	if(!String2SemiHomoCipherVector(cipherVec, istr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipherVector2String(ostr, cipherVec))
	{
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cipherVec.clear();
	cout << "String2SemiHomoCipherVector/SemiHomoCipherVector2String:" << endl << istr << endl << ostr << endl << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	istr = " [debf:76abcd50:03, 78cbde3:567530:0, 7887b3:523a530:2] ";
	if(!String2SemiHomoCipherVector(cipherVec, istr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipherVector2String(ostr, cipherVec))
	{
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cipherVec.clear();
	cout << "String2SemiHomoCipherVector/SemiHomoCipherVector2String:" << endl << istr << endl << ostr << endl << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	istr = " ( 87Ace0:ab4530:000, 87Ac30f:ab453:20) ";
	if(!String2SemiHomoCipherPair(cipherPair, istr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipherPair2String(ostr, cipherPair))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "String2SemiHomoCipherPair/SemiHomoCipherPair2String:" << endl << istr << endl << ostr << endl << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	istr = " [] ";
	if(!String2SemiHomoCipherPairVector(cipherPairVec, istr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipherPairVector2String(ostr, cipherPairVec))
	{
		cipherPairVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cipherPairVec.clear();
	cout << "String2SemiHomoCipherPairVector/SemiHomoCipherPairVector2String:" << endl << istr << endl << ostr << endl << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	istr = " [(debf:76abcd50:03, 78cbde3:567530:0), (7887b3:523a530:2, cdee3:554530:2), (12de3:567:6, 3cb3:530:1), ] ";
	if(!String2SemiHomoCipherPairVector(cipherPairVec, istr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipherPairVector2String(ostr, cipherPairVec))
	{
		cipherPairVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cipherPairVec.clear();
	cout << "String2SemiHomoCipherPairVector/SemiHomoCipherPairVector2String:" << endl << istr << endl << ostr << endl << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	istr = " [(debf:76abcd50:03, 78cbde3:567530:0), (7887b3:523a530:2, cdee3:554530:2), (12de3:567:6, 3cb3:530:1)] ";
	if(!String2SemiHomoCipherPairVector(cipherPairVec, istr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipherPairVector2String(ostr, cipherPairVec))
	{
		cipherPairVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cipherPairVec.clear();
	cout << "String2SemiHomoCipherPairVector/SemiHomoCipherPairVector2String:" << endl << istr << endl << ostr << endl << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	istr = "12.688393484343009128932748349e-1";
	if(!String2SemiHomoPlain(plain, istr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, plain))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "String2SemiHomoPlain/SemiHomoPlain2String:" << endl << istr << endl << ostr << endl << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	istr = "[ ]";
	if(!String2SemiHomoPlainVector(plainVec, istr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlainVector2String(ostr, plainVec))
	{
		plainVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	plainVec.clear();
	cout << "String2SemiHomoPlainVector/SemiHomoPlainVector2String:" << endl << istr << endl << ostr << endl << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	istr = "[-0.255, -1.031251, 1.25e-1, 3.e2, 7, ]";
	if(!String2SemiHomoPlainVector(plainVec, istr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlainVector2String(ostr, plainVec))
	{
		plainVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	plainVec.clear();
	cout << "String2SemiHomoPlainVector/SemiHomoPlainVector2String:" << endl << istr << endl << ostr << endl << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	istr = "[-0.255, -1.031251, 1.25e-1, 3.e2, 7]";
	if(!String2SemiHomoPlainVector(plainVec, istr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlainVector2String(ostr, plainVec))
	{
		plainVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	plainVec.clear();
	cout << "String2SemiHomoPlainVector/SemiHomoPlainVector2String:" << endl << istr << endl << ostr << endl << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	istr = "(10.2557897, -9.22257745)";
	if(!String2SemiHomoPlainPair(plainPair, istr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlainPair2String(ostr, plainPair))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "String2SemiHomoPlainPair/SemiHomoPlainPair2String:" << endl << istr << endl << ostr << endl << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	istr = " [ ] ";
	if(!String2SemiHomoPlainPairVector(plainPairVec, istr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlainPairVector2String(ostr, plainPairVec))
	{
		plainPairVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	plainPairVec.clear();
	cout << "String2SemiHomoPlainPairVector/SemiHomoPlainPairVector2String:" << endl << istr << endl << ostr << endl << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	istr = " [(-0.255, -1.031251), (1.25e-1, 3.e2), (7, 5), ] ";
	if(!String2SemiHomoPlainPairVector(plainPairVec, istr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlainPairVector2String(ostr, plainPairVec))
	{
		plainPairVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	plainPairVec.clear();
	cout << "String2SemiHomoPlainPairVector/SemiHomoPlainPairVector2String:" << endl << istr << endl << ostr << endl << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	istr = " [(-0.255, -1.031251), (1.25e-1, 3.e2), (7, 5)] ";
	if(!String2SemiHomoPlainPairVector(plainPairVec, istr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlainPairVector2String(ostr, plainPairVec))
	{
		plainPairVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	plainPairVec.clear();
	cout << "String2SemiHomoPlainPairVector/SemiHomoPlainPairVector2String:" << endl << istr << endl << ostr << endl << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	begin_clock = clock();
	if(!SetAgentKeyFromPsw(pswKey, pswKeyLen))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	end_clock = clock();
	cout << "SetAgentKeyFromPsw: OK " << end_clock - begin_clock  << endl << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	istr = "-3.1415944";
	cout << "EncryptSemiHomoPlain/DecryptSemiHomoCipher:" << endl;
	cout << " plain [i]: " << istr << endl;
	if(!String2SemiHomoPlain(plain, istr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!EncryptSemiHomoPlain(cipher, plain))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, cipher))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher [o]: " << ostr << endl;
	if(!DecryptSemiHomoCipher(plain, cipher))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, plain))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}	
	cout << " plain [o]: " << ostr << endl << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	istr = "[-3.14, 4.5, 0.1e-2, 2.3e2]";
	cout << "EncryptSemiHomoPlainVector/DecryptSemiHomoCipherVector:" << endl;
	cout << " plain vector [i]: " << istr << endl;
	if(!String2SemiHomoPlainVector(plainVec, istr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!EncryptSemiHomoPlainVector(cipherVec, plainVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipherVector2String(ostr, cipherVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher vector [o]: " << ostr << endl;
	if(!DecryptSemiHomoCipherVector(plainVec, cipherVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlainVector2String(ostr, plainVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	plainVec.clear();
	cipherVec.clear();
	cout << " plain vector [o]: " << ostr << endl << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	istr = "(-3.1415944, 786e-1)";
	cout << "EncryptSemiHomoPlainPair/DecryptSemiHomoCipherPair:" << endl;
	cout << " plain pair [i]: " << istr << endl;
	if(!String2SemiHomoPlainPair(plainPair, istr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!EncryptSemiHomoPlainPair(cipherPair, plainPair))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipherPair2String(ostr, cipherPair))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher pair [o]: " << ostr << endl;
	if(!DecryptSemiHomoCipherPair(plainPair, cipherPair))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlainPair2String(ostr, plainPair))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain pair [o]: " << ostr << endl << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	istr = "[(-2.1, 2.7), (0.1e1, 1.3e2)]";
	cout << "EncryptSemiHomoPlainPairVector/DecryptSemiHomoCipherPairVector:" << endl;
	cout << " plain pair vector [i]: " << istr << endl;
	if(!String2SemiHomoPlainPairVector(plainPairVec, istr))
	{
		plainPairVec.clear();
		cipherPairVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!EncryptSemiHomoPlainPairVector(cipherPairVec, plainPairVec))
	{
		plainPairVec.clear();
		cipherPairVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipherPairVector2String(ostr, cipherPairVec))
	{
		plainPairVec.clear();
		cipherPairVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher pair vector [o]: " << ostr << endl;
	if(!DecryptSemiHomoCipherPairVector(plainPairVec, cipherPairVec))
	{
		plainPairVec.clear();
		cipherPairVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlainPairVector2String(ostr, plainPairVec))
	{
		plainPairVec.clear();
		cipherPairVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	plainPairVec.clear();
	cipherPairVec.clear();
	cout << " plain pair vector [o]: " << ostr << endl << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	cout << "AddSemiHomoPlain/NegSemiHomoPlain/LeftShiftSemiHomoPlain/ScalarSemiHomoPlain/MulSemiHomoPlain/InvSemiHomoPlain/DivSemiHomoPlain:" << endl;
	cout << "AddSemiHomoCipher/NegSemiHomoCipher/LeftShiftSemiHomoCipher/ScalarSemiHomoCipher/MulSemiHomoCipher/InvSemiHomoCipher/DivSemiHomoCipher:" << endl;
	istr = "-2.1"; cout << " plain a [i]: " << istr << endl;
	if(!String2SemiHomoPlain(pa, istr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	istr = "4.1"; cout << " plain b [i]: " << istr << endl;
	if(!String2SemiHomoPlain(pb, istr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	istr = "6.6"; cout << " plain c [i]: " << istr << endl;
	conv(scalar, "173"); cout << " scalar [i]: " << scalar << endl;
	shiftNum = 9; cout << " shiftNum [i]: " << shiftNum << endl;
	if(!String2SemiHomoPlain(pc, istr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!EncryptSemiHomoPlain(ca, pa))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, ca))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher a [o]: " << ostr << endl;
	if(!EncryptSemiHomoPlain(cb, pb))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, cb))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher b [o]: " << ostr << endl;
	if(!EncryptSemiHomoPlain(cc, pc))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, cc))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher c [o]: " << ostr << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE
	//add
	if(!AddSemiHomoPlain(pr, pa, pb))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain a+b [o]: " << ostr << endl;
	if(!AddSemiHomoCipher(cr, ca, cb))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, cr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher a+b [o]: " << ostr << endl;
	if(!DecryptSemiHomoCipher(pr, cr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain a+b [t]: " << ostr << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE
	//neg
	if(!NegSemiHomoPlain(pr, pa))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain -a [o]: " << ostr << endl;
	if(!NegSemiHomoCipher(cr, ca))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, cr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher -a [o]: " << ostr << endl;
	if(!DecryptSemiHomoCipher(pr, cr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain -a [t]: " << ostr << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE
	//left shift
	if(!LeftShiftSemiHomoPlain(pr, pa, shiftNum))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain a<< [o]: " << ostr << endl;
	if(!LeftShiftSemiHomoCipher(cr, ca, shiftNum))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, cr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher a<< [o]: " << ostr << endl;
	if(!DecryptSemiHomoCipher(pr, cr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain a<< [t]: " << ostr << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE
	//scalar
	if(!ScalarSemiHomoPlain(pr, pa, scalar))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain a* [o]: " << ostr << endl;
	if(!ScalarSemiHomoCipher(cr, ca, scalar))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, cr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher a* [o]: " << ostr << endl;
	if(!DecryptSemiHomoCipher(pr, cr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain a* [t]: " << ostr << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE
	//mul
	if(!MulSemiHomoPlain(pm, pa, pb))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pm))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain a*b [o]: " << ostr << endl;
	if(!MulSemiHomoCipher(cm, ca, cb))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, cm))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher a*b [o]: " << ostr << endl;
	if(!DecryptSemiHomoCipher(pr, cm))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain a*b [t]: " << ostr << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE
	//a*b+c
	if(!AddSemiHomoPlain(pr, pm, pc))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain a*b+c [o]: " << ostr << endl;
	if(!AddSemiHomoCipher(cr, cm, cc))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, cr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher a*b+c [o]: " << ostr << endl;
	if(!DecryptSemiHomoCipher(pr, cr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain a*b+c [t]: " << ostr << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE
	//c+a*b
	if(!AddSemiHomoPlain(pr, pc, pm))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain c+a*b [o]: " << ostr << endl;
	if(!AddSemiHomoCipher(cr, cc, cm))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, cr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher c+a*b [o]: " << ostr << endl;
	if(!DecryptSemiHomoCipher(pr, cr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain c+a*b [t]: " << ostr << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE
	//inv
	if(!InvSemiHomoPlain(pr, pa))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain 1/a [o]: " << ostr << endl;
	if(!InvSemiHomoCipher(cr, ca))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, cr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher 1/a [o]: " << ostr << endl;
	if(!DecryptSemiHomoCipher(pr, cr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain 1/a [t]: " << ostr << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE
	//div
	if(!DivSemiHomoPlain(pr, pa, pb))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain a/b [o]: " << ostr << endl;
	if(!DivSemiHomoCipher(cr, ca, cb))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, cr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher a/b [o]: " << ostr << endl;
	if(!DecryptSemiHomoCipher(pr, cr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain a/b [t]: " << ostr << endl;
	cout << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	cout << "SumSemiHomoPlainVector/AverageSemiHomoPlainVector/VarianceSemiHomoPlainVector:" << endl;
	cout << "SumSemiHomoCipherVector/AverageSemiHomoCipherVector/VarianceSemiHomoCipherVector:" << endl;
	istr = "[]"; cout << " plain vector [i]: " << istr << endl;
	if(!String2SemiHomoPlainVector(plainVec, istr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!EncryptSemiHomoPlainVector(cipherVec, plainVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipherVector2String(ostr, cipherVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher vector [o]: " << ostr << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE
	//sum
	if(!SumSemiHomoPlainVector(pr, plainVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain sum [o]: " << ostr << endl;
	if(!SumSemiHomoCipherVector(cr, cipherVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, cr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher sum [o]: " << ostr << endl;
	if(!DecryptSemiHomoCipher(pr, cr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain sum [t]: " << ostr << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE
	//average
	if(!AverageSemiHomoPlainVector(pr, plainVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain average [o]: " << ostr << endl;
	if(!AverageSemiHomoCipherVector(cr, cipherVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, cr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher average [o]: " << ostr << endl;
	if(!DecryptSemiHomoCipher(pr, cr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain average [t]: " << ostr << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE
	//variance
	if(!VarianceSemiHomoPlainVector(pr, plainVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain variance [o]: " << ostr << endl;
	if(!VarianceSemiHomoCipherVector(cr, cipherVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, cr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher variance [o]: " << ostr << endl;
	if(!DecryptSemiHomoCipher(pr, cr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain sum [t]: " << ostr << endl;
	cout << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	cout << "SumSemiHomoPlainVector/AverageSemiHomoPlainVector/VarianceSemiHomoPlainVector:" << endl;
	cout << "SumSemiHomoCipherVector/AverageSemiHomoCipherVector/VarianceSemiHomoCipherVector:" << endl;
	istr = "[2.15]"; cout << " plain vector [i]: " << istr << endl;
	if(!String2SemiHomoPlainVector(plainVec, istr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!EncryptSemiHomoPlainVector(cipherVec, plainVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipherVector2String(ostr, cipherVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher vector [o]: " << ostr << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE
	//sum
	if(!SumSemiHomoPlainVector(pr, plainVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain sum [o]: " << ostr << endl;
	if(!SumSemiHomoCipherVector(cr, cipherVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, cr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher sum [o]: " << ostr << endl;
	if(!DecryptSemiHomoCipher(pr, cr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain sum [t]: " << ostr << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE
	//average
	if(!AverageSemiHomoPlainVector(pr, plainVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain average [o]: " << ostr << endl;
	if(!AverageSemiHomoCipherVector(cr, cipherVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, cr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher average [o]: " << ostr << endl;
	if(!DecryptSemiHomoCipher(pr, cr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain average [t]: " << ostr << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE
	//variance
	if(!VarianceSemiHomoPlainVector(pr, plainVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain variance [o]: " << ostr << endl;
	if(!VarianceSemiHomoCipherVector(cr, cipherVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, cr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher variance [o]: " << ostr << endl;
	if(!DecryptSemiHomoCipher(pr, cr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain sum [t]: " << ostr << endl;
	cout << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	cout << "SumSemiHomoPlainVector/AverageSemiHomoPlainVector/VarianceSemiHomoPlainVector:" << endl;
	cout << "SumSemiHomoCipherVector/AverageSemiHomoCipherVector/VarianceSemiHomoCipherVector:" << endl;
	istr = "[-3.2, 5.4, ]"; cout << " plain vector [i]: " << istr << endl;
	if(!String2SemiHomoPlainVector(plainVec, istr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!EncryptSemiHomoPlainVector(cipherVec, plainVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipherVector2String(ostr, cipherVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher vector [o]: " << ostr << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE
	//sum
	if(!SumSemiHomoPlainVector(pr, plainVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain sum [o]: " << ostr << endl;
	if(!SumSemiHomoCipherVector(cr, cipherVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, cr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher sum [o]: " << ostr << endl;
	if(!DecryptSemiHomoCipher(pr, cr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain sum [t]: " << ostr << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE
	//average
	if(!AverageSemiHomoPlainVector(pr, plainVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain average [o]: " << ostr << endl;
	if(!AverageSemiHomoCipherVector(cr, cipherVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, cr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher average [o]: " << ostr << endl;
	if(!DecryptSemiHomoCipher(pr, cr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain average [t]: " << ostr << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE
	//variance
	if(!VarianceSemiHomoPlainVector(pr, plainVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain variance [o]: " << ostr << endl;
	if(!VarianceSemiHomoCipherVector(cr, cipherVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, cr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher variance [o]: " << ostr << endl;
	if(!DecryptSemiHomoCipher(pr, cr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain sum [t]: " << ostr << endl;
	cout << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	cout << "SumSemiHomoPlainVector/AverageSemiHomoPlainVector/VarianceSemiHomoPlainVector:" << endl;
	cout << "SumSemiHomoCipherVector/AverageSemiHomoCipherVector/VarianceSemiHomoCipherVector:" << endl;
	istr = "[-13.0, 2.3, 7.8, 9.9]"; cout << " plain vector [i]: " << istr << endl;
	if(!String2SemiHomoPlainVector(plainVec, istr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!EncryptSemiHomoPlainVector(cipherVec, plainVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipherVector2String(ostr, cipherVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher vector [o]: " << ostr << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE
	//sum
	if(!SumSemiHomoPlainVector(pr, plainVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain sum [o]: " << ostr << endl;
	if(!SumSemiHomoCipherVector(cr, cipherVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, cr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher sum [o]: " << ostr << endl;
	if(!DecryptSemiHomoCipher(pr, cr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain sum [t]: " << ostr << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE
	//average
	if(!AverageSemiHomoPlainVector(pr, plainVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain average [o]: " << ostr << endl;
	if(!AverageSemiHomoCipherVector(cr, cipherVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, cr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher average [o]: " << ostr << endl;
	if(!DecryptSemiHomoCipher(pr, cr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain average [t]: " << ostr << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE
	//variance
	if(!VarianceSemiHomoPlainVector(pr, plainVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain variance [o]: " << ostr << endl;
	if(!VarianceSemiHomoCipherVector(cr, cipherVec))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipher2String(ostr, cr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher variance [o]: " << ostr << endl;
	if(!DecryptSemiHomoCipher(pr, cr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain sum [t]: " << ostr << endl;
	cout << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	cout << "LinearRegressionSemiHomoPlainPairVector/LinearRegressionSemiHomoCipherPairVector:" << endl;
	istr = "[(-13.0, 2.25), (7.5, 9.75), (7.25, 7.5), (3.5, 8.75), (12.25, 7.75), (65.0, 87.25)]"; cout << " plain pair vector [i]: " << istr << endl;
	if(!String2SemiHomoPlainPairVector(plainPairVec, istr))
	{
		plainPairVec.clear();
		cipherPairVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!EncryptSemiHomoPlainPairVector(cipherPairVec, plainPairVec))
	{
		plainPairVec.clear();
		cipherPairVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoCipherPairVector2String(ostr, cipherPairVec))
	{
		plainPairVec.clear();
		cipherPairVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher pair vector [o]: " << ostr << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE
	if(!LinearRegressionSemiHomoPlainPairVector(pb, pc, plainPairVec))
	{
		plainPairVec.clear();
		cipherPairVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << " plain linear regression [o]: ";
	if(!SemiHomoPlain2String(ostr, pb))
	{
		plainPairVec.clear();
		cipherPairVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << ostr << ", ";
	if(!SemiHomoPlain2String(ostr, pc))
	{
		plainPairVec.clear();
		cipherPairVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << ostr << endl;
	if(!LinearRegressionSemiHomoCipherPairVector(cb, cc, cipherPairVec))
	{
		plainPairVec.clear();
		cipherPairVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "cipher linear regression [o]: " << endl;
	if(!SemiHomoCipher2String(ostr, cb))
	{
		plainPairVec.clear();
		cipherPairVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << ostr << endl;
	if(!SemiHomoCipher2String(ostr, cc))
	{
		plainPairVec.clear();
		cipherPairVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << ostr << endl;
	cout << " plain linear regression [t]: ";
	if(!DecryptSemiHomoCipher(pr, cb))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << ostr << ", ";
	if(!DecryptSemiHomoCipher(pr, cc))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!SemiHomoPlain2String(ostr, pr))
	{
		plainVec.clear();
		cipherVec.clear();
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << ostr << endl;
	cout << endl;
#ifdef PAUSE_FOR_AWHILE
	getchar();
#endif//#ifdef PAUSE_FOR_AWHILE

	cout << "CLOCKS_PER_SEC = " << CLOCKS_PER_SEC << endl;
	conv(m_a, "123456789");
	begin_clock = clock();
	if(!EncryptRawData(c0_a, c1_a, m_a))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!DecryptRawData(m_b, c0_a, c1_a))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	end_clock = clock();
	cout << "EncryptRawData/DecryptRawData: " << ((m_a == m_b) ? "OK " : "FAIL ") << end_clock - begin_clock << endl << endl;

	conv(m_b, "65349809");
	if(!EncryptRawData(c0_b, c1_b, m_b))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	begin_clock = clock();
	if(!HomoAddOnEncryptedRawData(c0_f, c1_f, c0_a, c1_a, c0_b, c1_b))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	end_clock = clock();
	if(!DecryptRawData(m_f, c0_f, c1_f))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "HomoAddOnEncryptedRawData: " << (((m_a + m_b) == m_f) ? "OK " : "FAIL ") << end_clock - begin_clock << endl << endl;

	begin_clock = clock();
	if(!HomoNegOnEncryptedRawData(c0_b, c1_b, c0_a, c1_a))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	end_clock = clock();
	if(!HomoAddOnEncryptedRawData(c0_f, c1_f, c0_a, c1_a, c0_b, c1_b))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!DecryptRawData(m_f, c0_f, c1_f))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "HomoNegOnEncryptedRawData: " << ((m_f == 0) ? "OK " : "FAIL ") << end_clock - begin_clock << endl << endl;

	conv(m_b, "561234989");
	begin_clock = clock();
	if(!HomoScalarOnEncryptedRawData(c0_f, c1_f, c0_a, c1_a, m_b))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	end_clock = clock();
	if(!DecryptRawData(m_f, c0_f, c1_f))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "HomoScalarOnEncryptedRawData: " << (((m_a * m_b) == m_f) ? "OK " : "FAIL ") << end_clock - begin_clock << endl << endl;

	if(!EncryptRawData(c0_b, c1_b, m_b))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	begin_clock = clock();
	if(!HomoMulOnEncryptedRawData(c0_f, c1_f, c0_a, c1_a, c0_b, c1_b))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	end_clock = clock();
	if(!DecryptRawData(m_f, c0_f, c1_f))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "HomoMulOnEncryptedRawData: " << (((m_a * m_b) == m_f) ? "OK " : "FAIL ") << end_clock - begin_clock << endl << endl;

	begin_clock = clock();
	if(!HomoInvOnEncryptedRawData(c0_b, c1_b, c0_a, c1_a))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	end_clock = clock();
	if(!HomoMulOnEncryptedRawData(c0_f, c1_f, c0_a, c1_a, c0_b, c1_b))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!DecryptRawData(m_f, c0_f, c1_f))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "HomoInvOnEncryptedRawData: " << ((m_f == 1) ? "OK " : "FAIL ") << end_clock - begin_clock << endl << endl;

	if(!EncryptRawData(c0_b, c1_b, m_b))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	begin_clock = clock();
	if(!HomoDivOnEncryptedRawData(c0_f, c1_f, c0_a, c1_a, c0_b, c1_b))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	end_clock = clock();
	if(!HomoMulOnEncryptedRawData(c0_a, c1_a, c0_f, c1_f, c0_b, c1_b))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	if(!DecryptRawData(m_f, c0_a, c1_a))
	{
		cout << SemiHomoGetLastError() << endl;
		getchar();
		return 1;
	}
	cout << "HomoDivOnEncryptedRawData: " << ((m_f == m_a) ? "OK " : "FAIL ") << end_clock - begin_clock << endl << endl;

	getchar();
	return 0;
}
#else
#include <NTL/ZZ.h>
#include <openssl/evp.h>

#include "PaillierPrivate.h"

#define CIPHER_BUFSIZE 1024
#define PLAIN_BUFSIZE  1024
#define PSW_BUFSIZE    256
#define TRY_NUM 1
int main(void)
{
	ZZ a, b, c;
	unsigned char pswKey[PSW_BUFSIZE] = {0x56, 0x98, 0xEF};
	int pswKeyLen = 3;
	EVP_MD_CTX *mdCtx;
	unsigned char mdValue[EVP_MAX_MD_SIZE];
	unsigned int mdValueLen;
	EVP_CIPHER_CTX *cipherCtx;
	unsigned char *encryptKey;//[EVP_MAX_KEY_LENGTH];
	int encryptKeyLen;
	unsigned char *IV;//[EVP_MAX_IV_LENGTH];
	unsigned char plainText[PLAIN_BUFSIZE] = {0x12, 0x78, 0xAB};
	int plainTextLen = 3;
	unsigned char cipherText[CIPHER_BUFSIZE];
	int cipherTextLen;
	unsigned char checkText[PLAIN_BUFSIZE];
	int checkTextLen;
	int blockLen;
	int opResult;
	CPaillierPrivate mPaillierPri;
	ZZ zzM, zzC, zzX0, zzX1, zzN, zzY0, zzY1, zzZ0, zzZ1, zzR;
	clock_t dwTickBegin,dwTickEnd;
	int i;

	mPaillierPri.GenerateKeyFromPsw((unsigned char*)"Hello", 5);

	conv(zzR, "9812688873098271698274092873615263894");
	mPaillierPri.Encrypt(zzX0, zzX1, zzR);
	conv(zzM, "6730918276452847652539857639201925672");
	mPaillierPri.Encrypt(zzY0, zzY1, zzM);
	zzR += zzM;
	mPaillierPri.HomoAdd(zzZ0, zzZ1, zzX0, zzX1, zzY0, zzY1);
	mPaillierPri.Decrypt(zzM, zzZ0, zzZ1);
	cout << zzR << endl << zzM << endl << "check HomoAdd fin." <<endl;

	mPaillierPri.GetPubKey(zzN);
	conv(zzY0, "89736728765");
	zzZ0 += zzY0;
	mPaillierPri.HomoNeg(zzX0, zzX1, zzZ0, zzZ1);
	mPaillierPri.HomoAdd(zzY0, zzY1, zzX0, zzX1, zzZ0, zzZ1);
	cout << zzY0 << endl << zzY1 << endl << "check HomoInv fin." <<endl;

	conv(zzM, "981268898699345709812345544646946920926854");
	cout << zzM << endl;
#if(UseN2 == 1)
	dwTickBegin = clock();
	for(i=0; i<TRY_NUM; i++)
	{
		mPaillierPri.EncryptII(zzC, zzM, zzM);
	}
	dwTickEnd = clock();
	cout << dwTickEnd - dwTickBegin << endl;
#endif//#if(UseN2 == 1)
	dwTickBegin = clock();
	for(i=0; i<TRY_NUM; i++)
	{
		mPaillierPri.Encrypt(zzX0, zzX1, zzM, zzM);
	}
	dwTickEnd = clock();
	cout << dwTickEnd - dwTickBegin << endl;

	mPaillierPri.GetPubKey(zzN);
	cout << zzX0 + (zzX1*zzN) << endl;
#if(UseN2 == 1)
	cout << zzC << endl;
	dwTickBegin = clock();
	for(i=0; i<TRY_NUM; i++)
	{
		mPaillierPri.DecryptII(zzM, zzC);
	}
	dwTickEnd = clock();
	cout << dwTickEnd - dwTickBegin << endl;
	cout << zzM << endl;
	DivRem(zzX1, zzX0, zzC, zzN);
#endif//#if(UseN2 == 1)	
	dwTickBegin = clock();
	for(i=0; i<TRY_NUM; i++)
	{
		mPaillierPri.Decrypt(zzM, zzX0, zzX1);
	}
	dwTickEnd = clock();
	cout << dwTickEnd - dwTickBegin << endl;
	cout << zzM << endl;

	cout << "check encrypt/decrypt OK" << endl;

	opResult = EVP_MD_size(EVP_sha256());
	opResult = EVP_CIPHER_key_length(EVP_aes_128_cbc());
	opResult = EVP_CIPHER_iv_length(EVP_aes_128_cbc());

	mdCtx = EVP_MD_CTX_create();
	opResult = EVP_DigestInit_ex(mdCtx, EVP_sha256(), NULL);
	opResult = EVP_DigestUpdate(mdCtx, pswKey, pswKeyLen);
	opResult = EVP_DigestFinal_ex(mdCtx, mdValue, &mdValueLen);
	EVP_MD_CTX_destroy(mdCtx);

	encryptKey = mdValue;
	encryptKeyLen = EVP_CIPHER_key_length(EVP_aes_128_cbc());
	IV = mdValue + encryptKeyLen;

	cipherCtx = EVP_CIPHER_CTX_new();
	opResult = EVP_EncryptInit_ex(cipherCtx, EVP_aes_128_cbc(), NULL, encryptKey, IV);
	cipherTextLen = 0;
	opResult = EVP_EncryptUpdate(cipherCtx, cipherText+cipherTextLen, &blockLen, plainText, plainTextLen);
	cipherTextLen += blockLen;
	opResult = EVP_EncryptFinal_ex(cipherCtx, cipherText+cipherTextLen, &blockLen);
	cipherTextLen += blockLen;
	EVP_CIPHER_CTX_free(cipherCtx);

	cipherCtx = EVP_CIPHER_CTX_new();
	opResult = EVP_DecryptInit_ex(cipherCtx, EVP_aes_128_cbc(), NULL, encryptKey, IV);
	checkTextLen = 0;
	opResult = EVP_DecryptUpdate(cipherCtx, checkText+checkTextLen, &blockLen, cipherText, cipherTextLen);
	checkTextLen += blockLen;
	opResult = EVP_DecryptFinal_ex(cipherCtx, checkText+checkTextLen, &blockLen);
	checkTextLen += blockLen;
	EVP_CIPHER_CTX_free(cipherCtx);

	cout << "Input any key to terminate." << endl;
	getchar();
	return 0;
}
#endif
