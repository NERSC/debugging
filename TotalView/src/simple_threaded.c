#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#ifdef ADD_MPI
#include <mpi.h>
#endif //ADD_MPI

void random_vector(std::vector<int>& vec)
{
  size_t count = (size_t)rand() % 1000;
  if(count < 100) count = 150;

  for(size_t i=0; i<count; ++i)
  {
    vec.push_back(rand());
    usleep(20);
  }

}

void runme()
{
  std::vector<int> vec;

  random_vector(vec);

  std::cout << "A thread has finished" << std::endl;
}



int main(int argc, char** argv)
{
#ifdef ADD_MPI
  int rank, nnodes, nthreads;
  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Comm_size (MPI_COMM_WORLD, &nnodes);
#endif //ADD_MPI

  time_t tm = time(NULL);
  srand(tm);
  int numThreads = 25;
  if(argc >= 2)
  {
    numThreads = atoi(argv[1]);
    if(numThreads < 0)
      numThreads = 25;
  }

  std::vector<pthread_t> threads;
  threads.reserve(numThreads);

  std::cout << "Main is going to create " << numThreads << " threads." << std::endl;

  for(int i=0; i<numThreads; ++i)
  {
    pthread_t newThread;
    pthread_create(&newThread, NULL, (void*(*)(void*))runme, NULL);
    threads.push_back(newThread);
  }

  for(int i=0; i<numThreads; ++i)
  {
    pthread_join(threads[i], NULL);
  }

#ifdef ADD_MPI
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
#endif //ADD_MPI

  return 0;
}

