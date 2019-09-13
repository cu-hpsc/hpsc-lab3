#include <stdlib.h>

#define A_MIN (-2.0)
#define A_MAX (2.0)
#define B_MIN (0.0)
#define B_MAX (2.0)

#define SURVIVED(real,imag) (real*real + imag*imag <= 4)

#define MMAP_REAL(real,imag,real_0) (real*real - imag*imag + real_0)
#define MMAP_IMAG(real,imag,imag_0) (real*imag*2 + imag_0)

// allocates memory to vec, initialize its values, and returns size
long init_vec(double **vec, double min, double max, long density)
{
  long size = (max-min)*density+1;
  double span = max-min;
  long ind;
  *vec = malloc(sizeof(double) * size);
  for (ind=0; ind<size; ++ind) {
    (*vec)[ind] = ind*span/(size-1) + min;
  }
  return size;
}

long init_A(double **A, long density) {
  return init_vec(A, A_MIN, A_MAX, density);
}

long init_B(double **B, long density) {
  return init_vec(B, B_MIN, B_MAX, density);
}
