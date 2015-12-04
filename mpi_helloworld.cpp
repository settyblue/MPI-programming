#include<mpi.h>
#include<stdio.h>
#include<iostream>
using namespace std;

int main(int argc, char** argv){
	//Initialize MPI environment
	MPI_Init(NULL,NULL);

	//Get the number of processes
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	//Get the rank of the process.
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);

	//Get the name of the processor
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);

	//Print off a hellp world message.
	//cout<<"Hello World from processor "<<processor_name<<", of rank "<<world_rank<<" out of "
	//		<<world_size<<" processors\n";
	printf("hello World from processor %s, rank %d out of %d processors\n",processor_name,
		world_rank, world_size);

	MPI_Finalize();
}