// Copyright 2019 Nechaeva Ekaterina
#include <mpi.h>
#include <algorithm>
#include "../../../modules/task_2/nechaeva_e_scatter_fun/scatter.h"

int Scatter(void* send_data, int send_count, MPI_Datatype send_datatype, void* recv_data,
  int recv_count, MPI_Datatype recv_datatype, int root, MPI_Comm communicator) {
  int size, rank;
  int elemSizer, elemSizes;
  MPI_Comm_size(communicator, &size);
  MPI_Comm_rank(communicator, &rank);

  MPI_Type_extent(send_datatype, &elemSizes);
  MPI_Type_extent(recv_datatype, &elemSizer);
  int k = _msize(send_data) / elemSizes;
  if (k < size || k < size*send_count)
    throw - 1;
  double t = MPI_Wtime();
  if (rank == root) {
    memcpy(recv_data, reinterpret_cast<char*>(send_data) + root * send_count * elemSizer, send_count * elemSizes);
    for (int i = 0; i < size; i++) {
      if (i == root) {
        continue;
      } else {
        MPI_Send(reinterpret_cast<char*>(send_data) + i * send_count * elemSizes,
          send_count, send_datatype, i, 0, communicator);
      }
    }
  } else {
    MPI_Status status;
    MPI_Recv(recv_data, recv_count, recv_datatype, root, 0, communicator, &status);
  }
  t = MPI_Wtime() - t;
  std::cout << "WORK MY ALGORITM= " << t;
  return MPI_SUCCESS;
}
