#include <iostream>
#include <thread>
#include <threadsafe_stack.hpp>
int main() {
  threadsafe_stack<int> my_stack;
  std::thread t1([&]() {
    for (int i{0}; i < 100000; i++) {
      my_stack.push(i);
    }
  });
  int count{0};
  int value{};
  t1.join();

  std::thread t2([&]() {
    while (my_stack.try_pop(value)) {
      count++;
    }
  });
  t2.join();
  std::cout << count << std::endl;
}
