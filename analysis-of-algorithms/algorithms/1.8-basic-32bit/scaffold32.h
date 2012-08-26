/*
 * scaffold32.h
 *
 *  Created on: Mar 28, 2010
 *      Author: deepakkoni
 */

#ifndef SCAFFOLD32_h
#define SCAFFOLD32_h

#include <time.h>

#define WORDBYTES 4 // bytes per word
#define WORDBITS 32 // bits per word

typedef unsigned int int32;
typedef unsigned long long int64;
typedef unsigned short int int16;


//mpz import/export parameters
#define NAILS 0 //bit padding, usually 0
#define ORDER -1 //most significant word first, -1 for least first
#define ENDIAN 0 // native endianness

extern void Product32(void *a, void *b, void *c, unsigned int wa,
            unsigned int ba, unsigned int wb, unsigned int bb, unsigned
            int *wc, unsigned int *bc);

extern double mytime (void);

extern char* toBinary(int32 i, char *str);
extern char* toBinary32(int64 i, char *str);
extern int32** Product1_8_Basic(int32** a, int32** b, int32** c, int32 wa, int32 wb);
extern void Karatsuba(int32** a, int32** b, int32** res, int32 dig, int32 wa, int32 wb);
extern void Addition1_4(int32** x, int32** y, int32 wx, int32 wy, int32** sum);
extern void Difference(int32** x, int32** y, int32 wx,int32 wy, int32** diff);
extern int32 Max(int32 a, int32 b);
extern int32 Min(int32 a, int32 b);
extern void clear(int32** x, int32 size);

#endif
