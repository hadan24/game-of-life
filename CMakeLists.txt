# CMakeList.txt : CMake project for test, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project ("game-of-life")

# in case I want to add modern language features later
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


# if can't build the .lib/.a from src, also need FetchContent
include (FetchContent)
FetchContent_Declare (
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable (raylib)
include_directories (
    dep/imgui
    dep/raylib/src
    dep/rlimgui
)

# Add source to this project's executable.
list(APPEND IMGUI_SRCS
    "dep/imgui/imgui.cpp"
    "dep/imgui/imgui_draw.cpp"
    "dep/imgui/imgui_tables.cpp"
    "dep/imgui/imgui_widgets.cpp"

    "dep/rlimgui/rlImGui.cpp"
)
add_executable (
    ${PROJECT_NAME}
    src/main.cpp
    src/grid.cpp
    src/game-of-life.cpp

    ${IMGUI_SRCS}   # imgui stuff should rarely change, if ever
)

# if can build .lib/.a from src, only need this
target_link_libraries (${PROJECT_NAME} PRIVATE raylib)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
