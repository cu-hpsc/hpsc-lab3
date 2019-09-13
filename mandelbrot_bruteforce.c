#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "parse.h"
#include "shared.h"

int main(int argc, char **argv)
{
  struct args args;
  double *A_0; // real part of candidates
  double *B_0; // imaginary part of candidates
  double *A; // current real part of function iteration
  double *B; // current imaginary part of function iteration
  double scratch;
  long numA;
  long numB;
  long indf;
  long indA;
  long indB;
  long indAB;
  clock_t lasttick;
  long maps;

  // argument parsing via parse.h
  parse_args(&args, argc, argv);

  // make points using shared.h
  numA = init_A(&A_0, args.density);
  numB = init_B(&B_0, args.density);
  A = malloc(sizeof(double) * numA * numB);
  B = malloc(sizeof(double) * numA * numB);
  fprintf(stderr,"allocated %ld bytes\n",sizeof(double)
	  *(numA+numB+numA*numB*2));
  for (indA=0; indA<numA; ++indA) {
    for (indB=0; indB<numB; ++indB) {
      indAB = indA*numB+indB;
      A[indAB] = A_0[indA];
      B[indAB] = B_0[indB];
    }
  }

  // iterate over map
  lasttick = clock();
  for (indf=0; indf<args.max_iter; ++indf) {
    // apply function to each grid point
    for (indA=0; indA<numA; ++indA) {
      for (indB=0; indB<numB; ++indB) {
	indAB = indA*numB+indB;
	scratch = MMAP_REAL(A[indAB],B[indAB],A_0[indA]);
	B[indAB] = MMAP_IMAG(A[indAB],B[indAB],B_0[indB]);
	A[indAB] = scratch;
      }
    }
  }
  maps = args.max_iter * numA * numB;
  fprintf(stderr,"%ld maps\n",maps);
  lasttick = clock() - lasttick;
  fprintf(stderr,"%ld ticks\n",(long) lasttick);
  fprintf(stderr,"%f maps/tick\n",(double) maps / lasttick);

  // test for divergence
  printf("%f %f %f %f\n",A_MIN,A_MAX,B_MIN,B_MAX);
  for (indA=0; indA<numA; ++indA) {
    for (indB=0; indB<numB; ++indB) {
      indAB = indA*numB+indB;
      printf("%f ",(float) SURVIVED(A[indAB],B[indAB]));
    }
    printf("\n");
  }

  free(A_0);
  free(B_0);
  free(A);
  free(B);
  return 0;
}
