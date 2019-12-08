// Copyright 2019 Guschin Alexander
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <string>
#include "../../../modules/task_3/guschin_a_radix_sort_s_m/radix_sort_s_m.h"

TEST(radix_sort, placeholder) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::vector<std::int64_t> vec;

  if (rank == 0) {
    vec.resize(1000);
    Fill_random(&vec[0], vec.size());
    std::vector<std::int64_t> res(Radix_sort(vec));

    bool is_sort = true;
    for (int i = 1; i < vec.size(); ++i)
      if (res[i] < res[i - 1]) {
        is_sort = false;
        break;
      }
    EXPECT_EQ(is_sort, 1);
  }
}

TEST(radix_sort, placeholder2) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::vector<std::int64_t> vec;

  if (rank == 0) {
    vec.resize(1000);
    Fill_random(&vec[0], vec.size());
  }
  std::vector<std::int64_t> res(P_radix_sort(vec));
  if (rank == 0) {
    bool is_sort = true;
    for (int i = 1; i < vec.size(); ++i)
      if (res[i] < res[i - 1]) {
        is_sort = false;
        break;
      }

    EXPECT_EQ(is_sort, 1);
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
