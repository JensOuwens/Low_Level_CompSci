cmake_minimum_required(VERSION 3.20)

project(MyGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# ---------------------------------------
# Dependencies
# ---------------------------------------

set(SFML_VERSION 3.0.0)
set(IMGUI_VERSION 1.91.1)

# Build SFML as shared libraries
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)

# ---------------------------------------
# SFML
# ---------------------------------------

set(SFML_VERSION 3.0.0)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

FetchContent_Declare(
        SFML
        URL "https://github.com/SFML/SFML/archive/${SFML_VERSION}.zip"
)

set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SFML)
# ---------------------------------------
# Dear ImGui
# ---------------------------------------

FetchContent_Declare(
        imgui
        URL "https://github.com/ocornut/imgui/archive/v${IMGUI_VERSION}.zip"
)

FetchContent_MakeAvailable(imgui)

set(IMGUI_DIR ${imgui_SOURCE_DIR})


# ---------------------------------------
# ImGui-SFML
# ---------------------------------------

set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_DEMO ON)

FetchContent_Declare(
        imgui-sfml
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
        GIT_TAG v3.0
)

FetchContent_MakeAvailable(imgui-sfml)


# ---------------------------------------
# Our game
# ---------------------------------------

add_executable(MyGame
        src/main.cpp
        src/SpatialHash.cpp
)

target_include_directories(MyGame PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(MyGame PRIVATE
        SFML::Graphics
        ImGui-SFML::ImGui-SFML
)


# ---------------------------------------
# Copy runtime DLLs
# ---------------------------------------

add_custom_command(TARGET MyGame POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SFML::System>
        $<TARGET_FILE_DIR:MyGame>

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SFML::Window>
        $<TARGET_FILE_DIR:MyGame>

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SFML::Graphics>
        $<TARGET_FILE_DIR:MyGame>

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:ImGui-SFML::ImGui-SFML>
        $<TARGET_FILE_DIR:MyGame>

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/Program Files/JetBrains/CLion 2025.2.1/bin/mingw/bin/libstdc++-6.dll"
        $<TARGET_FILE_DIR:MyGame>

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/Program Files/JetBrains/CLion 2025.2.1/bin/mingw/bin/libgcc_s_seh-1.dll"
        $<TARGET_FILE_DIR:MyGame>

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/Program Files/JetBrains/CLion 2025.2.1/bin/mingw/bin/libwinpthread-1.dll"
        $<TARGET_FILE_DIR:MyGame>
)