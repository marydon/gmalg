#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

#include "sm2.h"
#include "debug.h"

u8 rA[ECC_NUMBITS] = {
	0xD4,0xDE,0x15,0x47,0x4D,0xB7,0x4D,0x06,0x49,0x1C,0x44,0x0D,0x30,0x5E,0x01,0x24,
	0x00,0x99,0x0F,0x3E,0x39,0x0C,0x7E,0x87,0x15,0x3C,0x12,0xDB,0x2E,0xA6,0x0B,0xB3
};

u8 rB[ECC_NUMBITS]= {
	0x7E,0x07,0x12,0x48,0x14,0xB3,0x09,0x48,0x91,0x25,0xEA,0xED,0x10,0x11,0x13,0x16,
	0x4E,0xBF,0x0F,0x34,0x58,0xC5,0xBD,0x88,0x33,0x5C,0x1F,0x9D,0x59,0x62,0x43,0xD6};

ecc_point pubkeyA;
ecc_point pubkeyB;
u8 prikeyA[ECC_NUMBITS];
u8 prikeyB[ECC_NUMBITS];

u8 IDa[16] = {
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38
};

u8 IDb[16] = {
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38
};

ecc_point RA;
ecc_point RB;
ecc_point V;

u8 K[64];
u32 klen = 16;

u8 ZA[32];
u8 ZB[32];
u8 SB[32];
u8 SA[32];

int main(int argc, char **argv)
{
	int rc;

	sm2_make_keypair(prikeyA, &pubkeyA);
	sm2_make_keypair(prikeyB, &pubkeyB);

	sm3_z(IDa, 16, &pubkeyA, ZA);
	sm3_z(IDb, 16, &pubkeyB, ZB);

	//a
	rc = ECC_KeyEx_Init_I(rA, &RA);
	if (rc)
		printf(" key ex err\n");
	else
		printf(" key ex ok\n");
	///b
	rc = ECC_KeyEx_Re_I(rB, prikeyB, &RA, &pubkeyA, ZA, ZB, K, klen, &RB, &V, SB);
	if (rc)
		printf(" key ex err\n");
	else
		printf(" key ex ok\n");
	//a
	rc = ECC_KeyEx_Init_II(rA, prikeyA, &RA, &RB, &pubkeyB, ZA, ZB, SB, K, klen, SA);
	if (rc)
		printf(" key ex err\n");
	else
		printf(" key ex ok\n");
	//bb
	rc = ECC_KeyEx_Re_II(&V, &RA, &RB, ZA, ZB, SA);
	if (rc)
		printf(" key ex err\n");
	else
		printf(" key ex ok\n");

	return 0;
}
