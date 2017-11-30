#include<stdint.h>
#include<stdio.h>
uint32_t block_Verifier(FILE *, uint32_t *, uint32_t *, uint32_t *);
void check_header(uint8_t *Pblock);
void check_header(uint8_t *Pblock);
void get_transactions(uint8_t *Pblock);
uint64_t varint(uint8_t *Pblock, int shift);