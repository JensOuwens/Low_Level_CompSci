cmake_minimum_required(VERSION 3.20)

project(MyGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# ---------------------------------------
# SFML
# ---------------------------------------

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.2
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
        ImGui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.91.9b
)

FetchContent_MakeAvailable(ImGui)

FetchContent_GetProperties(ImGui)

set(IMGUI_DIR ${imgui_SOURCE_DIR})


# ---------------------------------------
# ImGui-SFML
# ---------------------------------------

# SFML is already provided by FetchContent.
# Don't make ImGui-SFML search for another SFML installation.
set(IMGUI_SFML_FIND_SFML OFF)

FetchContent_Declare(
        ImGui-SFML
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
        GIT_TAG 2.6.x
)

FetchContent_MakeAvailable(ImGui-SFML)


# ---------------------------------------
# Our game
# ---------------------------------------

add_executable(MyGame
        src/main.cpp
        Src/ConcurrentInventory.cpp
        Src/ConcurrentInventory.h
)

target_link_libraries(MyGame PRIVATE
        sfml-graphics
        ImGui-SFML::ImGui-SFML
)

# Copy all runtime DLL dependencies next to MyGame.exe
add_custom_command(TARGET MyGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sfml-system>
        $<TARGET_FILE_DIR:MyGame>

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sfml-window>
        $<TARGET_FILE_DIR:MyGame>

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sfml-graphics>
        $<TARGET_FILE_DIR:MyGame>

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:ImGui-SFML>
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