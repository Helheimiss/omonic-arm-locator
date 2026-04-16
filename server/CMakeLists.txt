cmake_minimum_required(VERSION 4.1)
project(omonic-arm-locator-server)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)



file(COPY configs DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})


find_package(nanodbc REQUIRED COMPONENTS Sql)


set(BUILD_CTL OFF)
set(BUILD_EXAMPLES OFF)
set(BUILD_ORM OFF)
set(BUILD_BROTLI OFF)
set(BUILD_YAML_CONFIG OFF)
add_subdirectory(libs/drogon)


add_executable(${PROJECT_NAME}
        src/main.cpp
        src/armDB/ArmDB.cpp
        src/server/Server.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE drogon nanodbc)
target_include_directories(${PROJECT_NAME} PRIVATE src)