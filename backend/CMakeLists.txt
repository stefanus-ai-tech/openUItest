cmake_minimum_required(VERSION 3.10)
project(backend)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add Crow
add_subdirectory(external/Crow)

# Add cpr
set(CPR_BUILD_TESTS OFF CACHE BOOL "Don't build cpr tests")
add_subdirectory(external/cpr)

# Find required packages
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPQXX REQUIRED libpqxx)

# Add your source files
add_executable(backend main.cpp)

# Include directories
target_include_directories(backend PRIVATE 
    ${PostgreSQL_INCLUDE_DIRS}
    ${LIBPQXX_INCLUDE_DIRS}
    external/Crow/include
)

# Link libraries
target_link_libraries(backend PRIVATE 
    Crow::Crow
    cpr::cpr
    nlohmann_json::nlohmann_json
    ${PostgreSQL_LIBRARIES}
    ${LIBPQXX_LIBRARIES}
)