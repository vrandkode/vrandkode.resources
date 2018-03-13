#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <smmintrin.h>

double gettime(void)
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec + 1e-6*tv.tv_usec;
}

void sinx (int N, int terms, float *x, float *result)
{
  int i,j;

  for (i=0; i<N; i+=4)
  {
    __m128 origx = _mm_load_ps(&x[i]);
    __m128 value = origx;
    //float value = x[i];
    __m128 numer = _mm_mul_ps(origx,_mm_mul_ps(origx,origx));
    //float numer = x[i]*x[i]*x[i];
    __m128 denom = _mm_set1_ps((float) 6);
    //int denom = 6;
    int sign = -1;

    for (j=1; j<=terms; j++)
    {
      __m128 tmp = _mm_div_ps(_mm_mul_ps(_mm_set1_ps((float) sign),numer),denom);
      value = _mm_add_ps(value,tmp);
      //value += sign*numer/denom;
      numer = _mm_mul_ps(numer,_mm_mul_ps(origx,origx));
      //numer *= x[i]*x[i];
      denom = _mm_mul_ps(denom,_mm_set1_ps((float)(2*j+2)*(2*j+3)));
      //denom *= (2*j+2)*(2*j+3);
      sign *= -1;
    }

    _mm_store_ps(&result[i],value);
    //result[i] = value;
  }
}

int main(int argc, char *argv[])
{
  int N, terms;
  float *x, *result;
  int i;
  float sum = 0.0;
  double dtime;

  if (argc > 2){
    N = atoi(argv[1]);
    terms = atoi(argv[2]);
  } else if (argc > 1){
    N = atoi(argv[1]);
    terms = 12;
  } else{
    N = 131072;
    terms = 12;
  }
  printf("Using N=%d and terms=%d\n",N,terms);

  x = (float *) malloc(N*sizeof(float));
  result = (float *) calloc(N,sizeof(float));

  for (i=0; i<N; i++)
    x[i] = 1.0;

  dtime = gettime();

  sinx(N,terms,x,result);

  dtime = gettime() - dtime;

  for (i=0; i<N; i++)
    sum += result[i];

  printf("Sum for result: %12.4f\n",sum);
  printf("Exec time: %9.5f sec.\n",dtime); 
}
