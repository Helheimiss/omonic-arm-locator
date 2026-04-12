cmake_minimum_required(VERSION 4.1)
project(omonic-arm-locator-client)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)


add_subdirectory(libs/jsoncpp)

set(HTTPLIB_USE_OPENSSL_IF_AVAILABLE OFF)
set(HTTPLIB_USE_ZLIB_IF_AVAILABLE OFF)
set(HTTPLIB_USE_BROTLI_IF_AVAILABLE OFF)
set(HTTPLIB_USE_ZSTD_IF_AVAILABLE OFF)
add_subdirectory(libs/cpp-httplib)


add_executable(${PROJECT_NAME} src/main.cpp)
target_link_options(${PROJECT_NAME} PRIVATE -static)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE httplib::httplib jsoncpp)