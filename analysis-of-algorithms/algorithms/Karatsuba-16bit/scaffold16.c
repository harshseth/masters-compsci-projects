/*
 * scaffold32.c
 *
 *  Created on: Mar 28, 2010
 *      Author: deepakkoni
 */

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include "scaffold16.h"
#include <math.h>
#include <string.h>
#include <limits.h>


int16 Max(int32 a, int32 b){

	if(a > b)
		return a;
	else if(a < b)
		return b;
	else
		return a;
}

void clear(int16** x, int32 size){
	int32 i = 0;
	for(i = 0; i < size; i++)
		*x[i] = 0;
}

int16 Min(int32 a, int32 b){

	if(a < b)
		return a;
	else if(b < a)
		return b;
	else
		return a;
}

char* toBinary(int32 i, char *str){

	int32 num = i;
	strcpy(str, "");

	while(num){
		if( 0x0001 & num){
			strcat(str,"1");
			num = num >> 1;
		}
		else{
			strcat(str,"0");
			num = num >> 1;
		}
	}
	return str;
}

char* toBinary32(int32 i, char *str){

	int32 num = i;
	strcpy(str, "");

	while(num){
		if( 0x0001 & num){
			strcat(str,"1");
			num = num >> 1;
		}
		else{
			strcat(str,"0");
			num = num >> 1;
		}
	}
	return str;
}

int16** Product1_8_Basic(int16** a, int16** b, int16** c, int32 wa, int32 wb){

		int16 carry;
		int32  i, j, p, flag= 0;


		if(wa > wb)
			flag = 1;
		else if(wa < wb)
			flag = 2;
		else if (wa == wb)
			flag = 0;


	    if(flag == 0){

	    	//zeroing out the remaining digits
	    	for(i = 0; i < 	wa ; i++ )
	    		*c[i] = 0;

	    	//Actual 1.8 implementation
	    	for(i = 0; i< wa; i++){
	    		carry = 0;
	    		for(j = 0; j < wa; j++){

	    			p = (int32)(*a[i])*(*b[j]) + *c[i + j] + carry;
	    			*c[i+j] = (int16)(p & (0xffff));
	    			carry = (int16)((p>>16) & (0xffff));
	    		}
	    		*c[i+wa] = carry;
	    	}
	    }

	    if(flag == 1){
	    	 //zeroing out the remaining digits
	    	 for(i = 0; i < wa ; i++ )
	    	    *c[i] = 0;

	    	 //Actual 1.8 implementation
	    	 for(i = 0; i< wa; i++){
	    		carry = 0;
	    	    for(j = 0; j < wb; j++){
	    	    	p = (int32)(*a[i])*(*b[j]) + *c[i + j] + carry;
	    	    	*c[i+j] = (int16)(p & (0xffff));
	    	    	carry = (int16)((p>>16) & (0xffff));
	    	    }
	    	    *c[i+wb] = carry;
	    	 }
	   }

	    if(flag == 2){
	    	 //zeroing out the remaining digits
	    	 for(i = 0; i < wb ; i++ )
	    	    *c[i] = 0;
	    	 //Actual 1.8 implementation
	    	 for(i = 0; i< wb; i++){
	    		carry = 0;
	    	    for(j = 0; j < wa; j++){
	    	    	p = (int32)(*b[i])*(*a[j]) + *c[i + j] + carry;
	    	    	*c[i+j] = (int16)(p & (0xffff));
	    	    	carry = (int16)((p>>16) & (0xffff));
	    	    }
	    	    *c[i+wa] = carry;
	    	 }
	   }
	    	return &c[0];
}

