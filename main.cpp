#include <algorithm>
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <algorithm>
#include <ctime>
#include "qsort.h"

using namespace std;

//const int ARRAY_SIZE = 100000;

void generate_arr(vector<int>& arr) {
  generate(arr.begin(), arr.end(), []() { return rand() % 100; });
}

void parallel_sort(vector<int>& arr, int n, int ARRAY_SIZE) {

  //cout << "COUNT OF THREAD: " << n << endl;
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
  /*
  cout << "Quicksort: " << endl;

  for (auto& elem : arr) {
    cout << elem << " ";
  }

  cout << endl;*/
  if (n == 2) {
    ths[0] =
        thread(merge, ref(arr), steps[0].first, steps[1].first, steps[0].second,
               steps[1].second, steps[0].second - steps[0].first + 1,
               steps[1].second - steps[1].first + 1, 0);
    ths[0].join();
  } else if (n == 4) {
    ths[0] =
        thread(merge, ref(arr), steps[0].first, steps[1].first, steps[0].second,
               steps[1].second, steps[0].second - steps[0].first + 1,
               steps[1].second - steps[1].first + 1, 0);
    ths[1] =
        thread(merge, ref(arr), steps[2].first, steps[3].first, steps[2].second,
               steps[3].second, steps[2].second - steps[2].first + 1,
               steps[3].second - steps[3].first + 1, 1);

    ths[0].join();
    ths[1].join();

    ths[0] =
        thread(merge, ref(arr), steps[0].first, steps[2].first, steps[1].second,
               steps[3].second, steps[1].second - steps[0].first + 1,
               steps[3].second - steps[2].first + 1, 0);
    ths[0].join();

  } else if (n == 8) {
    ths[0] =
        thread(merge, ref(arr), steps[0].first, steps[1].first, steps[0].second,
               steps[1].second, steps[0].second - steps[0].first + 1,
               steps[1].second - steps[1].first + 1, 0);
    ths[1] =
        thread(merge, ref(arr), steps[2].first, steps[3].first, steps[2].second,
               steps[3].second, steps[2].second - steps[2].first + 1,
               steps[3].second - steps[3].first + 1, 1);
    ths[2] =
        thread(merge, ref(arr), steps[4].first, steps[5].first, steps[4].second,
               steps[5].second, steps[4].second - steps[4].first + 1,
               steps[5].second - steps[5].first + 1, 2);
    ths[3] =
        thread(merge, ref(arr), steps[6].first, steps[7].first, steps[6].second,
               steps[7].second, steps[6].second - steps[6].first + 1,
               steps[7].second - steps[7].first + 1, 3);

    ths[0].join();
    ths[1].join();
    ths[2].join();
    ths[3].join();

    ths[0] =
        thread(merge, ref(arr), steps[0].first, steps[2].first, steps[1].second,
               steps[3].second, steps[1].second - steps[0].first + 1,
               steps[3].second - steps[2].first + 1, 0);
    ths[1] =
        thread(merge, ref(arr), steps[4].first, steps[6].first, steps[5].second,
               steps[7].second, steps[5].second - steps[4].first + 1,
               steps[7].second - steps[6].first + 1, 1);
    ths[0].join();
    ths[1].join();

    ths[0] =
        thread(merge, ref(arr), steps[0].first, steps[4].first, steps[3].second,
               steps[7].second, steps[3].second - steps[0].first + 1,
               steps[7].second - steps[4].first + 1, 0);
    ths[0].join();
  }
}

int main() {
  int n;
  cin >> n;
    int ARRAY_SIZE = 1000000;
  for (int i = 0; i < 1; i++) {
      cout << "AMOUNT OF NUMBERS: " << ARRAY_SIZE << endl;
    vector<int> arr(ARRAY_SIZE);
    vector<int> arr_1(ARRAY_SIZE);
    generate_arr(arr);
    for (int i = 0; i < ARRAY_SIZE; i++) {
      arr_1[i] = arr[i];
    }
    /*cout << "Before: " << endl;
    for (auto& elem : arr) {
      cout << elem << " ";
    }
    cout << endl;*/
    clock_t start, finish;

    // PARALLEL SORT
    start = clock();
    parallel_sort(arr, n, ARRAY_SIZE);
    finish = clock();
    cout << i << " - " << "TIME PARALLEL: " << (float)(finish - start) / CLOCKS_PER_SEC
         << endl;
    cout << i << " - " << "IS SORTED: " << is_sorted(arr.begin(), arr.end()) << endl;

    // SORT
    start = clock();
    quickSort(arr_1, 0, ARRAY_SIZE - 1);
    finish = clock();
    cout << i << " - "
         << "TIME: " << (float)(finish - start) / CLOCKS_PER_SEC << endl;
    cout << i << " - "
         << "IS SORTED: " << is_sorted(arr_1.begin(), arr_1.end()) << endl;
    ARRAY_SIZE *= 10;
    cout << endl;
    cout << endl;
  }

  /*int iter = n / 2;
  int iter_n = 2;
  int val = 2;
  mutex mx;
  while (iter >= 1) {
    for (int i = 0, j = 0; i < iter; i++, j += 2) {
      ths[i] = thread(merge, ref(arr), steps[j].first, steps[j + 1].first,
                      steps[j].second, steps[j + 1].second,
                      steps[j].second - steps[j].first + 1,
                      steps[j + 1].second - steps[j + 1].first + 1, i);
    }
    for (int i = 0; i < iter; i++) {
      ths[i].join();
    }
    val *= 2;
    iter /= 2;
  }
  */


  /*cout << "Merge: " << endl;

  for (auto& elem : arr) {
    cout << elem << " ";
  }
  cout << endl;*/

  //cout << "IS SORTED: " << is_sorted(arr.begin(), arr.end()) << endl;
  return 0;
}