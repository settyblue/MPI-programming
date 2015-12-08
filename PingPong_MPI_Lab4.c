#include<stdio.h>
#include<mpi.h>	
#include<sys/time.h>
#include<time.h>

int main (int argc, char *argv[])
{	  
int i, rank, size, test_size;
MPI_Status   status;
double timeTaken,bandWidth,timeIteration;
clock_t t1, t2 ;   	  
 	   
		MPI_Init (&argc, &argv);         	   
              	   
		MPI_Comm_rank (MPI_COMM_WORLD, &rank); 	   
	   
		MPI_Comm_size (MPI_COMM_WORLD, &size);    	    	   
	  	
		 if (size != 2) {	           
		    printf ("Please run with 2 procs only.\n");	          
			MPI_Abort(MPI_COMM_WORLD, 1);	      
		}	        	            	   	   
				
		int test_data_size[] = {32,256,512,1024,2048};
		
		for (i= 0; i < 5; i++){
			double *package ,*package1;
			package = (double*) malloc(sizeof(double)*test_data_size[i]);
			package1 = (double*) malloc(sizeof(double)*test_data_size[i]);
                                int k;
				for ( k=0; k<test_data_size[i]; k++)
					{
					package[k]=(double)k;
					package1[k]=(double)k;
					}
					test_size = test_data_size[i];
					MPI_Barrier (MPI_COMM_WORLD);
					t1= clock();
                                        int j;
					for( j = 1000000 ; j >0 ;j --){
				    if (rank == 0 ) {           
			    MPI_Send( package, test_size, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
                            MPI_Recv( package1, test_size, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &status);
                            MPI_Send( package1, test_size, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
                            MPI_Recv( package, test_size, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &status);
					             
					  }	
	         
					 else { 
							MPI_Recv( package, test_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
							MPI_Send( package1, test_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
							MPI_Recv( package1, test_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
							MPI_Send( package, test_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
				      }								   	     
			 }	   
			 t2 = clock();
			 timeTaken =(double)t2 - t1 ;
			 timeIteration = (double)timeTaken / (4 *1000000);
			 bandWidth = (double)(sizeof(double)*test_size)/ timeIteration ;
			 if(rank == 0)	
			 {
			 	printf("Time Taken by Process %d for size %d is %lf \n",rank,test_size,timeIteration);
			 	printf("BandWidth Achieved by Process %d for size %d is %lf \n",rank,test_size,bandWidth);
			 }
			 else
			 {
			 	printf("Time Taken by Process %d for size %d is %lf \n",rank,test_size,timeIteration);
			 	printf("BandWidth Achieved by Process %d for size %d is %lf \n",rank,test_size,bandWidth);
			 }
			 free(package);	  
			 free(package1);  
	}
	MPI_Finalize();
}
