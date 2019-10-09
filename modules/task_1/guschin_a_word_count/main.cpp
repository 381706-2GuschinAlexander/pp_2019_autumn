// Copyright 2019 Guschin Alexander
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <string>
#include "../../../modules/task_1/guschin_a_word_count/word_count.h"

TEST(word_count, sequental) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::string st = "MPI is live ";
  if (rank == 0) {
    int res = getLinearCount(st, 0, st.size());
    ASSERT_EQ(res, 3);
  }
}

TEST(word_count, parallel) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::string st = "MPI is live ";
  int res = getCount(st);

  if (rank == 0) {
    ASSERT_EQ(res, 3);
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
