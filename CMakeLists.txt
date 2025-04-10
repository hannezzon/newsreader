cmake_minimum_required(VERSION 3.10)
project(NewsReader VERSION 1.0.1 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Find required packages
find_package(CURL REQUIRED)

# Add include directories for dependencies
include_directories(${CURL_INCLUDE_DIRS})

# Add source files
set(SOURCES
    src/main.cpp
    src/cli_parser.cpp
    src/command_handler.cpp
    src/rss_feed.cpp
)

# Create executable
add_executable(newsreader ${SOURCES})

# Link libraries
target_link_libraries(newsreader ${CURL_LIBRARIES})

# Install target
install(TARGETS newsreader DESTINATION bin)

# Add compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(newsreader PRIVATE -Wall -Wextra -Wpedantic)
elseif(MSVC)
    target_compile_options(newsreader PRIVATE /W4)
endif()

# Output binary to bin directory
set_target_properties(newsreader PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Add testing
enable_testing()
# Add tests here when needed
