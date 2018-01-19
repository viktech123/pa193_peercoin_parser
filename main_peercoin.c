/* Programe: This programe reads the Magic number of peercoin block verifyies it,
reades the size of the block and extracts the block for furhter processing in raw format.

Programmer :

Date : 5th Oct 2017

Ver : 00

Input : Name of the raw block file
output : Verification of magic number, block size and raw block

*********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <malloc.h>
#include "peercoin.h"
#define MAGICNUMBER 0xE6E8E9E5
#define HEADER_LEN 88

//uint32_t block_Verifier(FILE *, uint32_t *, uint32_t *, uint32_t *);

int main(int argc, char *argv[])
{
	FILE *inFileP;
	FILE *outFileP;
	FILE *block1File;
	FILE *block2File;
	char tempBuffer[255];
	char inFileName[255];
	char outFileName[255];
	char flag = 'Y';
	uint32_t blockNum[10];
	uint32_t blockPos[10];
	uint32_t blockLength[10];
	
	uint32_t num_of_Blocks;
	
	
	//printf("\n Enter the raw file name : ");
	//scanf("%s",tempBuffer);
	//strncpy_s(inFileName, 255, tempBuffer, 255);

	inFileP= fopen("blocks.dat", "rb");
	if (inFileP == NULL)
	{
		printf("\n The input file could not be opended \n");
		return -1;
	}
	num_of_Blocks=block_Verifier(inFileP, blockNum, blockPos,blockLength);
	if (num_of_Blocks > 0)
	{
		printf("\nBlock extraction is sucessfull\n");
	}
	else
	{
		printf("\n No block could be extraced");
		return(0);
	}

	for (uint32_t i = 0; i < num_of_Blocks; i++)
	{
		printf("\n Block Number: %d,  Starting position: %d , Block Length: %d", blockNum[i], blockPos[i],blockLength[i]);
		/*strncpy(outFileName, "block", 5);
		strncat(outFileName, (char)i, 2);
		strncat(outFileName, ".dat", 4);
		outFileP = fopen(outFileName, "wb");*/

	}
	// Allocating the memory for block 1 and block 2 based on the values returned form block_Verifier()
	uint8_t *block1;
	uint8_t *block2;
	block1 = (uint8_t *) malloc(sizeof(uint8_t) * blockLength[0]);
	if(block1==NULL)
	{printf("\n memory not allocated");
	 exit(0);
	}
	block2 = (uint8_t *) malloc(sizeof(uint8_t) * blockLength[1]);
	if(block2==NULL)
	{printf("\n memory not allocated");
	 exit(0);
	}

	block1File = fopen("block1.tmp", "w+"); 
	block2File = fopen("block2.tmp", "w+");
	// Reading the block1 and block2 from the file and storing in the heap

	fseek(inFileP, blockPos[0], SEEK_SET);
	fread(block1,blockLength[0],1,inFileP);
	fwrite(block1, 1, blockLength[0], block1File);
	rewind(block1File);

	fseek(inFileP, blockPos[1], SEEK_SET);
	fread(block2, blockLength[1], 1, inFileP);
	fwrite(block2, 1, blockLength[1], block2File);
	rewind(block2File);

	parse_Header(block1File,blockLength[0]); // parsing block1
	//get_transactions(block1);
	uint64_t transNumber;
	transNumber = varint(block1File, HEADER_LEN);
	printf("\n Number of transactions :%lld", transNumber);
	get_Transactions(block1File, (HEADER_LEN + transNumber)); // the 
	get_blocksignature(block1File);
	printf("\n Block2");
	parse_Header(block2File, blockLength[1]); // parsing block2
	// parsing function of blocks to be placed here;
	free(block1);
	free(block2);
	fclose(inFileP);
	fclose(block1File);
	fclose(block2File);
}

// fucntions moved to func_peercoin.c
