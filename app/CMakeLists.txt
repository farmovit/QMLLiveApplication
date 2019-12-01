cmake_minimum_required(VERSION 3.1)

project(app_test LANGUAGES CXX)

add_definitions(-DENABLE_SHARED_LIBRARY)

include_directories("${CMAKE_SOURCE_DIR}/lib")
find_package(Qt5 COMPONENTS Core Quick REQUIRED)

add_executable(${PROJECT_NAME}
    "main.cpp")
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME} PRIVATE qmlliveapplication Qt5::Core Qt5::Quick)
