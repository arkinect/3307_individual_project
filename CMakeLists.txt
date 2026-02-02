cmake_minimum_required(VERSION 3.15)
project(cpp_project)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Find packages defined in vcpkg.json
find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(main main.cpp)

# Link the libraries
target_link_libraries(main PRIVATE fmt::fmt nlohmann_json::nlohmann_json)