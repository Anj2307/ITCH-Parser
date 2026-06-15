cmake_minimum_required(VERSION 3.10)
project(nasdaq_itchparsing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

include_directories(include)

add_executable(itch_parser
    src/main.cpp
    src/file_reader.cpp
    src/decoder.cpp
    src/order_book.cpp
    src/indicators.cpp
    src/trends.cpp
    src/patterns.cpp
    src/signals.cpp
)

target_link_libraries(itch_parser ws2_32)