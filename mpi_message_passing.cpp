#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

int main(int argc, char** argv){
	//Initialie the MPU environment.
	MPI_Init(NULL,NULL);
	//Find out rank and size.
	int world_size, world_rank;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	int number;
	if(world_rank == 0){
		number = 100;
		MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		printf("Process 0 sent number %d to the process 1\n", number);
	}else if(world_rank == 1){
		MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Process 1 received number %d from the process 0\n", number);
	}

	//Finalize the MPI library.
	MPI_Finalize();
}