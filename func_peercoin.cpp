/***************************************************************
Programe : This File Contains fuction called in main for parsing

*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <malloc.h>
#include "peercoin.h"
#include "SHA256.h"

// This function reades the block file and finds out the number of blocks, their length and the staring postionof the blcok
uint32_t block_Verifier(FILE *inF,uint32_t *bNum, uint32_t *bPos, uint32_t *bLen)
{
	
	uint32_t magicNumber=0; // Vraible to read the magic number from the file
	uint32_t blockLength=0;
	uint32_t blockCount=0; // Variable to store the number of blocks obtained in file
	uint32_t byteRead=0;
	
	char flag = 'Y';
	do
	{
		byteRead = fread(&magicNumber, 4, 1, inF);
		//printf("\n Number of byte read %d",byteRead);
		if (byteRead < 1)
		{
			flag = 'N';
			magicNumber = 0;
		}

		{
			if (magicNumber == 0xE5E9E8E6)
			{
				
				blockCount++;
				*(bNum + (blockCount - 1)) = blockCount;
				//*(bPos + (blockCount - 1)) = ftell(inF);
				printf("\n The magic number is found in the file");
				fread(&blockLength,1,4,inF);
				*(bLen + (blockCount - 1)) = blockLength;
				*(bPos + (blockCount - 1)) = ftell(inF);
				//convLittleEndian(&blockLength);
				//printf("\nThe length of the block is : %d", blockLength);
				fseek(inF, blockLength, SEEK_CUR);
			}

		}
	} while (flag == 'Y');
	return (blockCount);
}


// This function converts Bigendian numbers to Littel endian formate
uint32_t conv_LittleEndian(uint32_t *data)
{
	uint32_t num = *data;
	uint32_t b0, b1, b2, b3, b4, b5, b6, b7;
	uint32_t res = 0;

	b0 = (num & 0xf) << 28;
	b1 = (num & 0xf0) << 24;
	b2 = (num & 0xf00) << 20;
	b3 = (num & 0xf000) << 16;
	b4 = (num & 0xf0000) << 12;
	b5 = (num & 0xf00000) << 8;
	b6 = (num & 0xf000000) << 4;
	b7 = (num & 0xf0000000) << 4;

	res = b0 + b1 + b2 + b3 + b4 + b5 + b6 + b7;
	*data = res;
	printf("%d\n", res);
	return 0;
}

void parse_Header(FILE *Pblock, uint32_t blockSize)
{
	uint32_t nVersion;
	unsigned char hashPrevBlock[32];
	unsigned char hashMerkleRoot[32];
	uint32_t Timestamp;
	uint32_t Target_Difficulty;
	uint32_t Nonce;
	
	//fseek(Peerblock, beg, SEEK_SET);     // Moving the file pointer to beg of block

	int v1 = fread(&nVersion, 4, 1, Pblock);
	//memcpy(&nVersion,Pblock,4*sizeof(uint8_t));
	int HP1 = fread(&hashPrevBlock[0], 32, 1, Pblock);
	//int HP1 = fread(hashPrevBlock, 32, 1, Pblock);
	//memcpy(&hashPrevBlock, Pblock+4, 32* sizeof(uint8_t));
	int HM1 = fread(&hashMerkleRoot[0], 32, 1, Pblock);
	//memcpy(&hashMerkleRoot, Pblock+36, 32 * sizeof(uint8_t));
	int T1 = fread(&Timestamp, 4, 1, Pblock);
	//memcpy(&Timestamp, Pblock+68, 4 * sizeof(uint8_t));
	int TD1 = fread(&Target_Difficulty, 4, 1, Pblock);
	//memcpy(&Target_Difficulty, Pblock+72, 4 * sizeof(uint8_t));
	int NC1 = fread(&Nonce, 4, 1, Pblock);
	//memcpy(&Nonce, Pblock+76, 4 * sizeof(uint8_t));
	printf("\n Version Number = %u\n", nVersion);

	printf("\n Hash of Previous Block:\n");
	for (unsigned int i = 0; i<32; ++i)
		printf("%02x", hashPrevBlock[31 - i]);
	printf("\n\n");

	printf("\n Hash of Merkle Root:\n");
	for (unsigned int i = 0; i<32; i++)
		printf("%02x", hashMerkleRoot[31 - i]);
	printf("\n\n");
	printf("\n Timestamp (creation time of the block) = %u\n", Timestamp);
	printf("\n Target Difficulty = %u\n", Target_Difficulty);
	printf("\n Nonce  = %u\n", Nonce);

	//return true;
}

uint64_t varint(FILE *Pblock, uint32_t position)
{
	uint8_t s1;
	uint16_t s2;
	uint32_t s3;
	uint64_t s4;

	int ntrans = fread((char *)&s1, 1, 1, Pblock);

	if (s1 < 0xfd)
	{
		return (uint64_t)s1;
		printf("\n s1=%d", s1);
	}
	else if (s1 == 0xfd)
	{
		fread((char *)&s2, 1, 2, Pblock);
		printf("\n s2=%d", s2);
		return (uint64_t)s2;
	}
	else if (s1 == 0xfe)
	{
		fread((char *)&s3, 1, 4, Pblock);
		return (uint64_t)s3;
		printf("\n s3=%d", s3);
	}
	else if (s1 == 0xff)
	{
		fread((char *)&s4, 1, 8, Pblock);
		return (uint64_t)s4;
		printf("\n s4=%d", s4);

	}
}


void get_Transactions(FILE *Pblock, uint32_t postion)
{
	uint32_t txnversion;
	uint32_t timestamp, lock_time;
	//uint32_t c;
	uint64_t inputp_txn, output_txn;

	uint32_t txn_start = ftell(Pblock);

	uint32_t c = fread(&txnversion, 1, 4, Pblock);
	if (c != 4)
	{
		printf("\n error");
	}
	c = 0;
	printf("\n Transaction version: %u", txnversion);

	c = fread(&timestamp, 1, 4, Pblock);
	if (c != 4)
	{
		printf("\n error");
	}
	c = 0;
	printf("\n Transaction Timestamp : %u", timestamp);

	inputp_txn = varint(Pblock,ftell(Pblock));

	printf("\n Number of Input transactions: %llu\n", inputp_txn);

	for (uint64_t i = 0; i<inputp_txn; i++)
	{
		get_ip_txn(Pblock);
	}
	output_txn = varint(Pblock,ftell(Pblock));

	printf("\n Number of Output transactions: %llu\n", output_txn);
	for (uint64_t i = 0; i<output_txn; i++)
	{
		get_op_txn(Pblock);
	}

	fread((char *)&lock_time, 1, 4, Pblock);

	printf("\n LockTime: %u", lock_time);


	int txn_end = ftell(Pblock);

	unsigned int txn_size = txn_end - txn_start;

	printf("\n Transaction size : %u\n", txn_size);

	fseek(Pblock, txn_start, SEEK_SET);  //Moving the file pointer to start of transaction

	unsigned char* txn;
	unsigned char* txn_hash1, *txn_hash2, *txn_hash = { 0, };

	txn_hash1 = (unsigned char*)malloc(32 * sizeof(unsigned char));
	txn_hash2 = (unsigned char*)malloc(32 * sizeof(unsigned char));
	txn_hash = (unsigned char*)malloc(32 * sizeof(unsigned char));
	txn = (unsigned char*)malloc(txn_size * sizeof(unsigned char));

	fread((char *)&txn[0], 1, txn_size, Pblock);   //transaction content of size txn_size

	computeSHA256(txn, txn_size, txn_hash1);
	computeSHA256(txn_hash1, 32, txn_hash2);

	printf("\n Transaction Hash : \n");
	for (unsigned int i = 0; i < 32; ++i)
		printf("%02x", txn_hash2[31 - i]);
	printf("\n");
	
}

void get_ip_txn(FILE *Pblock)
{
	unsigned char txn_id[32];
	unsigned char *script_Sig;
	uint32_t n, txn_sequence;

	uint64_t scriptSigLength;


	//fread((char *)&txn_id[0], 32, 1, Pblock); // Reading Hash of previous Transaction
	fread(txn_id, 1, 32, Pblock); // by rams
	printf("\n Hash of previous transaction:\n");
	for (unsigned int i = 0; i<32; ++i)
		printf("%2x", txn_id[31 - i]);

	printf("\n");

	//fread((char *)&n, 1, 4, Pblock);
	fread(&n, 4, 1, Pblock); // by RS
	printf("\n N-Index = %u ", n); //n-Index refers to an output in the previous transaction

	scriptSigLength = varint(Pblock,ftell(Pblock)); //length of script ib bytes
	printf("\n Script Signature Length =%llu \n", scriptSigLength);

	script_Sig = (unsigned char*)malloc(scriptSigLength * sizeof(unsigned char));

	//fread(&script_Sig[0], 1, scriptSigLength, Pblock);
	fread(script_Sig, 1, scriptSigLength, Pblock);
	printf("\n Input Script:");
	for (uint64_t i = 0; i<scriptSigLength; ++i)
		printf("%02x", script_Sig[i]); //input script
	printf("\n");

	//fread((char *)&txn_sequence, 1, 4, Pblock); //Always expected to be 0xFFFFFFFF
	fread(&txn_sequence, 4, 1, Pblock);
	printf("\n Transaction sequence =%02x ", txn_sequence);

}

void get_op_txn(FILE *Pblock)
{
	unsigned char *scriptPubKey = { 0, };
	//unsigned char *scriptPubKey;  // By Rams
	uint64_t nvalue;
	uint64_t scriptPubKeyLength;

	fread((char *)&nvalue, 1, 8, Pblock);
	//nvalue = varint(Pblock, ftell(Pblock));
	scriptPubKeyLength = varint(Pblock,ftell(Pblock));
	printf("\n Script Signature Length=%llu ", scriptPubKeyLength);

	scriptPubKey = (unsigned char*)malloc(scriptPubKeyLength * sizeof(unsigned char));
	//fread((char *)&scriptPubKey[0], scriptPubKeyLength, 1, Pblock);
	fread(scriptPubKey, scriptPubKeyLength, 1, Pblock);
	printf("\n Printing the Output Transaction Script");
	for (uint64_t i = 0; i<scriptPubKeyLength; i++)
		printf("%02x", scriptPubKey[i]);
	printf("\n");
	
	printf("\n Value of transaction =%f ", ((nvalue*1.0) / 1000000));
}

void get_blocksignature(FILE *Pblock)
{
	uint64_t SigLength;
	unsigned char *block_Sig;

	SigLength = varint(Pblock,ftell(Pblock));

	printf("\n Signature Length=%llu ", SigLength);

	block_Sig = (unsigned char*)malloc(SigLength * sizeof(unsigned char));

	fread(&block_Sig[0], 1, SigLength, Pblock);

	printf("\n Block Signature :");
	for (uint64_t i = 0; i<SigLength; i++)
		printf("%02x", block_Sig[i]); //Block Signature
	printf("\n");
}