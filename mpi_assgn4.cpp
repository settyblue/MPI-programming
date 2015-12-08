#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define NUMBER_OF_ITERATIONS 1000000
#define NUMBER_OF_MESSAGE_SIZE 5
using namespace std;

int main(int argc, char** argv){
	//Initialize MPI Environment.
	MPI_Init(NULL,NULL);
	clock_t start_t,end_t;
	double time_taken, time_taken_per_message_pass, bandwidth;
	//Get the world size and the rank.
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD,&world_size);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);

	int message_size[NUMBER_OF_MESSAGE_SIZE] = {32,256,512,1024,2048};
	//variable to 
	double* data_A ;//= (double*)malloc(sizeof(double)*2048);
	double* data_B ;//= (double*)malloc(sizeof(double)*2048); 
	for(int i=0; i < NUMBER_OF_MESSAGE_SIZE; i++){
		data_A = (double*)malloc(sizeof(double)*message_size[i]);
		data_B = (double*)malloc(sizeof(double)*message_size[i]);
		for(int j=0; j < message_size[i] ; j++){
			data_A[j] = j;
			data_B[j] = j;
		}
		MPI_Barrier(MPI_COMM_WORLD);
		start_t = clock();

		for(int j=0;j<NUMBER_OF_ITERATIONS;j++){
			if(world_rank == 0){
				MPI_Send(data_A,message_size[i],MPI_DOUBLE,1,0,MPI_COMM_WORLD);
				MPI_Recv(data_B,message_size[i],MPI_DOUBLE,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
				MPI_Send(data_B,message_size[i],MPI_DOUBLE,1,0,MPI_COMM_WORLD);
				MPI_Recv(data_A,message_size[i],MPI_DOUBLE,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			}else{
				MPI_Recv(data_A,message_size[i],MPI_DOUBLE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
				MPI_Send(data_B,message_size[i],MPI_DOUBLE,0,0,MPI_COMM_WORLD);
				MPI_Recv(data_B,message_size[i],MPI_DOUBLE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
				MPI_Send(data_A,message_size[i],MPI_DOUBLE,0,0,MPI_COMM_WORLD);
			}
		}
		MPI_Barrier(MPI_COMM_WORLD);

		end_t = clock();
		time_taken = (double)end_t - start_t;
		time_taken_per_message_pass = (double)time_taken/(4*NUMBER_OF_ITERATIONS);
		bandwidth = sizeof(double)*message_size[i]/time_taken_per_message_pass;
		if(world_rank == 0)
			printf("Time taken for communication of data size %d is %lf seconds, with bandwidth %lf \n\n",message_size[i],time_taken_per_message_pass, bandwidth);
		free(data_A);
		free(data_B);
	}
	//Finish MPI.
	MPI_Finalize();
}

