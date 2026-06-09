cmake_minimum_required(VERSION 3.10)
project(nasdaq_itchparsing)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(itch_parser
    src/main.cpp
    src/file_reader.cpp
    src/decoder.cpp
    src/order_book.cpp
)

target_link_libraries(itch_parser ws2_32)