void Karatsuba(int16** a, int16** b, int16** res, int32 dig, int32 wa, int32 wb){

	//a is always either larger or equal to b
	// u1, u2 point to larger number and v1,v2 point to smaller number
	int32 i;
	int16 **u1, **v2, **u2;
	int32 U1SIZE = (wa - dig), U2SIZE = dig, T1SIZE = Max((wa - dig), dig), T2SIZE = Max((wb - dig), dig);
	int32 V1SIZE = (wb - dig), V2SIZE = dig, RESSIZE = (wa + wb);
	int32 W3SIZE = (T1SIZE + T2SIZE), W4SIZE = dig;
	int32 C1SIZE = (U2SIZE + V2SIZE - dig), C2SIZE = (W3SIZE - dig);


	if(dig == 1 || dig == 0)
		Product1_8_Basic(a,b,&res[10*RESSIZE],wa,wb);


	else{
		if(wa == wb){

			int16  **v1;

			if((u1 = (int16 **)calloc(U1SIZE,sizeof(int16*)))== NULL)
				fprintf(stderr, "\n Memory cannot be allocated to u1\n");
			if((u2 = (int16 **)calloc(U2SIZE,sizeof(int16*)))== NULL)
				fprintf(stderr, "\n Memory cannot be allocated to u2\n");
			if((v2 = (int16 **)calloc(V2SIZE,sizeof(int16*)))== NULL)
				fprintf(stderr, "\n Memory cannot be allocated to v1\n");
			if((v1 = (int16 **)calloc(V1SIZE,sizeof(int16*)))== NULL)
				fprintf(stderr, "\n Memory cannot be allocated to v2\n");

			  //Allocating space for individual pointers
			for(i = 0; i < U1SIZE; i++)
			   if((u1[i] = (int16 *)calloc(1, sizeof(int16))) == NULL)
				   fprintf(stderr,"\nMemory cannot be allocated\n");
			for(i = 0; i < U2SIZE; i++)
			   if((u2[i] = (int16 *)calloc(1, sizeof(int16))) == NULL)
				   fprintf(stderr,"\nMemory cannot be allocated\n");
			for(i = 0; i < V2SIZE; i++)
			   if((v2[i] = (int16 *)calloc(1, sizeof(int16))) == NULL)
				   fprintf(stderr,"\nMemory cannot be allocated\n");
			for(i = 0; i < V1SIZE; i++)
			   if((v1[i] = (int16 *)calloc(1, sizeof(int16))) == NULL)
				   fprintf(stderr,"\nMemory cannot be allocated\n");

			//populating u1, u2, v1, v2
		    //copying values
		    for(i = 0; i < U2SIZE; i++)
		        memcpy(u2[i], a[i], sizeof(int16));
		    for(i = 0; i < U1SIZE; i++)
		    	memcpy(u1[i], a[i + U2SIZE], sizeof(int16));
		    for(i = 0; i < V2SIZE; i++)
		        memcpy(v2[i], b[i], sizeof(int16));
		    for(i = 0; i < V1SIZE; i++)
		        memcpy(v1[i], b[i + V2SIZE], sizeof(int16));

		/*    //printing u1, u2, v1, v2
		    for(i = 0; i < U1SIZE; i++)
		    	printf("\n u1[%u] = %x" ,i, *u1[i]);
		    for(i = 0; i < U2SIZE; i++)
		      	printf("\n u2[%u] = %x" ,i, *u2[i]);
		    for(i = 0; i < V1SIZE; i++)
		    	printf("\n v1[%u] = %x" ,i, *v1[i]);
		    for(i = 0; i < V2SIZE; i++)
		      	printf("\n v2[%u] = %x" ,i, *v2[i]);
*/
		    //Calculating the sum (U1 + U2)
			Addition1_4(u1, u2, U1SIZE, U2SIZE, &res[2*RESSIZE]);
			Addition1_4(v1, v2, V1SIZE, V2SIZE, &res[3*RESSIZE]);

	/*	 	//Printing (U1+U2) and (V1+V2)
			for(i = 0; i < T1SIZE; i++)
		 		printf("\n (u1+u2) = %x", *res[2*RESSIZE + i]);
		 	for(i = 0; i < T2SIZE; i++)
		 		printf("\n (v1+v2) = %x", *res[3*RESSIZE + i]);
*/
		 	//product of T1,T2 & U1,V1 and U2,V2
		 	Product1_8_Basic(&res[2*RESSIZE], &res[3*RESSIZE], &res[4*RESSIZE],T1SIZE, T2SIZE );
		 	Product1_8_Basic(&u1[0], &v1[0], &res[0],U1SIZE, V1SIZE );
		 	Product1_8_Basic(&u2[0], &v2[0], &res[RESSIZE],U2SIZE, V2SIZE );

	/*	 	for(i = 0; i < W3SIZE; i++)
		 		printf("\n (u1 + u2)*(v1 + v2) = %x" , *res[4*RESSIZE + i]);
		 	for(i = 0; i < (U1SIZE + V1SIZE); i++)
		 	    printf("\n (u1)*(v1) = %x" , *res[i]);
		 	for(i = 0; i < (U2SIZE + V2SIZE); i++)
		 	    printf("\n (u2)*(v2) = %x" , *res[RESSIZE + i]);
*/
		 	//Calculating the difference of W3=(T1*T2), W2 = (U1*V1) and W4 = (U2*V2)
		 	Difference(&res[4*RESSIZE], &res[0], W3SIZE, (U1SIZE + V1SIZE),&res[6*RESSIZE]);
		 	Difference(&res[6*RESSIZE], &res[RESSIZE], W3SIZE, (U2SIZE + V2SIZE),&res[7*RESSIZE]);

	/*	 	for(i = 0; i < W3SIZE; i++)
		 		printf("\n (u1+ u2)*(v1 + v2) - u1*v1 = %x" ,*res[6*RESSIZE + i]);
		 	for(i = 0; i < W3SIZE; i++)
		 		printf("\n (u1+ u2)*(v1 + v2) - u1*v1 - u2*v2= %x" , *res[7*RESSIZE + i]);
*/
		 	// ADD W3 + C1, W2 + C2
		 	Addition1_4(&res[7*RESSIZE], &res[RESSIZE + W4SIZE], W3SIZE, C1SIZE, &res[8*RESSIZE]);
		 	if((U1SIZE + V1SIZE) > C2SIZE)
		 		Addition1_4(&res[0], &res[8*RESSIZE + dig], (U1SIZE + V1SIZE), C2SIZE, &res[9*RESSIZE]);
		 	else
		 		Addition1_4(&res[8*RESSIZE + dig], &res[0], C2SIZE, (U1SIZE + V1SIZE), &res[9*RESSIZE]);

		 	for(i = 0; i < Max(C2SIZE,(U1SIZE + V1SIZE)); i++){
		 	//	printf("\n W1 and W2 = %x", *res[9*RESSIZE + i]);
		 		memcpy(res[10*RESSIZE + 2*dig + i], res[9*RESSIZE + i], sizeof(int16));
		 	}
		 	for(i = 0; i < dig; i++){
		 	//	printf("\n W3 = %x", *res[8*RESSIZE + i]);
		 		memcpy(res[10*RESSIZE + dig + i], res[8*RESSIZE + i], sizeof(int16));
		 	}
		 	for(i = 0; i < W4SIZE; i++){
		 	//	printf("\n W4 = %x", *res[RESSIZE + i]);
		 		memcpy(res[10*RESSIZE +i], res[RESSIZE + i], sizeof(int16));
		 	}

		 	// freeing the pointers
	   	    for(i = 0; i < V1SIZE; i++)
	   	        free(v1[i]);

	   	    free(v1);
		}



		else{


			if((u1 = (int16 **)calloc(U1SIZE,sizeof(int16*)))== NULL)
				fprintf(stderr, "\n Memory cannot be allocated to u1\n");
			if((u2 = (int16 **)calloc(U2SIZE,sizeof(int16*)))== NULL)
				fprintf(stderr, "\n Memory cannot be allocated to u2\n");
			if((v2 = (int16 **)calloc(V2SIZE,sizeof(int16*)))== NULL)
				fprintf(stderr, "\n Memory cannot be allocated to v1\n");


			  //Allocating space for individual pointers
			for(i = 0; i < U1SIZE; i++)
			   if((u1[i] = (int16 *)calloc(1, sizeof(int16))) == NULL)
				   fprintf(stderr,"\nMemory cannot be allocated\n");
			for(i = 0; i < U2SIZE; i++)
			   if((u2[i] = (int16 *)calloc(1, sizeof(int16))) == NULL)
				   fprintf(stderr,"\nMemory cannot be allocated\n");
			for(i = 0; i < V2SIZE; i++)
			   if((v2[i] = (int16 *)calloc(1, sizeof(int16))) == NULL)
				   fprintf(stderr,"\nMemory cannot be allocated\n");


			//populating u1, u2, v1, v2
		    //copying values
		    for(i = 0; i < U2SIZE; i++)
		        memcpy(u2[i], a[i], sizeof(int16));
		    for(i = 0; i < U1SIZE; i++)
		    	memcpy(u1[i], a[i + U2SIZE], sizeof(int16));
		    for(i = 0; i < V2SIZE; i++)
		        memcpy(v2[i], b[i], sizeof(int16));


/*		    //printing u1, u2, v1, v2
		    for(i = 0; i < U1SIZE; i++)
		    	printf("\n u1[%u] = %x" ,i, *u1[i]);
		    for(i = 0; i < U2SIZE; i++)
		      	printf("\n u2[%u] = %x" ,i, *u2[i]);
		    for(i = 0; i < V2SIZE; i++)
		      	printf("\n v2[%u] = %x" ,i, *v2[i]);
*/
		    //Calculating the sum (U1 + U2)
		    if(U1SIZE > U2SIZE)
		    	Addition1_4(u1, u2, U1SIZE, U2SIZE, &res[2*RESSIZE]);
		    else
		    	Addition1_4(u2, u1, U2SIZE, U1SIZE, &res[2*RESSIZE]);

		    	for(i = 0; i < T2SIZE; i++)
		    		memcpy(res[3*RESSIZE + i], v2[i], sizeof(int16));

	/*	    	//Printing (U1+U2) and (V1+V2)
		    	for(i = 0; i < T1SIZE; i++)
		    		printf("\n (u1+u2) = %x", *res[2*RESSIZE + i]);
		    	for(i = 0; i < T2SIZE; i++)
		    		printf("\n (v1+v2) = %x", *res[3*RESSIZE + i]);
*/
		    	//product of T1,T2 & U1,V1 and U2,V2
		    	Product1_8_Basic(&res[2*RESSIZE], &res[3*RESSIZE], &res[4*RESSIZE], T1SIZE, T2SIZE );
		    	Product1_8_Basic(&u2[0], &v2[0], &res[RESSIZE], U2SIZE, V2SIZE );

		/*    	for(i = 0; i < W3SIZE; i++)
		    		printf("\n (u1 + u2)*(v1 + v2) = %x" , *res[4*RESSIZE + i]);
		    	for(i = 0; i < (U2SIZE + V2SIZE); i++)
		    		printf("\n (u2)*(v2) = %x" , *res[RESSIZE + i]);
*/
		    	//Calculating the difference of W3=(T1*T2), W2 = (U1*V1) and W4 = (U2*V2)
				Difference(&res[4*RESSIZE], &res[RESSIZE], W3SIZE, (U2SIZE + V2SIZE),&res[5*RESSIZE]);

			//	for(i = 0; i < W3SIZE; i++)
				//	printf("\n (u1+ u2)*(v1 + v2) - u1*v1 - u2*v2= %x" , *res[5*RESSIZE + i]);

				// ADD W3 + C1, W2 + C2
			 	Addition1_4(&res[5*RESSIZE], &res[RESSIZE + W4SIZE], W3SIZE, C1SIZE, &res[6*RESSIZE]);
			 	for(i = 0; i < W3SIZE; i++){
	//		 		printf("\n W1,W2 and W3 = %x", *res[6*RESSIZE + i]);
			 		memcpy(res[10*RESSIZE + i + dig], res[6*RESSIZE + i], sizeof(int16));
			 	}
			 	for(i = 0; i < W4SIZE; i++){
			 	//	printf("\n W4 = %x", *res[RESSIZE + i]);
			 		memcpy(res[10*RESSIZE + i], res[RESSIZE + i], sizeof(int16));
			 	}

		}



 		// freeing the pointers
   	    for(i = 0; i < U1SIZE; i++)
   	    	free(u1[i]);
   	    for(i = 0; i < U2SIZE; i++)
   	    	free(u2[i]);
   	    for(i = 0; i < V2SIZE; i++)
   	    	free(v2[i]);
   	    free(u1);
   	    free(u2);
   	    free(v2);
   	    //free(str);
	}
}


