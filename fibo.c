#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  
  MPI_Init(NULL, NULL);
 
  int fibo(int pos){
      if(pos>1){
          return fibo(pos-1)+fibo(pos-2); 
      }else{
          if(pos==0){
              return 0;
          }
          if(pos==1){
              return 1;
          }
      }
  }
  int namelen;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  MPI_Get_processor_name(processor_name, &namelen);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  if (world_size != 6 && world_size != 1) {
    fprintf(stderr, "World size must be six or one for %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  double startwtime = 0.0, endwtime;
  int r,n;
  n = 48;
  int num;
  num = n;
  int result;
  int constF;
  constF = 4;
  
  if (world_rank != 0) {
    n = n - (world_rank+constF);
    r = fibo(n);
    if(world_rank != 1){
        if(world_rank == 5 || world_rank == 2){
            r = 5*r;
        }else{
            r = 10*r;
        }
    }
    MPI_Send(&r, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    printf("%s Process %d send r =  %d to process 0\n",processor_name, world_rank, r);
  } else {
    startwtime = MPI_Wtime();
    if(world_size==1){
        result = fibo(n);
    }else{
        result = fibo(n - (world_size+constF));
    }
    printf("\n%s Process %d send r =  %d to process 0\n",processor_name, world_rank, result);
    printf("%s Process %d receive r =  %d , result = %d from 0 \n",processor_name, world_rank, result,result);
    for(int i=1 ; i<world_size ; i++){
        MPI_Recv(&r, 1, MPI_INT,i, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        
        result = result + r;
        printf("%s Process %d receive r =  %d , result = %d from %d \n",processor_name, world_rank, r,result,i);
    }
    endwtime = MPI_Wtime();
    printf("\nFibonacci of %d  = %d \n",num,result);
    printf("Execution time = %f\n\n", endwtime - startwtime);
  }
  MPI_Finalize();
}


