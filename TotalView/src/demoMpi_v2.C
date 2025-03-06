/* compile with mpiCC -o demoMpi demoMpi.C -g -lm */
/* run with mpirun -np 10 -tv demoMpi for Message Queue and
   run with mpirun -np 4 -tv demoMpi  for Lamination and broken links */ 
#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


template<class T>
T getMax(T* A,int  b);

template<class T>
T* get_full_domain(int a);

int BUFLEN=512, NSTEPS=10;

char *sendMessage(char *sbuf, char *recbuf, int dest, int source)
{
    MPI_Status status;
    int my_mpi_comm_world=MPI_COMM_WORLD;
    int my_mpi_char=MPI_CHAR;

    MPI_Recv(recbuf, BUFLEN, my_mpi_char, source, 99, my_mpi_comm_world, &status);
    MPI_Send(sbuf, strlen(sbuf)+1, my_mpi_char, dest, 99, my_mpi_comm_world);
    return recbuf;
}

char *f2(char *sbuf, char *recbuf, int dest, int source)
{
    return sendMessage(sbuf, recbuf, dest, source );
}
   
char *f1(char *sbuf, char *recbuf, int dest, int source)
{
    return f2(sbuf, recbuf, dest, source );
}

char *g2(char *sbuf, char *recbuf, int dest, int source)
{
    return sendMessage(sbuf, recbuf, dest, source );
}

char *g1(char *sbuf, char *recbuf, int dest, int source)
{
    return g2(sbuf, recbuf, dest, source );
}


int main(int argc, char *argv[])
{
    int		root=0,full_domain_length,sub_domain_length;
    double	global_max,local_max;
    double	*full_domain,*sub_domain;
    int myid, numprocs, next, namelen, previous;
    int default_length=1000;
    char* sendBuffer=new char[BUFLEN];
    char* recvBuffer=new char[BUFLEN];
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int my_mpi_comm_world=MPI_COMM_WORLD;
    int my_mpi_double=MPI_DOUBLE;
    int my_mpi_max=MPI_MAX;
    int my_mpi_int=MPI_INT;
    MPI_Init(&argc,&argv);
    // sleep(1);
    MPI_Comm_size(my_mpi_comm_world,&numprocs);
    MPI_Comm_rank(my_mpi_comm_world,&myid);
    MPI_Get_processor_name(processor_name,&namelen);

    MPI_Get_processor_name(processor_name,&namelen);
    if( myid%2 == 0) sleep( 2 );

    fprintf(stderr,"Process %d on %s\n",myid,processor_name);
    sprintf(sendBuffer,"hello there from %d on %s",myid,processor_name);
    next = myid+4; /* set a barrier here */
    if(next>=numprocs) 
      next-=numprocs;
    previous = myid-4;
    if(previous<0) 
      previous+=numprocs;
               
    /*  Part 1: Deadlock and code patching */    
    /* After hitting deadlock, enable eval points and restart program */ 
    if (myid%2==0){
        f1(sendBuffer, recvBuffer, next, previous);
    } else {
        g1(sendBuffer, recvBuffer, next, previous );
    }

    fprintf(stderr,"%d get '%s'\n",myid,recvBuffer);
    MPI_Barrier(my_mpi_comm_world);

   /* Part 2: Collective communication  */
/*
 * Root obtains full domain and broadcasts its length.
 */
    if (myid == root) {
      if( argc > 2) full_domain_length=atoi((char*)argv[2]);
      else full_domain_length=default_length;
      full_domain=get_full_domain<double>(full_domain_length);
    }
    MPI_Bcast(&full_domain_length, 1, my_mpi_int, root, my_mpi_comm_world);
/*
 * Allocate subdomain memory.
 * Scatter the initial dataset among the processes.
 */
    sub_domain_length = full_domain_length / numprocs;
    sub_domain = new double[sub_domain_length];

    MPI_Scatter(full_domain, sub_domain_length, my_mpi_double,
   	    sub_domain, sub_domain_length, my_mpi_double,
    	    root, my_mpi_comm_world);
/*
 * Loop computing and determining max values.
   Stop here and observe effect of Scatter. Dive on sub_domain and show array of size
   sub_domain_length. Laminate and visualize. Then hit Go. all subarrays will be sorted
   in parallel and their local max will be returned.
 */
   local_max=getMax<double>(sub_domain, sub_domain_length);
   MPI_Reduce(&local_max, &global_max, 1, my_mpi_double,
		my_mpi_max, root, my_mpi_comm_world);
/*
 * Gather final dataset.
   Dive and Laminate local_max to observe different values for local subarray. Now
   visualize again and observe sorted arrays.
 */
    MPI_Gather(sub_domain, sub_domain_length, my_mpi_double,
	    full_domain, sub_domain_length, my_mpi_double,
	    root, my_mpi_comm_world);

    /* Part 3 all to all comunication and "bottleneck problem" */
    /* After you reached barrier hit Go and then Halt. Open Message
       Queue Graph. You can clearly identify bottleneck at node 1. Enable eval and Go */

    if(myid==1){
      int work=1;
      while(work)
	work=1;
    }
    
    MPI_Alltoall(sub_domain, sub_domain_length/numprocs, my_mpi_double,
		 sub_domain, sub_domain_length/numprocs, my_mpi_double,
		 my_mpi_comm_world);
    


    MPI_Finalize();
    /* Before finishing you can Laminate and Visualize sub_array again to check AlltoAll execution */
    return (0);
}
//// **************************************************************************************** ///
template<class T>
T* get_full_domain(int size){
  T* array=new T[size];
  for(int i=0;i<size;++i){
    array[i]=rand();//*100/RAND_MAX;
  }
  return array; 
}
template<class T>
int myCmp(const void *a, const void *b){
  T* i=(T*)a;
  T* j=(T*)b;
  return (*i<*j?-1:(*i==*j?0:1));
}

template<class T>
T getMax(T* array,int length){
  qsort(array,length,sizeof(T),&myCmp<T>);
  T lmax=array[length-1];
  return lmax;
}