void Addition1_4(int16** x, int16** y, int32 wx, int32 wy, int16** sum){

	int16  carry = 0;
	int32 i,s = 0;

	for(i = 0; i < wy; i++){
		s = (int32)(*x[i]) + *y[i] + carry;
		*sum[i] = (int16)(s & 0xffff);
		carry = (int16)((s>>16) & 0xffff );
	}

	for(i = wy; i < wx; i++){
		s = (int32)(*x[i]) + carry;
		*sum[i] = (int16)(s & (0xffff));
		carry = (int16)((s>>16) & 0xffff);
	}
}

void Difference(int16** x, int16** y, int32 wx, int32 wy, int16** diff){

	int16  borrow = 0;
	int32 i,s = 0;


	for(i =0; i < wy; i++){
		s = (int32)(*x[i]) - *y[i] - borrow;
		*diff[i] = (int16)(s & 0xffff);
		//printf("\n%x - %x - %x = %x ", *x[i], *y[i], borrow, *diff[i]);
		borrow = (int16)((s>>16) & 0xffff);
		if(borrow != 0)
			borrow = 1;
	}

	for(i = wy; i < wx; i++){
		s = (int32)(*x[i]) - borrow;
		*diff[i] = (int16)(s & 0xffff);
		borrow = (int16)((s>>16) & (0xffff));
		if(borrow != 0)
			borrow = 1;
	}
}

