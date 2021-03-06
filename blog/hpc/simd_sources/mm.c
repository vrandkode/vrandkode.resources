#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define N 800

double gettime(void)
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec + 1e-6*tv.tv_usec;
}

int main(int argc, char *argv[]) 
{
    float a[N][N], b[N][N], c[N][N];
    int i,j,k;
    float sum = 0.0;
    double dtime;

    // Initialize input matrices
    for (i = 0; i < N; i++)
       for (j = 0; j < N; j++)
       {
          a[i][j] = b[i][j] = 3.14;
          c[i][j] = 0.0;
       }

    // Matrix multiplication
    dtime = gettime();

    for (i = 0; i < N; i++) 
       for (k = 0; k < N; k++) 
          for (j = 0; j < N; j++) 
             c[i][j] = c[i][j] + a[i][k]*b[k][j];

    dtime = gettime() - dtime;

    for (i = 0; i < N; i++)
       for (j = 0; j < N; j++)
          sum += c[i][j];

    // Print results
    printf("Sum for matrix C: %12.4f\n",sum);
    printf("Exec time: %9.5f sec.\n",dtime); 
}
