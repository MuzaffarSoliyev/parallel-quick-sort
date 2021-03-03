#include <algorithm>
#include <cmath>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <fstream>

const int THREAD_COUT = 8;
// const int ARRAY_SIZE = 1000000;
using namespace std;

vector<bool> is_sort_parts(THREAD_COUT);
vector<bool> is_sort_parts1((int)(THREAD_COUT / 2));
vector<bool> is_sort_parts2((int)(THREAD_COUT / 4));

bool ret() {
  for (int i = 0; i < THREAD_COUT; i++) {
    if (is_sort_parts[i] == false) {
      return false;
    }
  }
  return true;
}
bool ret1() {
  for (int i = 0; i < (int)(THREAD_COUT / 2); i++) {
    if (is_sort_parts1[i] == false) {
      return false;
    }
  }
  return true;
}
bool ret2() {
  for (int i = 0; i < (int)(THREAD_COUT / 4); i++) {
    if (is_sort_parts2[i] == false) {
      return false;
    }
  }
  return true;
}

void print_array(vector<int>& arr, int n) {
  for (int i = 0; i < n; i++) {
    cout << arr[i] << " ";
  }
  cout << endl;
}

void generate_arr(vector<int>& arr) {
  generate(arr.begin(), arr.end(), []() { return rand() % 100; });
}

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

mutex mtx, mtx1, mtx2;
condition_variable cv, cv1, cv2;

void parallel_function(vector<int>& numbers, vector<pair<int, int>> steps,
                       int thread_index, int n) {
  quickSort(numbers, steps[thread_index].first, steps[thread_index].second);
  is_sort_parts[thread_index] = true;
  int iter = 0;
  while (iter < log2(n)) {
    if (thread_index % 2 == 0 && iter == 0) {
      unique_lock<mutex> ulm(mtx);
      cv.wait(ulm, ret);

      merge(numbers, steps[thread_index].first, steps[thread_index + 1].first,
            steps[thread_index].second, steps[thread_index + 1].second,
            steps[thread_index].second - steps[thread_index].first + 1,
            steps[thread_index + 1].second - steps[thread_index + 1].first + 1);
      is_sort_parts1[thread_index / 2] = true;
      // print_array(numbers, ARRAY_SIZE);
      // cout << "NNNN: " << thread_index << "---" <<
      // is_sort_parts1[thread_index/2] << endl;
    } else {
      cv.notify_one();
    }
    if (thread_index % 4 == 0 && iter == 1) {
      // cout << "THREAD ID: " << thread_index << endl;
      // cout << "NNNN: " << thread_index << "---" <<
      // is_sort_parts[thread_index] << endl;
      unique_lock<mutex> ulm1(mtx1);
      cv1.wait(ulm1, ret1);
      merge(numbers, steps[thread_index].first, steps[thread_index + 2].first,
            steps[thread_index + 1].second, steps[thread_index + 3].second,
            steps[thread_index + 1].second - steps[thread_index].first + 1,
            steps[thread_index + 3].second - steps[thread_index + 2].first + 1);
      is_sort_parts2[thread_index / 4] = true;
      // cout << "NNNN: " << thread_index << "---" <<
      // is_sort_parts2[thread_index/4] << endl; print_array(numbers,
      // ARRAY_SIZE);
    } else {
      cv1.notify_one();
    }
    if (thread_index % 8 == 0 && iter == 2) {
      // cout << "THREAD ID: " << thread_index << endl;
      // cout << "NNNN: " << thread_index << "---" <<
      // is_sort_parts[thread_index] << endl;
      unique_lock<mutex> ulm2(mtx2);
      cv2.wait(ulm2, ret2);
      merge(numbers, steps[thread_index].first,
            steps[thread_index + 2 * 2].first,
            steps[thread_index + 2 * 1 + 1].second,
            steps[thread_index + 2 * 3 + 1].second,
            steps[thread_index + 2 * 2 + 1].second - steps[thread_index].first +
                1,
            steps[thread_index + 2 * 3 + 1].second -
                steps[thread_index + 2 * 2].first + 1);
      // cout << "NNNN: " << thread_index << "---" <<
      // is_sort_parts[thread_index] << endl; print_array(numbers, ARRAY_SIZE);
    } else {
      cv2.notify_one();
    }

    iter++;
  }
}

void show_parts() {
  for (int i = 0; i < is_sort_parts.size(); i++) {
    cout << is_sort_parts[i] << " ";
  }
  cout << endl;
  for (int i = 0; i < is_sort_parts1.size(); i++) {
    cout << is_sort_parts1[i] << " ";
  }
  cout << endl;
  for (int i = 0; i < is_sort_parts2.size(); i++) {
    cout << is_sort_parts2[i] << " ";
  }
  cout << endl;
}

void change_to_zeros() {
  for (int i = 0; i < is_sort_parts.size(); i++) {
    is_sort_parts[i] = false;
  }
  for (int i = 0; i < is_sort_parts1.size(); i++) {
    is_sort_parts1[i] = false;
  }
  for (int i = 0; i < is_sort_parts2.size(); i++) {
    is_sort_parts2[i] = false;
  }
}

int main() {
  int ARRAY_SIZE = 1000;
  ofstream out;
  out.open("out.txt", ios_base::app);
  out << "THREAD COUNT: " << THREAD_COUT << endl;
  out << "Elements amount:\t"
      << "TIME PARALLEL:\t"
      << "IS SORTED:\t"
      << "SINGLE SORT TIME:\t"
      << "IS SORTED:" << endl;
  for (int i = 0; i < 5; i++) {
    // show_parts();
    int n;
    n = THREAD_COUT;
    vector<int> arr(ARRAY_SIZE);
    vector<int> arr_1(ARRAY_SIZE);
    generate_arr(arr);
    for (int i = 0; i < ARRAY_SIZE; i++) {
      arr_1[i] = arr[i];
    }

    int step = ARRAY_SIZE / n;

    vector<pair<int, int>> steps;
    for (int i = 0; i < n; i++) {
      steps.push_back({i * step, (i + 1) * step - 1});
    }
    steps[n - 1].second = ARRAY_SIZE - 1;
    thread* ths = new thread[n];

    clock_t start, finish;
    
    /*-------------PARALLEL SORT-------------*/

    start = clock();

    for (int i = 0; i < n; i++) {
      ths[i] = thread(parallel_function, ref(arr), ref(steps), i, n);
    }
    for (int i = 0; i < n; i++) {
      ths[i].join();
    }
    finish = clock();
    out << ARRAY_SIZE << "\t\t\t\t";
    out << (float)(finish - start) / CLOCKS_PER_SEC << "\t\t\t";
    out << is_sorted(arr.begin(), arr.end()) << "\t\t\t";
    
    /*-------------SINGLE SORT-------------*/

    start = clock();
    quickSort(arr_1, 0, ARRAY_SIZE - 1);
    finish = clock();
    out << (float)(finish - start) / CLOCKS_PER_SEC << "\t\t\t\t"; 
    out << is_sorted(arr_1.begin(), arr_1.end()) << "\t\t";
    ARRAY_SIZE *= 10;
    change_to_zeros();
    out << endl;
  }
  out << endl;
  return 0;
}
