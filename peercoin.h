#include<stdint.h>
#include<stdio.h>
uint32_t block_Verifier(FILE *, uint32_t *, uint32_t *, uint32_t *);
void parse_Header(FILE *Pblock, uint32_t blockSize);
uint64_t varint(FILE *Pblock, uint32_t position);
void get_Transactions(FILE *Pblock, uint32_t postion);
void get_ip_txn(FILE *Pblock);
void get_op_txn(FILE *Pblock);
void get_blocksignature(FILE *Pblock);
//void get_transactions(uint8_t *Pblock);