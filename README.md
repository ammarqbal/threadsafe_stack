# THREADSAFE_STACK

This is a simple thread-safe stack implementation in C++ using a `std::mutex` and `std::condition_variable`

## FEATURES

- [✅] Thread-safe `push`
- [✅] Thread-safe `try_pop(T&)`
- [✅] Thread-safe `try_pop()` returning `std::optional<T>`
- [✅] The stack is non-copyable and non-movable

## TO BE IMPLEMENTED

- [✅] `wait_and_pop()` using `std::condition_variable`
- [] Add timeout support
- [] Add tests

## Build

```bash
mkdir build
cd build
cmake ..
cmake --build .
```
