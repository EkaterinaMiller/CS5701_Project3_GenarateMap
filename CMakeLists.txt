cmake_minimum_required(VERSION 3.22)
project(SFML3_App LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# PATH helps at runtime for DLL lookup, but find_package() needs the CMake config path.
if(WIN32)
    list(APPEND CMAKE_PREFIX_PATH "C:/SFML-3.1.0")
endif()

# 1. Tell CMake to explicitly look for static SFML libraries
set(SFML_STATIC_LIBRARIES ON)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

set(SFML_APP_SOURCES
    src/main.cpp
    src/bulletList.cpp
    src/button.cpp
    src/mapDisplay.cpp
)

add_executable(sfml-app ${SFML_APP_SOURCES}
    src/display.cpp
    include/helpers.h)
target_include_directories(sfml-app PRIVATE include)
target_link_libraries(sfml-app PRIVATE SFML::Graphics SFML::Window SFML::System)

# 2. WINDOWS ONLY: Force SFML to compile in static mode (prevents missing DLL errors)
if(WIN32)
    target_compile_definitions(sfml-app PRIVATE SFML_STATIC)
endif()

# Automatically copy the assets into the executable output directory on every build
add_custom_command(
    TARGET sfml-app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:sfml-app>"
    COMMENT "Copying game assets to the build output directory..."
)