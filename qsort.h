#pragma once
#include <iostream>
#include <vector>
#include <mutex>
using namespace std;



int partition(vector<int>& numbers, int left, int right) {
  int pivot = numbers[right];
  int i = left - 1;

  for (int j = left; j <= right - 1; j++) {
    if (numbers[j] < pivot) {
      i++;
      swap(numbers[i], numbers[j]);
    }
  }
  swap(numbers[i + 1], numbers[right]);
  i++;
  return i;
}

void quickSort(vector<int>& numbers, int left, int right) {
  
    if (left < right) {
    int middle = partition(numbers, left, right);

    quickSort(numbers, left, middle - 1);
    quickSort(numbers, middle + 1, right);
  }
}
mutex mux;

void merge(vector<int>& arr_1, int start_1, int start_2, int end_1, int end_2,
           int n, int m) {
  
  vector<int> merged_array(n + m);
  int j = 0, i = 0;

  while (i + j < n + m) {
    if (j == m || (i < n && arr_1[start_1 + i] < arr_1[start_2 + j])) {
      merged_array[i + j] = arr_1[start_1 + i];
      i++;
    } else {
      merged_array[i + j] = arr_1[start_2 + j];
      j++;
    }
  }
  for (int i = start_1, j = 0; i <= end_2; i++, j++) {
    arr_1[i] = merged_array[j];
  }
}
