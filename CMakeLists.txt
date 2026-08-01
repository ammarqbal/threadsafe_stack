cmake_minimum_required(VERSION 3.20)

project(threadsafe_stack
    LANGUAGES
    CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(SOURCES
    src/main.cpp
)
set(HEADERS
    include/threadsafe_stack.hpp
)
add_executable(threadsafe_stack
  ${SOURCES}
  ${HEADERS}
)
target_include_directories(threadsafe_stack
    PRIVATE
        include
)

set(SANITIZER "" CACHE STRING "Sanitizer to use (address, thread, undefined)")

if(SANITIZER AND NOT MSVC)
    target_compile_options(threadsafe_stack PRIVATE
        -fsanitize=${SANITIZER}
        -fno-omit-frame-pointer
        -g
    )

    target_link_options(threadsafe_stack PRIVATE
        -fsanitize=${SANITIZER}
    )
endif()

if(MSVC)
    target_compile_options(threadsafe_stack PRIVATE
        /W4
    )
else()
    target_compile_options(threadsafe_stack PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()
