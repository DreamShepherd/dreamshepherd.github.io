#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadPool {
public:
  explicit ThreadPool(size_t threads);
  ~ThreadPool();

  template<class F>
  void enqueue(F&& f);
  void finish();
private:
  std::vector<std::thread> workers;
  std::queue<std::function<void()>> tasks;
  std::mutex queueMutex;
  std::condition_variable condition;
  bool stop = false;
};

template<class F>
void ThreadPool::enqueue(F&& f) {
  {
    std::unique_lock<std::mutex> lock(queueMutex);
    tasks.emplace(std::forward<F>(f));
  }
  condition.notify_one();
}