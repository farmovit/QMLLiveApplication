cmake_minimum_required(VERSION 3.6)

project(
    qmlliveapplication
    VERSION 0.1
    DESCRIPTION "Application for QML live coding"
    HOMEPAGE_URL "https://github.com/farmovit/qmlliveapplication"
    LANGUAGES CXX)

cmake_policy(SET CMP0020 NEW)
cmake_policy(SET CMP0042 NEW)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib)
enable_testing()
add_subdirectory(test)
add_subdirectory(app)
