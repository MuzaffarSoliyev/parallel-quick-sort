#pragma once
#include <iostream>
#include <vector>
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

