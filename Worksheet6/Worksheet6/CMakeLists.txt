cmake_minimum_required(VERSION 3.16)

project(Worksheet6 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_resources(QRC_SOURCES resources.qrc)

qt_add_executable(Worksheet6
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ModelPart.cpp
    ModelPart.h
    ModelPartList.cpp
    ModelPartList.h
    ${QRC_SOURCES}
)

target_link_libraries(Worksheet6 PRIVATE Qt6::Widgets)
