#pragma once
#include <condition_variable>
#include <ctime>
#include <iostream>
#include <mutex>
#include <queue>
#include <set>
#include <thread>
#include <vector>

using namespace std;

template <typename T>
void print(const vector<T> &arr) {
  for (size_t i = 0; i < arr.size(); i++) cout << arr[i] << " ";
  cout << endl;
}

queue<pair<int, int> > tasks;
mutex q_mutex, s_mutex;
condition_variable cv;
set<int> ss;

template <typename T>
int partition(vector<T> &arr, int l, int r) {
  T tmp = arr[r];  // as pivot element
  int i = l - 1;

  for (int j = l; j <= r - 1; j++)
    if (arr[j] < tmp) {
      i++;
      swap(arr[i], arr[j]);
    }

  swap(arr[i + 1], arr[r]);
  i++;
  return i;
}

template <typename T>
void quick_sort(vector<T> &arr) {
  while (true) {
    unique_lock<mutex> u_lock(q_mutex);

    if (ss.size() == arr.size()) return;

    if (tasks.size() > 0) {
      pair<int, int> cur_task = tasks.front();
      tasks.pop();

      int l = cur_task.first, r = cur_task.second;

      if (l < r) {
        int q = partition(arr, l, r);  // split array

        s_mutex.lock();
        ss.insert(q);
        ss.insert(l);
        ss.insert(r);
        s_mutex.unlock();

        tasks.push(make_pair(l, q - 1));
        tasks.push(make_pair(q + 1, r));

        cv.notify_one();
      }
    } else
      cv.wait(u_lock);
  }
}