//unsigned int gradeSchool(unsigned int  *x, unsigned int *y, unsigned int *z, )
/* You are suppose to change the routine Product32 here to your own routine
 * The mpz calls in the scaffolded Product32 below are the normal GMP function
 * calls and should be neglected. By casting the void pointers as normal unsigned
 * integers, you should be able to access the data values as normal 4 bytes words.
 */


/* wa is word length of a, ba is bit length of a */
void Product16(void *a, void *b, void *c, unsigned int wa, unsigned int ba, unsigned int wb, unsigned int bb, unsigned int *wc, unsigned int *bc){

	int32 SMALLER_WORDSIZE = 0, WA = (wa + 1) , WB = (wb + 1);
	int32 i = 0;

    /* Cast a and b into short integers of size 16 bits */
    int16 *int_a = (int16 *) a;
    int16 *int_b = (int16 *) b;
    int16 *int_c = (int16 *) c;

    // Finding the smaller of wa and wb
    if(WA == WB)
    	SMALLER_WORDSIZE = WA/2;
    else
    	SMALLER_WORDSIZE = (WA < WB) ? WA : WB;

    *wc = WA + WB;

  //  printf("\n wa = %u, wb = %u \n", wa, wb);


    //creating space for and copying a and b, creating space for c
    int16** int16_a = (int16 **)calloc(WA, sizeof(int16 *));
    if(int16_a == NULL)
    	fprintf(stderr,"\nMemory cannot be allocated\n");
    int16** int16_b = (int16 **)calloc(WB, sizeof(int16 *));
    if(int16_b == NULL)
        fprintf(stderr,"\nMemory cannot be allocated\n");
    int16** int16_c = (int16 **)calloc(*wc, sizeof(int16 *));
    if(int16_c == NULL)
        fprintf(stderr,"\nMemory cannot be allocated\n");


    //Allocating space for individual pointers
    for(i = 0; i < WA; i++)
    	if((int16_a[i] = (int16 *)calloc(1, sizeof(int16))) == NULL)
    		 fprintf(stderr,"\nMemory cannot be allocated\n");
    for(i = 0; i < WB; i++)
    	if((int16_b[i] = (int16 *)calloc(1, sizeof(int16))) == NULL)
    		 fprintf(stderr,"\nMemory cannot be allocated\n");
    for(i = 0; i < *wc; i++)
    	if((int16_c[i] = (int16 *)calloc(1, sizeof(int16))) == NULL)
    		 fprintf(stderr,"\nMemory cannot be allocated\n");

    //Allocating space for recursive workspace
    int16** result = (int16 **)calloc(12*(WA + WB), sizeof(int16 *));
        if(result == NULL)
        	fprintf(stderr,"\nMemory cannot be allocated\n");

    for(i = 0; i < 12*(WA + WB); i++)
    	if((result[i] = (int16 *)calloc(1, sizeof(int16))) == NULL)
            fprintf(stderr,"\nMemory cannot be allocated\n");

    //copying values
    for(i = 0; i < WA -1; i++)
        memcpy(int16_a[i], &int_a[i], sizeof(int16));
	*int16_a[WA-1] = 0;
    for(i = 0; i < WB - 1; i++)
    	memcpy(int16_b[i], &int_b[i], sizeof(int16));
	*int16_b[WB-1] = 0;

   /* printf("a = \n");
    //printing the values
    for(i = 0; i < wa; i++)
   	    printf(" %x" , *int16_a[i]);
    printf("b = \n");
   	for(i = 0; i < wb; i++)
   	    printf(" %x" , *int16_b[i]);
*/
   	if(WA > WB)
   	//karatsuba
   		Karatsuba(int16_a, int16_b, result , SMALLER_WORDSIZE, WA, WB);
   	else if(WA < WB)
   		Karatsuba(int16_b, int16_a, result , SMALLER_WORDSIZE, WB, WA);
   	else
   		Karatsuba(int16_a, int16_b, result , SMALLER_WORDSIZE, WA, WB);

	for(i = 0; i < *wc; i++)
		memcpy(&int_c[i],result[10*(WA+WB)+i], sizeof(int16));

/*
    printf("\nThe value of 1<<32 is %s", toBinary64((int32)1<<32 , str));
    printf("\nThe size of unsigned short int is %u bytes", sizeof(int16));
    printf("\nThe size of unsigned int is %u bytes", sizeof(int16));
    printf("\nThe size of unsigned long int is %u bytes", sizeof(int16));
    printf("\nThe size of unsigned long long int is %u bytes", sizeof(int32));

    	Product1_8_Basic(int16_a, int16_b, int16_c, wa, wb);

    	for(i = 0; i < *wc; i++)
    		memcpy(&int_c[i],int16_c[i], sizeof(int16));

*/

	/*  mpz_import(x, wa, ORDER, WORDBYTES, ENDIAN, NAILS, a);
		mpz_import(y, wb, ORDER, WORDBYTES, ENDIAN, NAILS, b);
	    mpz_mul(z,x,y);
		c = mpz_export(c, wc, ORDER, WORDBYTES, ENDIAN, NAILS, z);
	*/

   	// freeing the pointers
   	    for(i = 0; i < WA; i++)
   	    	free(int16_a[i]);
   	    for(i = 0; i < WB; i++)
   	    	free(int16_b[i]);
   	    for(i = 0; i < *wc; i++)
   	    	free(int16_c[i]);

   	    for(i = 0; i < 12*(WA+WB); i++)
   	    	free(result[i]);

   	    free(int16_a);
   	    free(int16_b);
   	    free(int16_c);
   	    free(result);

}


