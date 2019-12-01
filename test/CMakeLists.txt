set(UNIT_TEST qmlLiveTest)

find_package(Qt5 COMPONENTS Core Test REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/lib")
add_executable(${UNIT_TEST} QmlLiveTest.cpp QmlLiveTest.h)
target_link_libraries(${UNIT_TEST} ${PROJECT_NAME} Qt5::Core Qt5::Test)
# add_custom_command(
    # TARGET ${UNIT_TEST}
    # COMMENT "Run tests"
    # POST_BUILD
    # COMMAND ${UNIT_TEST}
# )
