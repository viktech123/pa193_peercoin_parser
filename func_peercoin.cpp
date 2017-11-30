#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <malloc.h>
#include "peercoin.h"
#define MAGICNUMBER 0xE6E8E9E5;
//#include "peercoin.h"
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
		printf("\n Number of byte read %d",byteRead);
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
				printf("\nThe magic number is found in the file");
				fread(&blockLength,1,4,inF);
				*(bLen + (blockCount - 1)) = blockLength;
				*(bPos + (blockCount - 1)) = ftell(inF);
				//convLittleEndian(&blockLength);
				printf("\nThe length of the block is : %d", blockLength);
				fseek(inF, blockLength, SEEK_CUR);
			}

		}
	} while (flag == 'Y');
	return (blockCount);
}



uint32_t convLittleEndian(uint32_t *data)
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

}

void check_header(uint8_t *Pblock)
{
	uint32_t nVersion;
	unsigned char hashPrevBlock[32];
	unsigned char hashMerkleRoot[32];
	uint32_t Timestamp;
	uint32_t Target_Difficulty;
	uint32_t Nonce;


	//fseek(Peerblock, beg, SEEK_SET);     // Moving the file pointer to beg of block

	//int v1 = fread(&nVersion, 4, 1, Pblock);
	memcpy(&nVersion,Pblock,4*sizeof(uint8_t));
	//int HP1 = fread(&hashPrevBlock[0], 32, 1, Pblock);
	memcpy(&hashPrevBlock, Pblock+4, 32* sizeof(uint8_t));
	//int HM1 = fread(&hashMerkleRoot[0], 32, 1, Pblock);
	memcpy(&hashMerkleRoot, Pblock+36, 32 * sizeof(uint8_t));
	//int T1 = fread(&Timestamp, 4, 1, Pblock);
	memcpy(&Timestamp, Pblock+68, 4 * sizeof(uint8_t));
	//int TD1 = fread(&Target_Difficulty, 4, 1, Pblock);
	memcpy(&Target_Difficulty, Pblock+72, 4 * sizeof(uint8_t));
	//int NC1 = fread(&Nonce, 4, 1, Pblock);
	memcpy(&Nonce, Pblock+76, 4 * sizeof(uint8_t));
	printf("\n Version  = %u\n", nVersion);

	printf("\nHash previous Block:");
	for (unsigned int i = 0; i<32; ++i)
		printf("%02x", hashPrevBlock[31 - i]);
	printf("\n");

	printf("\nHash Merkle Root:");
	for (unsigned int i = 0; i<32; i++)
		printf("%02x", hashMerkleRoot[31 - i]);
	printf("\n");
	printf("\n Timestamp  = %u\n", Timestamp);
	printf("\n Target Difficulty = %u\n", Target_Difficulty);
	printf("\n Nonce  = %u\n", Nonce);

	//return true;
}


uint64_t varint(uint8_t *Pblock, int shift)
{
	uint8_t s1;
	uint16_t s2;
	uint32_t s3;
	uint64_t s4;

	//fread((char *)&s1, sizeof(s1), 1, Pblock);
	memcpy(&s1,Pblock+shift,1*sizeof(uint8_t));
	if (s1<0xfd)
		return (uint64_t)s1;
	else if (s1 == 0xfd)
	{
		//fread((char *)&s2, sizeof(s2), 1, Pblock);
		//fread((char *)&s2, sizeof(s2), 1, Pblock);
		return (uint64_t)s2;
	}
	else if (s1 == 0xfe)
	{
		//fread((char *)&s2, sizeof(s2), 1, Pblock);
		return (uint64_t)s2;
	}
	else if (s1 == 0xff)
	{
		//fread((char *)&s4, sizeof(s4), 1, Pblock);
		return (uint64_t)s2;

	}
}
void get_transactions(uint8_t *Pblock)
{

	uint32_t txnversion;
	uint32_t timestamp, lock_time;

	uint64_t inputp_txn, output_txn;

	//int txn_start = ftell(Pblock);



	/*int c = fread(&txnversion, 1, 4, Pblock);
	if (c != 4)
	{
		printf("error");
	}*/

	memcpy(&txnversion, Pblock+88,1*sizeof(uint32_t));

	printf("\nversion: %u", txnversion);

	//fread((char *)&timestamp, 4, 1, Pblock);
	memcpy(&txnversion, Pblock + 92, 1 * sizeof(uint32_t));
	printf("\ntimestamp: %u", timestamp);

	//	time_t txn_time=timestamp;

	//        printf("\ntransaction time: %u",timestamp);


	inputp_txn = varint(Pblock,96);

	printf("\nNumber of Input transactions: %llu\n", inputp_txn);

	for (uint64_t i = 0; i<inputp_txn; i++)
	{
		//get_ip_txn(Pblock);
	}

	//output_txn = varint(Pblock);
	printf("\nNumber of Output transactions: %llu\n", output_txn);
	for (uint64_t i = 0; i<output_txn; i++)
	{
		//get_op_txn(Pblock);
	}

	//fread((char *)&lock_time, 1, 4, Pblock);
	printf("\nLockTime: %u", lock_time);


	//int txn_end = ftell(Pblock);

	unsigned int txn_size = 0;// txn_end - txn_start;

	//printf("\nTransaction size : %u\n", txn_size);

	//fseek(Pblock, txn_start, SEEK_SET);  //Moving the file pointer to start of transaction

	unsigned char* txn;
	unsigned char* txn_hash1, *txn_hash2, *txn_hash = { 0, };

	txn_hash1 = (unsigned char*)malloc(32 * sizeof(unsigned char));
	txn_hash2 = (unsigned char*)malloc(32 * sizeof(unsigned char));
	txn_hash = (unsigned char*)malloc(32 * sizeof(unsigned char));
	txn = (unsigned char*)malloc(txn_size * sizeof(unsigned char));

	//fread((char *)&txn[0], txn_size, 1, Pblock);   //reading transaction hash of size txn_size

	//SHA256(txn, txn_size, txn_hash1);
	//SHA256(txn_hash1, 32, txn_hash2);

	printf("\nTransaction Hash : ");
	for (unsigned int i = 0; i<32; ++i)
		printf("%02x", txn_hash2[31 - i]);
	printf("\n");
}

