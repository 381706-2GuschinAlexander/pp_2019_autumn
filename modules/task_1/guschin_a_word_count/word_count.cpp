// Copyright 2019 Guschin Alexander
#include <mpi.h>
#include <string>
#include "../../../modules/task_1/guschin_a_word_count/word_count.h"

bool isLetter(char sym) {
  return (sym <= 'z' && sym >= 'a') || (sym <= 'Z' && sym >= 'A');
}

int getLinearCount(std::string st, int start, int end) {
  int count = 0;
  bool flag = 0;
  for (int i = start; i < end; i++) {
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

int getCount(std::string st) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int start, len, local_count = 0;
  int global_count;

  if (rank == 0) {
    start = 0;
    len = st.size() / size + st.size() % size;
  } else {
    start = rank * st.size() / size + st.size() % size;
    len = st.size() / size;
  }

  local_count = getLinearCount(st, start, start + len);

  if (rank < size && rank > 0)
    if (isLetter(st[start - 1]) && isLetter(st[start]))
      --local_count;

  MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  return global_count;
}
