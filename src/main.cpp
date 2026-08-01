#include <iostream>
#include <mutex>
#include <thread>
#include <threadsafe_stack.hpp>
#include <vector>
int main() {
  threadsafe_stack<int> my_stack;
  std::thread producer([&my_stack]() {
    for (int i{0}; i < 8; i++) {
      my_stack.push(-1);
    }

    for (int i{0}; i < 1000000; i++) {
      my_stack.push(i);
    }
  });
  producer.join();

  std::mutex ctm;
  int count{0};
  std::vector<std::thread> workers;
  for (int i{0}; i < 8; i++) {
    workers.push_back(std::thread([&count, &ctm, &my_stack]() {
      int value{0};
      while (true) {
        my_stack.wait_and_pop(value);
        if (value == -1) {
          return;
        }
        {
          std::lock_guard<std::mutex> lk(ctm);
          count++;
        }
      }
    }));
  }

  for (auto &worker : workers) {
    worker.join();
  }
  std::cout << count << '\n';
}
