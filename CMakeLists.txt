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
        main_window.cpp
        spell_checker.cpp
        spell_checker_highlighter.cpp
        find_replace_dialog.cpp
        word_frequency_dialog.cpp
)

set(HEADERS
        main_window.h
        text_transform.h
        notepad_exception.h
        spell_checker.h
        spell_checker_highlighter.h
        sort.h
        line_number_area.h
        find_replace_dialog.h
        word_frequency_dialog.h
)

set(UI_FILES
        word_frequency_dialog.ui
        find_replace_dialog.ui
)

add_executable(Notepad
        ${SOURCES}
        ${HEADERS}
        ${UI_FILES}
)

target_link_libraries(Notepad PRIVATE Qt6::Widgets Qt6::PrintSupport)

target_include_directories(Notepad PRIVATE ${CMAKE_CURRENT_BINARY_DIR})