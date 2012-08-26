/* difftime example */
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

int main ()
{
  time_t start,end;
  struct timeval tvStart, tvEnd;
  char szInput [256];
  double dif;

  gettimeofday(&tvStart, NULL);
  start = (time_t)tvStart.tv_sec;
  printf ("Please, enter your name: ");
  gets (szInput);
  gettimeofday(&tvEnd, NULL);
  end = (time_t)tvEnd.tv_sec;
  //dif = difftime (end,start);
  printf ("Hi %s.\n", szInput);
  printf ("It took you %.2lf seconds to type your name.\n", ((double)tvEnd.tv_sec + (double)((double)tvEnd.tv_usec/ 1000000.0)) - ((double)tvStart.tv_sec + (double)((double)tvStart.tv_usec/ 1000000.0)));

  return 0;
}
