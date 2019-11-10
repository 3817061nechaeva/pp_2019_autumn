// Copyright 2019 Zhivaev Artem
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./scatter.h"


TEST(MPI_Scatter, int_to_int) {
  const int root = 0;
  int rank, size;
  int k = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> buf(1000);
  std::vector<int> buf1(1000);
  std::vector<int> buf2(1000);
  std::vector<int> recv;
  std::vector<int> scatter;

  for (int& i : buf) {
    i = ++k;
  }

  recv.resize(buf.size() / size);
  scatter.resize(recv.size());

  Scatter(buf.data(), buf.size() / size, MPI_INT, recv.data(), buf.size() / size, MPI_INT, root, MPI_COMM_WORLD);
  double t = MPI_Wtime();
  MPI_Scatter(buf.data(), buf.size() / size, MPI_INT, scatter.data(), buf.size() / size, MPI_INT, root, MPI_COMM_WORLD);
  t = MPI_Wtime() - t;
  std::cout << "WORK NOT MY ALGORITM=" << t << std::endl;

  MPI_Gather(recv.data(), recv.size(), MPI_INT,
    buf1.data(), recv.size(), MPI_INT, root, MPI_COMM_WORLD);
  MPI_Gather(scatter.data(), scatter.size(), MPI_INT,
    buf2.data(), scatter.size(), MPI_INT, root, MPI_COMM_WORLD);

  if (rank == 0) {
    ASSERT_EQ(buf1,buf2);
  }
}

//TEST(MPI_Scatter, throw_when_process_more_then_elements) {
//  const int root = 0;
//  int* send;
//  send = new int[1];
//  int* recv;
//
//  recv = new int[1];
//
//  for (int i = 0; i < 1; i++)
//    send[i] = i + 1;
//
//  ASSERT_ANY_THROW(Scatter(send, 1, MPI_INT, recv, 1, MPI_INT, root, MPI_COMM_WORLD));
//}
//
//TEST(MPI_Scatter, throw_when_part_of_elements_more_then_elements) {
//  const int root = 0;
//  int* send;
//  send = new int[2];
//  int* recv;
//
//  recv = new int[1];
//
//  for (int i = 0; i < 2; i++)
//    send[i] = i + 1;
//
//  ASSERT_ANY_THROW(Scatter(send, 2, MPI_INT, recv, 2, MPI_INT, root, MPI_COMM_WORLD));
//}
//
//TEST(MPI_Scatter, double_to_int) {
//  const int root = 0;
//  int rank, size;
//  int k = 1.2;
//  MPI_Comm_size(MPI_COMM_WORLD, &size);
//  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//
//  std::vector<double> buf(12);
//  std::vector<int> buf1(12 * 2);
//  std::vector<int> buf2(12 * 2);
//  std::vector<int> recv;
//  std::vector<int> scatter;
//
//  for (double& i : buf) {
//    i = ++k;
//  }
//
//  recv.resize((buf.size() / size)*(sizeof(double) / sizeof(int)));
//  scatter.resize(recv.size());
//
//  Scatter(buf.data(), buf.size() / size, MPI_DOUBLE, recv.data(), recv.size(), MPI_INT, root, MPI_COMM_WORLD);
//  MPI_Scatter(buf.data(), buf.size() / size, MPI_DOUBLE, scatter.data(), recv.size(), MPI_INT, root, MPI_COMM_WORLD);
//
//  MPI_Gather(recv.data(), recv.size(), MPI_INT,
//    buf1.data(), recv.size(), MPI_INT, root, MPI_COMM_WORLD);
//  MPI_Gather(scatter.data(), scatter.size(), MPI_INT,
//    buf2.data(), scatter.size(), MPI_INT, root, MPI_COMM_WORLD);
//  if (rank == 0) {
//    ASSERT_EQ(buf1, buf2);
//  }
//}
//TEST(MPI_Scatter, int_to_char) {
//  const int root = 0;
//  int rank, size;
//  int k = 1.2;
//  MPI_Comm_size(MPI_COMM_WORLD, &size);
//  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//
//  std::vector<int> buf(12);
//  std::vector<char> buf1(12*4);
//  std::vector<char> buf2(12 * 4);
//  std::vector<char> recv;
//  std::vector<char> scatter;
//
//  for (int& i : buf) {
//    i = ++k;
//  }
//
//  recv.resize((buf.size() / size)*(sizeof(int) / sizeof(char)));
//  scatter.resize(recv.size());
//
//  Scatter(buf.data(), buf.size() / size, MPI_INT, recv.data(), recv.size(), MPI_CHAR, root, MPI_COMM_WORLD);
//  double t = MPI_Wtime();
//  MPI_Scatter(buf.data(), buf.size() / size, MPI_INT, scatter.data(), recv.size(), MPI_CHAR, root, MPI_COMM_WORLD);
//  t = MPI_Wtime() - t;
//  std::cout << "WORK NOT MY ALGORITM=" << t<<std::endl;
//
//  MPI_Gather(recv.data(), recv.size(), MPI_CHAR,
//    buf1.data(), recv.size(), MPI_CHAR, root, MPI_COMM_WORLD);
//  MPI_Gather(scatter.data(), scatter.size(), MPI_CHAR,
//    buf2.data(), scatter.size(), MPI_CHAR, root, MPI_COMM_WORLD);
//  if (rank == 0) {
//    ASSERT_EQ(buf1, buf2);
//  }
//}



int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners =
    ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
