#ifndef __MACROS_H
#define __MACROS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define checkAlloc(var) if(var==NULL) {printf("ERROR: no more memory!\n"); exit(0);}
#define MALLOC(type,var) type *var = (type*)malloc(sizeof(type));
#define CALLOC(type,var) type *var = (type*)calloc(1,sizeof(type)); checkAlloc(var)
#define uint8 uint_fast8_t
#define uint16 uint_fast16_t

#endif
