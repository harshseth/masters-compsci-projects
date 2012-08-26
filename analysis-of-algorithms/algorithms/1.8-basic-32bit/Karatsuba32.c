/*
 ============================================================================
 Name        : Karatsuba32.c
 Author      : Deepak Konidena
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

/* test framework code
   ./mainX [bitsize of operands] [number of tests] [base to print]
     [truncation]
   defaults are 3200, 100, 0, and 0

   0 base means no printing.  2 base is binary, 10 base is decimal, 16
   is hex.  1 base is an error.  printing is of the randomly generated
   operands and the multiplication results from the code and GMP.  A
   summary of the run is always printed.

   If truncation is 1, then
   Half of the time, both operands are roughly the given bitsize
   (randomly having leading bitz be zero can make them slightly
   smaller.)  The other half of the time , one operand or the other has
   a random bitlength, thus usually being notably shorter, down to 0.
   Note that GMP claims the wordsize of 0 is 0.
   */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <gmp.h>
#include <unistd.h>
#include "scaffold32.h"

int main(int argc, char **argv) {


    mpz_t x; mpz_init(x);
    mpz_t y; mpz_init(y);

    mpz_t xsize_z; mpz_init(xsize_z);
    mpz_t ysize_z; mpz_init(ysize_z);

    gmp_randstate_t randstate; gmp_randinit_default(randstate);
    gmp_randseed_ui(randstate, time(NULL));

    srand(time(NULL));

    int number_of_bits = 3200;
    int xbits, ybits;
    int number_of_tests = 100;
    int printing=0;
    int truncation=0;
    int j;

    if(argc >=2)
      number_of_bits = atoi(argv[1]);
    if(argc >=3)
      number_of_tests = atoi(argv[2]);
    if(argc >=4)
      printing=atoi(argv[3]);
    if(argc >=5)
      truncation=atoi(argv[4]);

    double start, finish, elapsed, elapsed_gmp = 0;
    int num_correct = 0, num_wrong = 0;


    char *x_str = (char *) malloc((1 + number_of_bits)*sizeof(char));
    char *y_str = (char *) malloc((1 + number_of_bits)*sizeof(char));
    char *z_str = (char *) malloc((1 + 2*number_of_bits)*sizeof(char));
    char *res_str = (char *) malloc((1 + 2*number_of_bits)*sizeof(char));

     void *x_address;
     void *y_address;

 //    printf("\nSize of size_t is %d bytes", sizeof(size_t));
 //    printf("\nSize of char is %d bytes", sizeof(char));


     size_t *x_size = malloc(sizeof(size_t));
     size_t *y_size = malloc(sizeof(size_t));
     size_t *z_size = malloc(sizeof(size_t));
     size_t x_bits, y_bits;
     size_t *z_bits = malloc(sizeof(size_t));


    for(j = 0; j < number_of_tests; j++){

      xbits = number_of_bits;
      ybits = number_of_bits;

      if (truncation){

    	  if (rand() %2) {
    		  ;
    	  }
    	  else if (rand() % 2) {
    		  ybits = rand() % number_of_bits;
    	  }
    	  else {
    		  xbits = rand() % number_of_bits;
    	  }
     }

      mpz_ui_pow_ui(xsize_z, 2, xbits);
      mpz_ui_pow_ui(ysize_z, 2, ybits);
      mpz_urandomm(x, randstate, xsize_z);
      mpz_urandomm(y, randstate, ysize_z);

      if (printing){
    	  printf("\nx %s\ny %s\n",mpz_get_str(x_str,printing,x),mpz_get_str(y_str,printing,y));
      }

      x_address = mpz_export(NULL, x_size, ORDER, WORDBYTES, ENDIAN, NAILS, x);
      y_address = mpz_export(NULL, y_size, ORDER, WORDBYTES, ENDIAN, NAILS, y);



      void *z_address = malloc((*x_size + *y_size + 2)*WORDBYTES);

      x_bits=mynbits(x);
      y_bits=mynbits(y);

   //   printf("\nValues of x_bits and y_bits are %d, %d", x_bits, y_bits);

      start = mytime();
      Product32(x_address, y_address, z_address, *x_size, x_bits,*y_size, y_bits, z_size, z_bits);
      finish = mytime();
      elapsed += finish-start;

      mpz_t z;
      mpz_init(z);

      mpz_import(z, *z_size, ORDER, WORDBYTES, ENDIAN, NAILS, z_address);
      if (printing)
        printf("\ncode x*y %s\n", mpz_get_str(z_str,printing,z));

      mpz_t result;
      mpz_init(result);
      start = mytime();
      mpz_mul(result,x,y);
      finish = mytime();
      elapsed_gmp += finish-start;
      if (printing)
        printf("\nGMP  x*y %s\n", mpz_get_str(res_str,printing,result));

      if(mpz_cmp(result,z) == 0)
        num_correct ++;
      else
        num_wrong ++;

      mpz_clear(z);
      mpz_clear(result);
      free(x_address);
      free(y_address);
      free(z_address);
    }

    printf("============================================================\n");
    printf("\n\n");
    printf("Number of bits per number: %d\n", number_of_bits);
    printf("Number of multiplications done: %d\n", number_of_tests);
    printf("Number of correct multiplication: %d\n", num_correct);
    printf("Number of wrong multiplication: %d\n", num_wrong);
    printf("Total time elapsed (code):  %lf seconds\n", ((double) (elapsed)));
    printf("Total time elapsed (GMP):  %lf seconds\n", ((double)(elapsed_gmp)));
    printf("Total time ratio (code/GMP):  %lf \n",elapsed/elapsed_gmp);
    printf("\n\n");
    printf("============================================================\n");
    return 0;
}
