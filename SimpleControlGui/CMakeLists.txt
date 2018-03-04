project(SimpleControlGui)
cmake_minimum_required(VERSION 3.3)

set (CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core REQUIRED)

set (SRC_LIST ${SRC_LIST} mainwindow.ui mainwindow.cpp main.cpp calibrationdialog.cpp calibrationdialog.ui
                          calibrationdata.cpp homeposition.cpp robotcontroller.cpp vector2d.cpp matrix2d.cpp
                          kinematics.cpp calibrationadjustmentdialog.cpp calibrationadjustmentdialog.ui
                          calibrationresultsdialog.cpp calibrationresultsdialog.ui calibrationwizard.cpp
                          chessboard.cpp calibrationresultsgraph.cpp matrix.cpp leastsquares.cpp calibrationdonedialog.cpp
                          calibrationdonedialog.ui squarecalibrator.cpp piecemover.cpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})

qt5_use_modules(${PROJECT_NAME} Core Widgets SerialPort)

