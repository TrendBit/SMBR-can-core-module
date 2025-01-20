cmake_minimum_required(VERSION 3.16)
project(core-module
        VERSION 1.0
        LANGUAGES CXX)

# Compiler settings
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -pedantic -pg")

# Find required packages
find_package(Threads REQUIRED)
find_library(PIGPIO_LIBRARY pigpio REQUIRED)
find_library(RT_LIBRARY rt REQUIRED)

# Source files
file(GLOB_RECURSE SOURCES
    "source/*.cpp"
)

file(GLOB_RECURSE HEADERS
    "source/*.hpp"
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${PIGPIO_LIBRARY}
        Threads::Threads
        ${RT_LIBRARY}
)

# Set include directories
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/source
        ${CMAKE_CURRENT_SOURCE_DIR}/source/codes/messages
)

# Set output directories
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Installation
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

# Service installation target
add_custom_target(install-service
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/core-module.service
            /etc/systemd/system/core-module.service
    DEPENDS ${PROJECT_NAME}
    COMMENT "Installing core-module systemd service"
)
