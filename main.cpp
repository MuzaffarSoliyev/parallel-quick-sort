#include <chrono>
#include <iostream>
#include <thread>
#include <algorithm>
#include <atomic>
#include "qsort.h"

using namespace std;

const int ARRAY_SIZE = 20;


void generate_arr(vector<int> &arr) {
  generate( arr.begin(), arr.end(), []() { return rand() % 10;
      });
}

struct MyStruct {
  atomic<int> mid;
};

//test git

class TestClass
{
	int right;
	int left;
	TestClass(int first, int second) : right(first), left(second){}
};

int main() {
  vector<int> arr(ARRAY_SIZE);
  generate_arr(arr);
  int n;
  cin >> n;
  int step = ARRAY_SIZE / n;
  vector<pair<int, int>> steps;
  for (int i = 0; i < n; i++) {
    steps.push_back({i*step, (i + 1)*step - 1});
  }
  thread *ths = new thread[n];
  for (int i = 0; i < n; i++) {
    ths[i] = thread(quickSort, ref(arr), steps[i].first, steps[i].second);
  }

  for (int i = 0; i < n; i++) {
    ths[i].join();
  }

  for (auto& elem: arr) {
    cout << elem << " ";
  }
  cout << endl;
  return 0;
}