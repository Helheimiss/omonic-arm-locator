cmake_minimum_required(VERSION 4.1)
project(omonic-arm-locator-server)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(libs/drogon)
set(BUILD_CTL OFF)
set(BUILD_EXAMPLES OFF)
set(BUILD_ORM OFF)
set(BUILD_BROTLI OFF)
set(BUILD_YAML_CONFIG OFF)


add_executable(${PROJECT_NAME}
        src/main.cpp

        src/server/Server.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE drogon)
target_include_directories(${PROJECT_NAME} PRIVATE src)