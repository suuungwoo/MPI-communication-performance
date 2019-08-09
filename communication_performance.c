#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{
  int rank, proc;
  double start_time, stop_time;
  double *send, *recv;
  int tag = 100;
  int i, size;

  MPI_Init(&argc, &argv);
  for (size = 1; size < pow(10, 10); size *= 10)
  {
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc);

    int src = MPI_ANY_SOURCE;
    MPI_Status status;

    send = (double *)malloc(sizeof(double) * size);
    recv = (double *)malloc(sizeof(double) * size);

    for (i = 0; i < size; i++)
      send[i] = 1.0;

    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    if (rank > 0)
    {
      int dest = 0;
      MPI_Send(send, size, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
    }
    else
    {
      MPI_Recv(recv, size, MPI_DOUBLE, src, tag, MPI_COMM_WORLD, &status);
    }

    stop_time = MPI_Wtime();

    if (rank == 0)
      printf("%d,%f\n", size * sizeof(double), stop_time - start_time);
  }
  MPI_Finalize();
  return 0;
}
