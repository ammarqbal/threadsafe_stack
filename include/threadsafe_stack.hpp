#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <stack>
template <typename T> class threadsafe_stack {
public:
  threadsafe_stack() = default;
  ~threadsafe_stack() = default;

  threadsafe_stack(const threadsafe_stack &copy) = delete;
  threadsafe_stack &operator=(const threadsafe_stack &copy) = delete;

  threadsafe_stack(threadsafe_stack &&other) = delete;
  threadsafe_stack &operator=(threadsafe_stack &&other) = delete;

  void push(T value);
  bool try_pop(T &value);
  [[nodiscard]] std::optional<T> try_pop();
  void wait_and_pop(T &value);
  bool empty() const;

private:
  std::stack<T> data_;
  mutable std::mutex m_;
  std::condition_variable cv_;
};

template <typename T> void threadsafe_stack<T>::push(T value) {
  {
    std::lock_guard<std::mutex> lk(m_);
    data_.push(std::move(value));
  }

  cv_.notify_one();
}

template <typename T> bool threadsafe_stack<T>::try_pop(T &value) {
  std::lock_guard<std::mutex> lk(m_);
  if (data_.empty()) {
    return false;
  }
  value = std::move(data_.top());
  data_.pop();
  return true;
}

template <typename T> std::optional<T> threadsafe_stack<T>::try_pop() {
  std::lock_guard<std::mutex> lk(m_);
  if (data_.empty()) {
    return std::nullopt;
  }
  T value = std::move(data_.top());
  data_.pop();
  return value;
}

template <typename T> void threadsafe_stack<T>::wait_and_pop(T &value) {
  std::unique_lock<std::mutex> lk(m_);
  cv_.wait(lk, [this]() { return !data_.empty(); });
  value = std::move(data_.top());
  data_.pop();
}

template <typename T> bool threadsafe_stack<T>::empty() const {
  std::lock_guard<std::mutex> lk(m_);
  return data_.empty();
}
