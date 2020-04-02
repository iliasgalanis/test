#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char **argv)
{  
   int rank, size;
   int a=1;
   
   MPI_Init(&argc, &argv);

   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);

   if(rank==0){
      printf("\nRank:%2d, stelnw twra: %4d\n", rank, a);
      MPI_Send(&a,1,MPI_INT,1,0,MPI_COMM_WORLD);
      MPI_Recv(&a,1,MPI_INT,size-1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      printf("\nRank:%2d, parelava: %4d\n\n", rank, a);
    }
   else 
   {
      MPI_Recv(&a,1,MPI_INT,(rank-1),0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      printf("\nRank:%2d, parelava xtes: %4d", rank, a);
      a*=2;
      printf("\nRank:%2d, stelnw: %4d\n", rank, a);
      MPI_Send(&a,1,MPI_INT,(rank+1)%size,0,MPI_COMM_WORLD);
   }

   MPI_Finalize();
   return 0;
}
