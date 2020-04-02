#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main( int argc, char **argv )
{
    int rank, size;
    int N, part;
    float *X, *Y, *Xw, *Yw;
    int i=0;
    float sumx=0, sumy=0, sumtotalx, sumtotaly;
    float Cx, Cy; // centroid
    float dis, max, min, maxtotal, mintotal;

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    
    if (rank == 0) {
       printf("N=");
       scanf("%d", &N);
       X = malloc(N * sizeof(float));
       Y = malloc(N * sizeof(float));
       for (i=0; i<N; i++) {
          // tyxaies float times sto diastima 0 - 100
          X[i] = (float)rand()/((float)RAND_MAX/100);
          Y[i] = (float)rand()/((float)RAND_MAX/100);
          printf("%.2f - %.2f\n", X[i], Y[i]);
       }
       
   }

   MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

   part = N / size;
   Xw = malloc(part * sizeof(float));
   Yw = malloc(part * sizeof(float));
   MPI_Scatter(X, part, MPI_FLOAT, Xw, part, MPI_FLOAT, 0, MPI_COMM_WORLD);
   MPI_Scatter(Y, part, MPI_FLOAT, Yw, part, MPI_FLOAT, 0, MPI_COMM_WORLD);
   for (i=0; i<part; i++) {
      sumx += Xw[i];
      sumy += Yw[i];
      printf("%d: %d - %.2f, %.2f -> %.2f, %.2f\n", rank, i, Xw[i], Yw[i], sumx, sumy);
   }
   MPI_Allreduce(&sumx, &sumtotalx, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD); 
   MPI_Allreduce(&sumy, &sumtotaly, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
   Cx = sumtotalx / N;
   Cy = sumtotaly / N;
   max = dis = sqrt((Xw[0] - Cx)*(Xw[0] - Cx) + (Yw[0] - Cy)*(Yw[0] - Cy));;
   min = max;
   for (i=0; i<part; i++) {
     dis = sqrt((Xw[i] - Cx)*(Xw[i] - Cx) + (Yw[i] - Cy)*(Yw[i] - Cy));
     if (max < dis)
        max = dis;
     if (min > dis)
        min = dis;
     printf("%.2f - %.2f\n", max, min);
   }
   MPI_Reduce(&max, &maxtotal, 1, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);
   MPI_Reduce(&min, &mintotal, 1, MPI_FLOAT, MPI_MIN, 0, MPI_COMM_WORLD);
   if (rank==0) {
      printf("\n\nCentroid = %.2f, %.2f\n", Cx, Cy);
      printf("Megisti synoloiki  apostasi = %.2f\n", maxtotal);
      printf("Elaxisti synoliki apostasi = %.2f\n", mintotal);
   }
   MPI_Finalize();
   return 0;
}
