/*
 * fread_sample.c
 *
 *  Created on: Nov 30, 2009
 *      Author: deepakkoni
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define PACKETSIZE 1024

static char *buffer, *buffer1;

int main () {
  FILE * pFile, *pFile1;
  long lSize, tempSize;
  size_t result, result1;

  pFile = fopen ( "myfile.pdf" , "rb" );
  pFile1 = fopen ( "myfile1.pdf" , "wb" );

  if (pFile==NULL) {
	  fputs ("File error",stderr);
	  exit (1);
  }

  // obtain file size:
  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  rewind (pFile);

  buffer = (char *)calloc(1,lSize*sizeof(char));

  /*buffer1 = (char *)calloc(1,lSize*sizeof(char));

  while(!feof(pFile)){

	  tempSize = lSize - ftell(pFile);

	  if(tempSize == 0)
		break;

	  if(tempSize < PACKETSIZE){
		  if((result = fread (buffer,1,tempSize,pFile)) != tempSize){
			  fprintf(stderr,"\nfread() error");
		  }
		  strcpy(buffer1,buffer);
		  if((result1 = fwrite (buffer1,1,tempSize,pFile1)) != tempSize){
		  			  fprintf(stderr,"\nfwrite() error");
		  		  }
	  }
	  else{
		  if((result = fread (buffer,1,PACKETSIZE,pFile)) != PACKETSIZE){
			  fprintf(stderr,"\nfread() error");
		  }
		  strcpy(buffer1,buffer);
		  if((result1 = fwrite (buffer1,1,PACKETSIZE,pFile1)) != PACKETSIZE){
			  fprintf(stderr,"\nfwrite() error");
		  }
	  }

  }
*/

  result = fread (buffer,1,lSize,pFile);
  if (result != lSize) {
	  fputs ("Reading error",stderr);
	  exit (3);
  }

  fseek (pFile1 , 0 , SEEK_SET);
  result1 = fwrite (buffer , 1 , lSize , pFile1 );
  if (result1 != lSize) {
  	  fputs ("Writing error",stderr);
  	  exit (4);
    }

  free(buffer);
  fclose (pFile);
  fclose (pFile1);
  return 0;
}
