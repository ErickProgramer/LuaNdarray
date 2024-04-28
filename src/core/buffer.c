#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<stdarg.h>
#include<inttypes.h>

#include "error.h"
#include "buffer.h"

void LNBuff_Init(LNBuffer *b){
    b->buff = NULL;
    b->n = 0;
}

void LNBuff_addchar(LNBuffer *b, char chr){
    b->buff = LNMem_realloc(b->buff, b->n+2);
    if(LNError_Ocurred()){
        return;
    }
    b->buff[b->n] = chr;
    b->buff[b->n+1] = '\0';
    b->n++;
}

void LNBuff_addlstring(LNBuffer *b, const char *str, size_t l){
    b->buff = LNMem_realloc(b->buff, b->n + l + 1);
    memcpy(b->buff + b->n, str, l);
    b->n += l;
    b->buff[b->n] = '\0';
    // b->buff = LNMem_realloc(b->buff, b->n + l + 1);
    // if(LNError_Ocurred())
    //     return;
    // memcpy(b->buff + b->n, str, l);
    // b->buff[b->n + l] = '\0';

    // b->n += l;
}

void LNBuff_addstring(LNBuffer *b, const char *str){
    return LNBuff_addlstring(b, str, strlen(str));
}

void LNBuff_addfstring(LNBuffer *b, const char *fmt, ...){
    va_list args;
    va_start(args, fmt);
    int n = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    va_start(args, fmt);
    char *res = malloc(n+1);
    vsnprintf(res, n+1, fmt, args);
    LNBuff_addlstring(b, res, n);
    va_end(args);
}

void LNBuff_addtype(LNBuffer *b, const void *data, const LNTypeDescr *type){
    switch (type->id){
        case LN_INT8:
            LNBuff_addfstring(b, "%" PRId8, *(const int8_t*)data);
            break;
        case LN_INT16:
            LNBuff_addfstring(b, "%" PRId16, *(const int16_t*)data);
            break;
        case LN_INT32:
            LNBuff_addfstring(b, "%" PRId32, *(const int32_t*)data);
            break;
        case LN_INT64:
            LNBuff_addfstring(b, "%" PRId64, *(const int64_t*)data);
            break;
        
        case LN_UINT8:
            LNBuff_addfstring(b, "%" PRIu8, *(const uint8_t*)data);
            break;
        case LN_UINT16:
            LNBuff_addfstring(b, "%" PRIu16, *(const uint16_t*)data);
            break;
        case LN_UINT32:
            LNBuff_addfstring(b, "%" PRIu32, *(const uint32_t*)data);
            break;
        case LN_UINT64:
            LNBuff_addfstring(b, "%" PRIu64, *(const uint64_t*)data);
            break;
        
        case LN_FLOAT32:
            LNBuff_addfstring(b, LNDTYPE_FLOAT32_FMT, *(const float32_t*)data);
            break;
        case LN_FLOAT64:
            LNBuff_addfstring(b, LNDTYPE_FLOAT64_FMT, *(const float64_t*)data);
            break;

        /*
        local r,i=re(c),im(c)
		if i==0 then
			return tostring(r)
		elseif r==0 then
			if i==1 then
				return "i"
			elseif i==-1 then
				return "-i"
			end
			return i.."i"
		elseif i<0 then
			if i==-1 then
				return r.."-i"
			end
			return r..i.."i"
		else
			if i==1 then
				return r.."+i"
			end
			return r.."+"..i.."i"
		end
        */
        case LN_COMPLEX64:{
            float32_t r =((const complex64_t*)data)->realp, i=((const complex64_t*)data)->imagp;
            if(i==0){
                LNBuff_addfstring(b, "%.4g", r);
            }else if(r==0){
                if(i==1){
                    LNBuff_addchar(b, 'i');
                } else if(i==-1){
                    LNBuff_addstring(b, "-i");
                } else{
                    LNBuff_addfstring(b, "%.4gi", i);
                }
            } else if(i<0){
                if(i==-1){
                    LNBuff_addfstring(b, "%.4g-i",r);
                } else{
                    LNBuff_addfstring(b, "%.4g%.4gi",r,i);
                }
            } else{
                if(i==1){
                    LNBuff_addfstring(b, "%.4g+i", r);
                } else{
                    LNBuff_addfstring(b, "%.4g+%.4gi",r,i);
                }
            }
            break;
        }
        case LN_COMPLEX128:{
            float64_t r = ((const complex128_t*)data)->realp, i=((const complex128_t*)data)->imagp;
            if(i==0){
                LNBuff_addfstring(b, "%.4g", r);
            }else if(r==0){
                if(i==1){
                    LNBuff_addchar(b, 'i');
                } else if(i==-1){
                    LNBuff_addstring(b, "-i");
                } else{
                    LNBuff_addfstring(b, "%.4gi", i);
                }
            } else if(i<0){
                if(i==-1){
                    LNBuff_addfstring(b, "%.4g-i",r);
                } else{
                    LNBuff_addfstring(b, "%.4g%.4gi",r,i);
                }
            } else{
                if(i==1){
                    LNBuff_addfstring(b, "%.4g+i", r);
                } else{
                    LNBuff_addfstring(b, "%.4g+%.4gi",r,i);
                }
            }
            break;
        }
        
        case LN_CHAR:
            switch(*(const char*)data){
                case '\n':
                    LNBuff_addstring(b, "'\\n'");
                    break;
                case '\t':
                    LNBuff_addstring(b, "'\\t'");
                    break;
                case '\0':
                    LNBuff_addstring(b, "''");
                    break;
                default:
                    LNBuff_addfstring(b, "'%c'", *(const char*)data);
                    break;
            }
            break;
        case LN_BOOL:
            LNBuff_addstring(b, *((const bool_t*)data) ? "true" : "false");
            break;
    }
}
