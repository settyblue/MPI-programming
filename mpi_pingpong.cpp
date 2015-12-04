#include<mpi.h>
#include<stdlib.h>
#include<stdio.h>
using namespace std;

#define PING_PONG_LIMIT 5

int main(int argc, char** argv){
	//Initialize the MPI Communicator.
	MPI_Init(NULL,NULL);

	//Get the world size and the rank.
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	//set the ping pong count limit.
	int MESSAGE_SIZE = 2;
	int ping_pong_count = 0;
	int partner_rank = (world_rank + 1) % 2;
	
	//Run the iterations for playing the ping pong of messages.
	for(int i=0; i<PING_PONG_LIMIT; i++){
		if(world_rank == ping_pong_count%2){
			ping_pong_count++;
			MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
			printf("%d sent and incremented ping_pong_count %d to %d\n", world_rank, ping_pong_count,
				partner_rank);
		} else {
			MPI_Recv(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("%d rececived ping_pong_count %d from %d\n", world_rank, ping_pong_count,
				partner_rank);
		}
	}
	//Finalize the MPI library.
	MPI_Finalize();
}