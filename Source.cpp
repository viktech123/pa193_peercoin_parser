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
//#include <cstdint.h>
#define MAGICNUMBER 0xE6E8E9E5;

uint16_t block_Verifier(FILE *, uint16_t *, uint16_t *, uint16_t *);
uint16_t main()

{

	FILE *inFileP;
	FILE *outFileP;
	char tempBuffer[255];
	char inFileName[255];
	char outFileName[255];
	char flag = 'Y';
	uint16_t blockNum[10];
	uint16_t blockPos[10];
	uint16_t blockLength[10];
	
	uint16_t num_of_Blocks;
	
	
	printf("\n Enter the raw file name : ");
	scanf("%s",tempBuffer);
	strncpy_s(inFileName, 255, tempBuffer, 255);

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
		printf("\No block could be extraced");
		return(0);
	}

	for (uint16_t i = 0; i < num_of_Blocks; i++)
	{
		printf("\n Block Number: %d,  Starting position: %d", blockNum[i], blockPos[i]);
		/*strncpy(outFileName, "block", 5);
		strncat(outFileName, (char)i, 2);
		strncat(outFileName, ".dat", 4);
		outFileP = fopen(outFileName, "wb");*/

	}
	// Allocating the memory for block 1 and block 2 based on the values returned form block_Verifier()
	uint8_t *block1;
	uint8_t *block2;
	block1 = (uint8_t)malloc(sizeof(uint8_t)*blockLength[0]);
	block2 = (uint8_t)malloc(sizeof(uint8_t)*blockLength[1]);

	// Reading the block1 and block2 from the file and storing in the heap

	fseek(inFileP, blockPos[0], SEEK_SET);
	fread(block1,blockLength[0],1,inFileP);

	fseek(inFileP, blockPos[1], SEEK_SET);
	fread(block2, blockLength[1], 1, inFileP);

	// parsing function of blocks to be placed here;
	free(block1);
	free(block2);
	fclose(inFileP);
}

uint16_t block_Verifier(FILE *inF,uint16_t *bNum, uint16_t *bPos, uint16_t *bLen)
{
	
	uint16_t magicNumber=0; // Vraible to read the magic number from the file
	uint16_t blockLength=0;
	uint16_t blockCount=0; // Variable to store the number of blocks obtained in file
	uint16_t byteRead=0;
	
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
				*(bPos + (blockCount - 1)) = ftell(inF);
				printf("\nThe magic number is found in the file");
				fread(&blockLength,1,4,inF);
				*(bLen + (blockCount - 1)) = blockLength;
				//convLittleEndian(&blockLength);
				printf("\nThe length of the block is : %d", blockLength);
				fseek(inF, blockLength, SEEK_CUR);
			}

		}
	} while (flag == 'Y');
	return (blockCount);
}



uint16_t convLittleEndian(uint16_t *data)
{
	uint16_t num = *data;
	uint16_t b0, b1, b2, b3, b4, b5, b6, b7;
	uint16_t res = 0;

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