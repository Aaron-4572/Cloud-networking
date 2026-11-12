cmake_minimum_required(VERSION 3.29)
project(SFML_Crow)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)

# ASIO (Crow Dependency)
FetchContent_Declare(
        asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
        GIT_TAG asio-1-30-2
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
)
FetchContent_MakeAvailable(asio)
set(ASIO_INCLUDE_DIR "${asio_SOURCE_DIR}/asio/include" CACHE PATH "Asio include directory" FORCE)
set(asio_FOUND TRUE CACHE BOOL "Asio found flag" FORCE)

# Crow
FetchContent_Declare(
        CrowCpp
        GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
        GIT_TAG v1.2.0
)

set(CROW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CROW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(CrowCpp)

target_include_directories(Crow INTERFACE
        $<BUILD_INTERFACE:${asio_SOURCE_DIR}/asio/include>
)
target_compile_definitions(Crow INTERFACE ASIO_STANDALONE)



# Build Executable
add_executable(${PROJECT_NAME}
        src/main.cpp
)

# Link Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE Crow::Crow )

# Copy Webapp
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)

# Automatically copy resources folder to the build directory post-build
file(GLOB_RECURSE WEBAPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/resources/webapp/*")
add_custom_target(CopyWebapp ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/webapp"
        "${CMAKE_CURRENT_BINARY_DIR}/resources/webapp"
        DEPENDS ${WEBAPP_FILES}
)
add_dependencies(${PROJECT_NAME} CopyWebapp)

