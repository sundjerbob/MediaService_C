cmake_minimum_required(VERSION 3.24)
project(MediaService C)

set(CMAKE_C_STANDARD 11)

# Include the MinIO client source files
include_directories(curl-8.3.0_1-win64-mingw/curl-8.3.0_1-win64-mingw/include)

# Add the MinIO client source files to your project
add_executable(MediaService main.c)

# Link the Winsock library (ws2_32.lib) for Windows
if (WIN32)
    target_link_libraries(MediaService ws2_32)
endif ()
