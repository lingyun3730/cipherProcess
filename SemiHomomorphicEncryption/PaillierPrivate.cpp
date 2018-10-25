#include "PaillierPrivate.h"
#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>

CPaillierPrivate::CPaillierPrivate(void)
{
	boolPriKeyOK = 0;
}

CPaillierPrivate::~CPaillierPrivate(void)
{
}

int CPaillierPrivate::GenerateKeyFromPsw(unsigned char *pswKey, unsigned int pswKeyLen, unsigned int rsaModuleByteLen)
{
	EVP_MD_CTX *mdCtx;
	unsigned char mdValue[EVP_MAX_MD_SIZE];
	unsigned int mdValueLen;
	EVP_CIPHER_CTX *cipherCtx;
	unsigned char *encryptKey;
	unsigned char *IV;
	unsigned char plainText[MAX_RSAMODULE_BYTELEN+EVP_MAX_MD_SIZE];
	int plainTextLen;
	unsigned char cipherText[MAX_RSAMODULE_BYTELEN+EVP_MAX_MD_SIZE+EVP_MAX_BLOCK_LENGTH];
	int cipherTextLen;
	int blockLen;
	int opResult;

	ZZ p, q;

	rsaModuleByteLen &= (~3u);
	if(rsaModuleByteLen < 4) rsaModuleByteLen = 4; else if(rsaModuleByteLen > MAX_RSAMODULE_BYTELEN) rsaModuleByteLen = MAX_RSAMODULE_BYTELEN;

	if((mdCtx = EVP_MD_CTX_create()) == NULL)
	{
		Paillier_SetErrorMsg("openssl: EVP_MD_CTX_new");
		return 0;
	}

	if(EVP_DigestInit_ex(mdCtx, EVP_sha256(), NULL) != 1)
	{
		Paillier_SetErrorMsg("openssl: EVP_DigestInit_ex");
		EVP_MD_CTX_destroy(mdCtx);
		return 0;
	}

	if(EVP_DigestUpdate(mdCtx, "Paillier", 8) != 1)
	{
		Paillier_SetErrorMsg("openssl: EVP_DigestUpdate");
		EVP_MD_CTX_destroy(mdCtx);
		return 0;
	}

	if(EVP_DigestUpdate(mdCtx, pswKey, pswKeyLen) != 1)
	{
		Paillier_SetErrorMsg("openssl: EVP_DigestUpdate");
		EVP_MD_CTX_destroy(mdCtx);
		return 0;
	}

	if(EVP_DigestFinal_ex(mdCtx, mdValue, &mdValueLen) !=1)
	{
		Paillier_SetErrorMsg("openssl: EVP_DigestFinal_ex");
		EVP_MD_CTX_destroy(mdCtx);
		return 0;
	}

	if(mdValueLen != 32)
	{
		Paillier_SetErrorMsg("openssl: EVP_sha256");
		EVP_MD_CTX_destroy(mdCtx);
		return 0;
	}

	if(EVP_CIPHER_key_length(EVP_aes_128_cbc()) != 16)
	{
		Paillier_SetErrorMsg("openssl: EVP_CIPHER_key_length");
		EVP_MD_CTX_destroy(mdCtx);
		return 0;
	}

	if(EVP_CIPHER_iv_length(EVP_aes_128_cbc()) != 16)
	{
		Paillier_SetErrorMsg("openssl: EVP_CIPHER_iv_length");
		EVP_MD_CTX_destroy(mdCtx);
		return 0;
	}

	encryptKey = mdValue;
	IV = mdValue + 16;

	memcpy(plainText, mdValue, mdValueLen);
	plainTextLen = mdValueLen;
	do{
		opResult = EVP_DigestInit_ex(mdCtx, EVP_sha256(), NULL);
		opResult = EVP_DigestUpdate(mdCtx, "Paillier:RSA", 12);
		opResult = EVP_DigestUpdate(mdCtx, plainText, plainTextLen);
		opResult = EVP_DigestFinal_ex(mdCtx, plainText+plainTextLen, &mdValueLen);
		plainTextLen += mdValueLen;
	}while(plainTextLen < (int)rsaModuleByteLen);

	EVP_MD_CTX_destroy(mdCtx);

	if((cipherCtx = EVP_CIPHER_CTX_new()) == NULL)
	{
		Paillier_SetErrorMsg("openssl: EVP_CIPHER_CTX_new");
		return 0;
	}

	if(EVP_EncryptInit_ex(cipherCtx, EVP_aes_128_cbc(), NULL, encryptKey, IV) !=1)
	{
		Paillier_SetErrorMsg("openssl: EVP_EncryptInit_ex");
		EVP_CIPHER_CTX_free(cipherCtx);
		return 0;
	}

	if(EVP_EncryptUpdate(cipherCtx, cipherText, &cipherTextLen, plainText, plainTextLen) != 1)
	{
		Paillier_SetErrorMsg("openssl: EVP_EncryptUpdate");
		EVP_CIPHER_CTX_free(cipherCtx);
		return 0;
	}

	if(EVP_EncryptFinal_ex(cipherCtx, cipherText+cipherTextLen, &blockLen) != 1)
	{
		Paillier_SetErrorMsg("openssl: EVP_EncryptFinal_ex");
		EVP_CIPHER_CTX_free(cipherCtx);
		return 0;
	}

	cipherTextLen += blockLen;
	EVP_CIPHER_CTX_free(cipherCtx);

	cipherText[rsaModuleByteLen-1] |= 0xC0;
	cipherText[0] |= 0x01;

	rsaModuleByteLen >>= 1;

	cipherText[rsaModuleByteLen-1] |= 0xC0;
	cipherText[rsaModuleByteLen] |= 0x01;

	ZZFromBytes(p, cipherText, rsaModuleByteLen);
	ZZFromBytes(q, cipherText+rsaModuleByteLen, rsaModuleByteLen);

	return GenerateKeyAfterPQ(p, q);
}

