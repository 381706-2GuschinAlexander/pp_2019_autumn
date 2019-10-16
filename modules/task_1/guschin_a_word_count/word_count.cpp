// Copyright 2019 Guschin Alexander
#include <mpi.h>
#include <string>
#include "../../../modules/task_1/guschin_a_word_count/word_count.h"

bool isLetter(char sym) {
  return (sym <= 'z' && sym >= 'a') || (sym <= 'Z' && sym >= 'A');
}

int getLinearCount(std::string st) {
  int count = 0;
  bool flag = 0;
  for (int i = 0; i < st.size(); i++) {
    if (isLetter(st[i]) == true) {
      flag = 1;
    } else {
      if (flag == true)
        count++;
      flag = false;
    }
  }

  if (flag == true)
    count++;

  return count;
}

int getCount(const std::string st, int st_size) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int start, len, local_count = 0;
  int global_count;
  int delta = st_size / size;
  int rem = st_size % size;

  if (rank == 0) {
    for (int i = 1; i < size; ++i) {
      MPI_Send(&st[0] + i * delta + rem, delta, MPI_CHAR, i, 0, MPI_COMM_WORLD);
    }
  }

  std::string local_st;
  if (rank == 0) {
    local_st = std::string(st.begin(), st.begin() + delta + rem);
  }
  else {
    MPI_Status status;
    MPI_Recv(&local_st[0], delta, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
  }

   local_count = getLinearCount(local_st);

  if (size < st_size && rank == 0) {
    for (int i = 1; i < size; ++i)
      if (isLetter(st[i * delta + rem - 1]) && isLetter(i * delta + rem)) 
        --local_count;
  }

  MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  return global_count;
}
