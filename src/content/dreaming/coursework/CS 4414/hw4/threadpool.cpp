#include "threadpool.hpp"

ThreadPool::ThreadPool(size_t threads) : stop(false) {
  for (size_t i = 0; i < threads; ++i) {
    workers.emplace_back([this]() {
      std::function<void()> task;
      while (true) {
        {
          std::unique_lock<std::mutex> lock(queueMutex);
          condition.wait(lock, [this]() { return stop || !tasks.empty(); });
          if (stop && tasks.empty()) return;
          task = std::move(tasks.front());
          tasks.pop();
        }
        task();
      }
    });
  }
}

ThreadPool::~ThreadPool() {
  stop = true;
  condition.notify_all();
  for (std::thread &worker : workers) if (worker.joinable()) worker.join();
}

void ThreadPool::finish() {
  stop = true;
  condition.notify_all();
  for (std::thread &worker : workers) worker.join();
}


// Explicit template instantiation
template void ThreadPool::enqueue(std::function<void()>&&);