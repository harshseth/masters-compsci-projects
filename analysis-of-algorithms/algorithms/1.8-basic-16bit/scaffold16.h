/*
 * scaffold32.h
 *
 *  Created on: Mar 28, 2010
 *      Author: deepakkoni
 */

#ifndef SCAFFOLD32_h
#define SCAFFOLD32_h

#include <time.h>

#define WORDBYTES 2 // bytes per word
#define WORDBITS 16 // bits per word

typedef unsigned int int32;
typedef unsigned short int int16;


//mpz import/export parameters
#define NAILS 0 //bit padding, usually 0
#define ORDER -1 //most significant word first, -1 for least first
#define ENDIAN 0 // native endianness

extern void Product16(void *a, void *b, void *c, unsigned int wa,
            unsigned int ba, unsigned int wb, unsigned int bb, unsigned
            int *wc, unsigned int *bc);

extern double mytime (void);

extern char* toBinary(int32 i, char *str);
extern char* toBinary64(int32 i, char *str);
extern int16** Product1_8_Basic(int16** a, int16** b, int16** c, int32 wa, int32 wb);
extern void Karatsuba(int16** a, int16** b, int16** res, int32 dig, int32 wa, int32 wb);
extern void Addition1_4(int16** x, int16** y, int32 wx, int32 wy, int16** sum);
extern void Difference(int16** x, int16** y, int32 wx,int32 wy, int16** diff);
extern int16 Max(int32 a, int32 b);
extern int16 Min(int32 a, int32 b);
extern void clear(int16** x, int32 size);

#endif
