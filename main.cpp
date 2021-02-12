#include <algorithm>
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

#include "qsort.h"

using namespace std;

const int ARRAY_SIZE = 107;

void generate_arr(vector<int>& arr) {
  generate(arr.begin(), arr.end(), []() { return rand() % 100; });
}

int main() {
  vector<int> arr(ARRAY_SIZE);
  generate_arr(arr);
  cout << "Before: " << endl;
  for (auto& elem : arr) {
    cout << elem << " ";
  }
  cout << endl;
  int n;
  cin >> n;
  int step = ARRAY_SIZE / n;

  vector<pair<int, int>> steps;
  for (int i = 0; i < n; i++) {
    steps.push_back({i * step, (i + 1) * step - 1});
  }
  steps[n - 1].second = ARRAY_SIZE - 1;
  thread* ths = new thread[n];
  for (int i = 0; i < n; i++) {
    ths[i] = thread(quickSort, ref(arr), steps[i].first, steps[i].second);
  }
  for (int i = 0; i < n; i++) {
    ths[i].join();
  }

  cout << "Quicksort: " << endl;

  for (auto& elem : arr) {
    cout << elem << " ";
  }

  cout << endl;
  ths[0] =
      thread(merge, ref(arr), steps[0].first, steps[1].first, steps[0].second,
             steps[1].second, steps[0].second - steps[0].first + 1,
             steps[1].second - steps[1].first + 1);
  ths[1] =
      thread(merge, ref(arr), steps[2].first, steps[3].first, steps[2].second,
             steps[3].second, steps[2].second - steps[2].first + 1,
             steps[3].second - steps[3].first + 1);

  ths[0].join();
  ths[1].join();

  ths[0] =
      thread(merge, ref(arr), steps[0].first, steps[2].first, steps[1].second,
             steps[3].second, steps[1].second - steps[0].first + 1,
             steps[3].second - steps[2].first + 1);
  ths[0].join();

  /*int iter = n / 2;
  int iter_n = 2;
  int val = 2;

  while (iter >= 1) {
    for (int i = 0, j = 0; i < iter; i++, j += 2) {
      ths[i] = thread(merge, ref(arr), steps[j].first, steps[j + 1].first,
                      steps[j].second, steps[j + 1].second, step, step);
    }
    for (int i = 0; i < iter; i++) {
      ths[i].join();
    }
    val *= 2;
    iter /= 2;
  }

  */

  cout << "Merge: " << endl;

  for (auto& elem : arr) {
    cout << elem << " ";
  }
  cout << endl;
  return 0;
}