#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <ctime>
#define NUMBER_OF_ITERATIONS 1000000
#define NUMBER_OF_MESSAGE_SIZE 5
using namespace std;

double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

int main(int argc, char** argv){
	//Initialize MPI Environment.
	MPI_Init(NULL,NULL);
	clock_t start_t,end_t;
	double s,e;
	//Get the world size and the rank.
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD,&world_size);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);

	int message_size[NUMBER_OF_MESSAGE_SIZE] = {32,256,512,1024,2048};
	int m = 32;
	//variable to 
	double* data_A ;//= (double*)malloc(sizeof(double)*2048);
	double* data_B ;//= (double*)malloc(sizeof(double)*2048); 
	//timer variables.
	double start, end;
	//partner rank assuming only two processes are active.
	//int partner_rank = (world_rank + 1) % 2;
	for(int i=0; i < NUMBER_OF_MESSAGE_SIZE; i++){
		data_A = (double*)malloc(sizeof(double)*message_size[i]);
		data_B = (double*)malloc(sizeof(double)*message_size[i]);
		MPI_Barrier(MPI_COMM_WORLD);
		start_t = clock();
		start = MPI_Wtime();
		s = get_wall_time();
		for(int j=0;j<NUMBER_OF_ITERATIONS;j++){
			if(world_rank == 0){
				MPI_Send(data_A,message_size[i],MPI_DOUBLE,1,0,MPI_COMM_WORLD);
				//printf("Sent data from %d to %d\n",0,1);
				MPI_Recv(data_A,message_size[i],MPI_DOUBLE,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
				//printf("%d Revceived data from %d\n",0,1);
			}else{
				MPI_Send(data_A,message_size[i],MPI_DOUBLE,0,0,MPI_COMM_WORLD);
				//printf("Sent data from %d to %d\n",1,0);
				MPI_Recv(data_A,message_size[i],MPI_DOUBLE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
				//printf("%d Revceived data from %d\n",1,0);
			}
		}
		MPI_Barrier(MPI_COMM_WORLD);
		end = MPI_Wtime();
		end_t = clock();
		e = get_wall_time();
		float time_taken = (float)(end_t - start_t);
		time_taken = time_taken/100;
		if(world_rank == 0)
			printf("Time taken for communication of data size %d is %d seconds.\n\n",message_size[i],time_taken);
	}

	//Finish MPI.
	MPI_Finalize();
}

