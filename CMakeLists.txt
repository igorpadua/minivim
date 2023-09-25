cmake_minimum_required(VERSION 3.5)

project(minivim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(minivim main.cpp
    minivim.hpp minivim.cpp)

include(GNUInstallDirs)
install(TARGETS minivim
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

target_link_libraries(minivim ${CURSES_LIBRARIES})
