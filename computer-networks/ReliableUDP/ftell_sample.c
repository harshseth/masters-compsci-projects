/*
 * ftell_sample.c
 *
 *  Created on: Nov 30, 2009
 *      Author: deepakkoni
 */

#include <stdio.h>

int main ()
{
  FILE * pFile;
  long size;

  pFile = fopen ("myfile.pdf","rb");
  if (pFile==NULL) perror ("Error opening file");
  else
  {
    fseek (pFile, 0, SEEK_END);
    size=ftell (pFile);
    fclose (pFile);
    printf ("Size of myfile.pdf: %ld bytes.\n",size);
  }
  return 0;
}
