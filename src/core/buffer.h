
#ifndef LUANDARRAY_CORE_SRC_BUFFER_H_
#define LUANDARRAY_CORE_SRC_BUFFER_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"utils.h"
#include"arraydtype.h"

typedef struct LNBuffer{
    char *buff;
    size_t n;
} LNBuffer;

#define LNBuff_ADDR(b) ((b)->buff)
#define LNBuff_LEN(b) ((b)->n)

void LNBuff_Init(LNBuffer *b);
void LNBuff_addchar(LNBuffer *b, char chr);
void LNBuff_addlstring(LNBuffer *b, const char *str, size_t l);
void LNBuff_addstring(LNBuffer *b, const char *str);
void LNBuff_addfstring(LNBuffer *b, const char *fmt, ...);
void LNBuff_addtype(LNBuffer *b, const void *data, const LNTypeDescr *dtype);

#define LNBuff_alloc() (LNBuffer*)LNMem_alloc(sizeof(LNBuffer))

#endif