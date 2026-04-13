cmake_minimum_required(VERSION 4.1)
project(omonic-arm-locator-client)

if (WIN32)
    set(CMAKE_PREFIX_PATH "C:/msys64/mingw64;${CMAKE_PREFIX_PATH}"})
    add_compile_definitions(WINVER=0x0A00 _WIN32_WINNT=0x0A00)
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)


add_subdirectory(libs/jsoncpp)


add_executable(${PROJECT_NAME} src/main.cpp)
target_link_options(${PROJECT_NAME} PRIVATE -static)
target_include_directories(${PROJECT_NAME} PRIVATE src include)
target_link_libraries(${PROJECT_NAME} PRIVATE jsoncpp
        $<$<PLATFORM_ID:Windows>:ws2_32>
        $<$<PLATFORM_ID:Windows>:iphlpapi>
)