int CPaillierPrivate::GenerateKeyAfterPQ(ZZ& p, ZZ& q)
{
	ZZ zzTmp;

	boolPriKeyOK = 0;
	boolPubKeyOK = 0;

	NextPrime(zzP, p);
	NextPrime(zzQ, q);

	if(zzQ == zzP)
	{
		conv(zzTmp, "981268946920926854");
		zzTmp += zzQ;
		NextPrime(zzQ, zzTmp);
	}

	zzP -= 1;
	zzQ -= 1;
	GCD(zzTmp, zzP, zzQ);
	div(zzK, zzP, zzTmp);
	zzK *= zzQ;
	zzP += 1;
	zzQ += 1;
	mul(zzN, zzP, zzQ);

	GCD(zzTmp, zzN, zzK);
	while(zzTmp != 1)
	{
		conv(zzTmp, "45894209846716");
		zzTmp += zzQ;
		NextPrime(zzQ, zzTmp);
		if(zzQ == zzP)
		{
			conv(zzTmp, "2633298120926854");
			zzTmp += zzQ;
			NextPrime(zzQ, zzTmp);
		}
		zzP -= 1;
		zzQ -= 1;
		GCD(zzTmp, zzP, zzQ);
		div(zzK, zzP, zzTmp);
		zzK *= zzQ;
		zzP += 1;
		zzQ += 1;
		mul(zzN, zzP, zzQ);
		GCD(zzTmp, zzN, zzK);
	}
	InvMod(zzKInv, zzK, zzN);
#if(UseN2 == 1)
	mul(zzN2, zzN, zzN);
#endif//#if(UseN2 == 1)
	div(zzHalfN, zzN, 2);

	boolPriKeyOK = 1;
	boolPubKeyOK = 1;

	return 1;
}

int CPaillierPrivate::GetPriKey(ZZ& p, ZZ& q)
{
	if(!boolPriKeyOK)
	{
		Paillier_SetErrorMsg("CPaillierPrivate: use GetPriKey before PriKey setup");
		return 0;
	}

	p = zzP;
	q = zzQ;
	return 1;
}

#if(UseN2 == 1)
int CPaillierPrivate::DecryptII(ZZ& m, ZZ& c)
{
	ZZ zzTmp;

	if(!boolPriKeyOK)
	{
		Paillier_SetErrorMsg("CPaillierPrivate: use Decrypt before PriKey setup");
		return 0;
	}

	if((c < 0) || (c >= zzN2))
	{
		Paillier_SetErrorMsg("CPaillierPrivate: decrypt invalid cipher");
		return 0;
	}

	PowerMod(zzTmp, c, zzK, zzN2);
	DivRem(m, zzTmp, zzTmp, zzN);
	if(zzTmp != 1)
	{
		Paillier_SetErrorMsg("CPaillierPrivate: decrypt with invalid cipher");
		return 0;
	}
	MulMod(m, m, zzKInv, zzN);

	return 1;
}
#endif//#if(UseN2 == 1)

int CPaillierPrivate::Decrypt(ZZ& m, ZZ& c0, ZZ& c1)
{
	ZZ zzX0, zzX1;

	if(!boolPriKeyOK)
	{
		Paillier_SetErrorMsg("CPaillierPrivate: use Decrypt2 before PriKey setup");
		return 0;
	}

	if((c0 < 0) || (c0 >= zzN))
	{
		Paillier_SetErrorMsg("CPaillierPrivate: decrypt invalid cipher");
		return 0;
	}

	if((c1 < 0) || (c1 >= zzN))
	{
		Paillier_SetErrorMsg("CPaillierPrivate: decrypt invalid cipher");
		return 0;
	}

	if(HomoScalar(zzX0, zzX1, c0, c1, zzK) != 1) return 0;

	if(zzX0 != 1)
	{
		Paillier_SetErrorMsg("CPaillierPrivate: decrypt invalid cipher");
		return 0;
	}
	MulMod(m, zzX1, zzKInv, zzN);

	return 1;
}
