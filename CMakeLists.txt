# CMakeList.txt : CMake project for test, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("test")

# if can't build the .lib/.a from src, need all the FetchContent stuff
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(raylib)
include_directories(
    dep/imgui
    dep/raylib/src
    dep/rlimgui
)

# Add source to this project's executable.
add_executable (
    ${PROJECT_NAME} main.cpp grid.cpp game-of-life.cpp
    dep/imgui/imgui.cpp
    dep/imgui/imgui_draw.cpp
    dep/imgui/imgui_tables.cpp
    dep/imgui/imgui_widgets.cpp

    dep/rlimgui/rlImGui.cpp
)

# if can build .lib/.a from src, only need this
target_link_libraries (${PROJECT_NAME} raylib)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET test PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
