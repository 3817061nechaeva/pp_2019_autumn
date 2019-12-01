// Copyright 2019 Nechaeva Ekaterina
#include <mpi.h>
#include <random>
#include "../../../modules/task_3/nechaeva_e_razr_sort/razr_sort.h"

void RazrSort(std::vector<double> src, std::vector<double> &dest, int byte, int size) {
  unsigned char *mas = (unsigned char *)src.data();
  int counter[256] = { 0 };
  int tek = 0;

  for (int i = 0; i < size; i++)
    counter[mas[8 * i + byte]]++;

  for (int j = 0; j < 256; j++) {
    int b = counter[j];
    counter[j] = tek;
    tek += b;
  }

  for (int i = 0; i < size; i++) {
    dest[counter[mas[8 * i + byte]]] = src[i];
    counter[mas[8 * i + byte]]++;
  }
}

void RazrSortLast(std::vector<double> src, std::vector<double> &dest, int byte, int size) {
  unsigned char *mas = (unsigned char *)src.data();
  int counter[256] = { 0 };
  int tek = 0;

  for (int i = 0; i < size; i++)
    counter[mas[8 * i + byte]]++;

  for (int j = 255; j >= 128; j--) {
    int b = counter[j];
    counter[j] += tek;
    tek = counter[j];
  }
  for (int j = 0; j < 128; j++) {
    int b = counter[j];
    counter[j] = tek;
    tek += b;
  }
  for (int i = 0; i < size; i++) {
    if (mas[8 * i + byte] < 128) {
      dest[counter[mas[8 * i + byte]]] = src[i];
      counter[mas[8 * i + byte]]++;
    } else {
      counter[mas[8 * i + byte]]--;
      dest[counter[mas[8 * i + byte]]] = src[i];
    }
  }
}

void DoubleSortWin(std::vector<double> &src, int size) {
  std::vector<double> dest(size);

  for (int i = 0; i < 7; i++) {
    RazrSort(src, dest, i, size);
    std::swap(src, dest);
  }
  RazrSortLast(src, dest, 7, size);
  std::swap(src, dest);
}

void DoubleSortLin(std::vector<double> src, int size) {
  std::vector<double> dest(size);

  for (int i = 7; i > 0; i--) {
    RazrSort(src, dest, i, size);
    std::swap(src, dest);
  }
  RazrSort(dest, src, 0, size);
}

//void NameOC(std::vector<double> src, int size) {
//  const char* env_p = getenv("OSTYPE");
//  if (env_p == NULL)
//  {
//    env_p = getenv("windir");
//    if (env_p != NULL)
//      DoubleSortWin(src, size);
//  }
//  else if (strcmp(env_p, "linux") == 0)
//    DoubleSortLin(src, size);
//}

void Rand(std::vector<double> &mas, int size) {
  std::mt19937 generator;
  std::random_device device;
  generator.seed(device());
  std::uniform_real_distribution<double> distribution(10,100);

  for (int i = 0; i < size; i++) {
    double f = distribution(generator);
    mas[i] = f;
  }
}

bool Tru(std::vector<double> mas, int size) {
  int a = 0;
  for (int i = 0; i < size-1; i++) {
    if (mas[i] > mas[i + 1])
      a = 1;
    if (a == 1)
      return false;
  }
  return true;
}

void Merge(std::vector<double> &mas1, std::vector<double> mas2, int size1, int size2) {
  std::vector<double> temp(size1 + size2);
  int i = 0, j = 0;
  for (int k = 0; k < size1+size2; k++) {
    if (i > size1-1) {
      double a = mas2[j];
      temp[k] = a;
      j++;
    } else if (j > size2-1) {
      double a = mas1[i];
      temp[k] = a;
      i++;
    } else if (mas1[i] < mas2[j]) {
      double a = mas1[i];
      temp[k] = a;
      i++;
    } else {
      double b = mas2[j];
      temp[k] = b;
      j++;
    }
  }
  mas1 = temp;
}

int ParallSort(std::vector<double> &src, int size) {
  int nsize, nrank;
  MPI_Comm_size(MPI_COMM_WORLD, &nsize);
  MPI_Comm_rank(MPI_COMM_WORLD, &nrank);

  if (size < nsize)
    throw - 1;
  if (size <= 0)
    throw - 1;

  int n = size / nsize;
  int p = size % nsize;
  std::vector<double> dest;

  if (nrank == 0) {
    dest.resize(n + p);
  } else {
    dest.resize(n);
  }

  MPI_Scatter(src.data(), n, MPI_DOUBLE, dest.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  if (nrank == 0) {
    int a = size - p;
    for (int i = n; i < n + p; ++i)
      dest[i] = src[a++];
    DoubleSortWin(dest, n + p);
  } else {
    DoubleSortWin(dest, n);
  }

  int locsize = nsize;
  int i = 0;
  int patner;
  int k = n+p;
  while (locsize > 1) {
    if (locsize % 2 == 0) {
      if (nrank % (int)pow(2.0, i + 1) != 0) {
        patner = nrank - (int)pow(2.0, i);
        MPI_Send(dest.data(), n*(int)pow(2.0, i),
          MPI_DOUBLE, patner, 0, MPI_COMM_WORLD);
        return 0;
      }
      else {
        std::vector<double> temp(n*(int)pow(2.0, i));
        patner = nrank + (int)pow(2.0, i);
        MPI_Status status;
        MPI_Recv(temp.data(), n*(int)pow(2.0, i), MPI_DOUBLE, patner, 0, MPI_COMM_WORLD, &status);
        if (nrank == 0) {
          Merge(dest, temp, dest.size(), n*(int)pow(2.0, i));
          k = k + n * (int)pow(2.0, i);
        }
        else
          Merge(dest, temp, n*(int)pow(2.0, i), n*(int)pow(2.0, i));
      }
    }
    else {
      if (nrank % (int)pow(2.0, i + 1) != 0 && nrank != (locsize - 1)*(int)pow(2.0, i)) {
        patner = nrank - (int)pow(2.0, i);
        MPI_Send(dest.data(), n*(int)pow(2.0, i),
          MPI_DOUBLE, patner, 0, MPI_COMM_WORLD);
        return 0;
      }
      else if (nrank == (locsize - 1)*(int)pow(2.0, i)) {
        MPI_Send(dest.data(), n*(int)pow(2.0, i),
          MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        return 0;
      }
      else {
        if (nrank == 0) {
          std::vector<double> temp(n*(int)pow(2.0, i));
          patner = (locsize - 1)*(int)pow(2.0, i);
          MPI_Status status;
          MPI_Recv(temp.data(), n*(int)pow(2.0, i), MPI_DOUBLE, patner, 0, MPI_COMM_WORLD, &status);
          Merge(dest, temp, dest.size(), n*(int)pow(2.0, i));
          k = k + n * (int)pow(2.0, i);
        }
        std::vector<double> temp(n*(int)pow(2.0, i));
        patner = nrank + (int)pow(2.0, i);
        MPI_Status status;
        MPI_Recv(temp.data(), n*(int)pow(2.0, i), MPI_DOUBLE, patner, 0, MPI_COMM_WORLD, &status);
        if (nrank == 0) {
          Merge(dest, temp, dest.size(), n*(int)pow(2.0, i));
          k= k + n * (int)pow(2.0, i);
        }
        else
          Merge(dest, temp, n*(int)pow(2.0, i), n*(int)pow(2.0, i));
      }
    }
    locsize = locsize / 2;
    i++;
  }
  src = dest;
  return 0;
}
