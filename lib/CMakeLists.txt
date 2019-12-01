
cmake_policy(SET CMP0043 NEW)

add_definitions(-DQT_NO_CAST_TO_ASCII
                -DQT_NO_CAST_FROM_ASCII
                -DQT_NO_URL_CAST_FROM_STRING
                -DQT_NO_CAST_FROM_BYTEARRAY
                -DQT_USE_QSTRINGBUILDER
                -DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT
                -DQT_STRICT_ITERATORS
                -DQT_DISABLE_DEPRECATED_BEFORE=0x060000
                -DQT_NO_FOREACH
               )
add_definitions(-DENABLE_SHARED_LIBRARY)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

qt5_add_resources(RESOURCES_QUICK ${CMAKE_CURRENT_SOURCE_DIR}/qml.qrc)

add_library(${PROJECT_NAME}
    SHARED
    "QmlLiveApplication.h"
    "QmlLiveApplication.cpp"
    "private/PathWatcher_p.h"
    "private/PathWatcher_p.cpp"
    "private/ImportPathConfig_p.h"
    "private/ImportPathConfig_p.cpp"
    "shared_export.h"
    ${RESOURCES_QUICK})
target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
        $<$<OR:$<CONFIG:Debug>,
        $<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Quick)
