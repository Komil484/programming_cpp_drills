cmake_minimum_required(VERSION 3.5)

project(Name_pairs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Name_pairs main.cpp)

include(GNUInstallDirs)
install(TARGETS Name_pairs
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
