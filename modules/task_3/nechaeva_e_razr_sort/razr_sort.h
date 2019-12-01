// Copyright 2019 Nechaeva Ekaterina
#ifndef MODULES_TASK_3_NECHAEVA_E_RAZR_SORT_RAZR_SORT_H_
#define MODULES_TASK_3_NECHAEVA_E_RAZR_SORT_RAZR_SORT_H_

#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>

void RazrSort(std::vector<double> src, std::vector<double> &dest, int byte, int size);
void RazrSortLast(std::vector<double> src, std::vector<double> &dest, int byte, int size);
void DoubleSortWin(std::vector<double> &src, int size);
void DoubleSortLin(std::vector<double> src, int size);
void Rand(std::vector<double> &mas, int size);
bool Tru(std::vector<double> mas, int size);
int ParallSort(std::vector<double> &src, int size);
void Merge(std::vector<double> &mas1, std::vector<double> mas2, int size1, int size2);

#endif  // MODULES_TASK_3_NECHAEVA_E_RAZR_SORT_RAZR_SORT_H_
