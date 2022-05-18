cmake_minimum_required(VERSION 3.16)
project(qtjack-examples
  DESCRIPTION "examples for qtjack"
  LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Jack REQUIRED)
find_package( Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package( qtjack REQUIRED)
find_package(Threads REQUIRED)
####### Attenuator #######
set(ATTENUATOR_SOURCES src/attenuator.cpp)

add_executable(attenuator)
target_sources(attenuator
  PRIVATE
  ${ATTENUATOR_SOURCES}
)
target_include_directories(attenuator SYSTEM
  PUBLIC ${JACK_INCLUDE_DIRS}
  qtjack::qtjack
)
target_link_libraries(attenuator PRIVATE
    qtjack::qtjack
    Qt5::Core
    ${JACK_LIBRARIES}
)

####### MP3 Player #######
set(MP3PLAYER_SOURCES
  src/mp3player.cpp
  src/mp3player_mainwindow.cpp
  include/mp3player_mainwindow.hpp)

QT5_WRAP_UI(MP3PLAYER_MAINWINDOW_UISrcs
  ${CMAKE_CURRENT_SOURCE_DIR}/src/mp3player_mainwindow.ui)

QT5_WRAP_CPP(MP3PLAYER_MOCrcs
  ${MP3PLAYER_SOURCES}
)

add_executable(mp3player
)
target_sources(mp3player
  PRIVATE
  ${MP3PLAYER_SOURCES}
  ${MP3PLAYER_MOCrcs}
  ${MP3PLAYER_MAINWINDOW_UISrcs}
)
target_include_directories(mp3player PRIVATE include)
target_include_directories(mp3player PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(mp3player SYSTEM
  PUBLIC
  ${Qt5Core_INCLUDE_DIRS}
  ${Qt5Widgets_INCLUDE_DIRS}
  ${Qt5Multimedia_ICLUDE_DIRS}
  ${JACK_INCLUDE_DIRS}
  qtjack::qtjack
)

target_link_libraries(mp3player PRIVATE
  qtjack::qtjack
  Qt5::Core
  Qt5::Widgets
  Qt5::Multimedia
  ${JACK_LIBRARIES}
)

####### Mididial #######
set(MIDIDIAL_SOURCES src/mididial.cpp
  src/mididial_mainwindow.cpp
  include/mididial_mainwindow.hpp
)

QT5_WRAP_CPP(MIDIDIAL_MOCrcs
  ${MIDIDIAL_SOURCES}
)

QT5_WRAP_UI(MIDIDIAL_UISrcs
  ${CMAKE_CURRENT_SOURCE_DIR}/src/mididial.ui)

add_executable(mididial)
target_sources(mididial
  PRIVATE
  ${MIDIDIAL_SOURCES}
  ${MIDIDIAL_UISrcs}
  ${MIDIDIAL_MOCrcs}
)
target_include_directories(mididial PRIVATE include)
target_include_directories(mididial PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(mididial SYSTEM
  PUBLIC
  ${Qt5Core_INCLUDE_DIRS}
  ${Qt5Widgets_INCLUDE_DIRS}
  ${JACK_INCLUDE_DIRS}
  qtjack::qtjack
)
target_link_libraries(mididial PRIVATE
  qtjack::qtjack
  Qt5::Core
  Qt5::Widgets
  ${JACK_LIBRARIES}
)

####### Sinedial #######
set(SINE_WAVE_SOURCES
  src/sine_wave.cpp
  include/sine_wave.hpp
)

add_library(sine_wave)
target_sources(sine_wave
	PRIVATE
  ${SINE_WAVE_SOURCES})
target_include_directories(sine_wave PRIVATE include)

set(SINEDIAL_SOURCES
  src/sinedial.cpp
  src/sinedial_mainwindow.cpp
  include/sinedial_mainwindow.hpp
)

QT5_WRAP_CPP(SINEDIAL_MOCrcs
  ${SINEDIAL_SOURCES}
)

QT5_WRAP_UI(SINEDIAL_UISrcs
  ${CMAKE_CURRENT_SOURCE_DIR}/src/sinedial.ui)

add_executable(sinedial)
target_sources(sinedial
  PRIVATE
  ${SINEDIAL_SOURCES}
  ${SINEDIAL_UISrcs}
  ${SINEDIAL_MOCrcs}
)
target_include_directories(sinedial PRIVATE include)
target_include_directories(sinedial PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(sinedial SYSTEM
  PUBLIC
  ${Qt5Core_INCLUDE_DIRS}
  ${Qt5Widgets_INCLUDE_DIRS}
  ${JACK_INCLUDE_DIRS}
  qtjack::qtjack
)
target_link_libraries(sinedial PRIVATE
  qtjack::qtjack
  Qt5::Core
  Qt5::Widgets
  ${JACK_LIBRARIES}
  sine_wave
)
