cmake_minimum_required(VERSION 3.16)

project(Worksheet6 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# IMPORTANT: only request what Exercise 3/4 needs (NO OpenVR)
find_package(VTK REQUIRED COMPONENTS
  CommonCore
  CommonDataModel
  FiltersSources
  IOGeometry
  RenderingCore
  RenderingOpenGL2
  InteractionStyle
  GUISupportQt
)

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
  optiondialog.h
  optiondialog.cpp
  optiondialog.ui
  ${QRC_SOURCES}
)

target_link_libraries(Worksheet6 PRIVATE
  Qt6::Widgets
  ${VTK_LIBRARIES}
)

# VTK9+ needs this
vtk_module_autoinit(
  TARGETS Worksheet6
  MODULES ${VTK_LIBRARIES}
)
