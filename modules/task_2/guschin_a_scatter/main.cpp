// Copyright 2019 Guschin Alexander
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <string>
#include "../../../modules/task_2/guschin_a_scatter/scatter.h"

TEST(scatter, throw_when_different_size) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int *p, *d;
  int dest[3];
  ASSERT_ANY_THROW(MPI_Scatter_custom(p, 1, MPI_INT, &dest[0], 99, MPI_INT, 0, MPI_COMM_WORLD));
}

TEST(scatter, can_scatter_and_gather) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int *p, *d;
  int dest[3];
  if (rank == 0) {
    p = new int[3 * size];
    d = new int[3 * size];
    for (int i = 0; i < 3 * size; ++i) p[i] = i;
  }
  MPI_Scatter_custom(p, 3, MPI_INT, &dest[0], 3, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Gather(&dest[0], 3, MPI_INT, d, 3, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    int isEq = 1;
    for (int i = 0; i < 3 * size; ++i)
      if (d[i] != i) isEq = 0;
    EXPECT_EQ(isEq, 1);
  }
}

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
