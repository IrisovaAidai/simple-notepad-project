cmake_minimum_required(VERSION 3.16)
project(Notepad VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets PrintSupport)

set(SOURCES
        main.cpp

)

set(HEADERS
        main_window.h
)

set(UI_FILES
        main.cpp

)

qt6_wrap_ui(UI_HEADERS ${UI_FILES})

add_executable(Notepad
        ${SOURCES}
        ${HEADERS}
        ${UI_FILES}
)

target_link_libraries(Notepad PRIVATE Qt6::Widgets Qt6::PrintSupport)

target_include_directories(Notepad PRIVATE ${CMAKE_CURRENT_BINARY_DIR})