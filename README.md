# Conway's Game of Life (sHaDe's version)

A simple Game of Life simulator made using Raylib and ImGUI.
A personal project to get my feet wet with C++ app/game development!
Raylib is intended to handle window setup and rendering, while
ImGUI will be used to create a UI so users can speed up, stop, or
otherwise control the simulation to their liking.


### Building

1. Clone the repo
    - use `--recursive` if you do not have:
        - the [DearImGUI](https://github.com/ocornut/imgui) or [rlImGui](https://github.com/raylib-extras/rlImGui) source files
        - nor a pre-compiled [Raylib](https://github.com/raysan5/raylib) static library
    - if using Visual Studio, don't forget to set the "Select Startup Item" field to `game-of-life.*`
2. Build
    - if building manually *without CMake*, be sure to compile the `DearImGui` and `rlImGui` source files alongside this project's sources
        - (the full list of needed files is in my [`CMakeLists.txt`](CMakeLists.txt))
3. Run and enjoy some cool blinking cells :)


### Schedule

Number  | Goal          | Timeline      | Notes
--|---------------------|---------------|--------
1 | Render Grid & Cells | 3/3 - 3/7     | - done 3/7 <br> [Github issue here](https://github.com/hadan24/game-of-life/issues/1)
2 | Store Cell States   | 3/10 - 3/14   | - done 3/16 (technically 3/9, after that was working on [adding sim rules](https://github.com/hadan24/game-of-life/issues/4) to ensure grid worked) <br> [Github issue here](https://github.com/hadan24/game-of-life/issues/2)
3 | Optimize Grid+Ticks | 3/17 - 3/21   | - done 3/27 (after several days of bad sleep and frantic interview prep) <br> [Github issue here](https://github.com/hadan24/game-of-life/issues/8)
4 | Toggle-able Cells   | 3/24 - 3/28   | - done 3/29 <br> [Github issue here](https://github.com/hadan24/game-of-life/issues/5)
5 | Tick vs Frame Rate  | 3/31 - 4/11   | - done 4/10 <br> [Github issue here](https://github.com/hadan24/game-of-life/issues/3)
6 | Create UI           | 4/14 - 4/25   | - done 4/28 (technically 4/27, got a UI w/ original intended features working, was hoping to add screen wrapping but no time, it'll be new feature) <br> [Github issue here](https://github.com/hadan24/game-of-life/issues/6)
7 | Screen Wrapping     | 4/28 - 5/2    | - done 5/3 (technically 5/2, spent more time to fix [mouse bounds bug](https://github.com/hadan24/game-of-life/issues/9)) <br> [goal's Github issue here](https://github.com/hadan24/game-of-life/issues/11)
8 | Clean Code + Repo   | 5/19 - 5/30   | - done 6/3, learning CMake and adapting this project to it was,,, an experience. Learned quite a bit though! <br> [Github issue here](https://github.com/hadan24/game-of-life/issues/12)

#### Stretch Goals

Number  | Goal          | Timeline      | Notes
--|---------------------|---------------|--------
1 | Zoom In/Out         | TBD           | - moved back to stretch goal to move on to new project, may return sometime <br> [Github issue here](https://github.com/hadan24/game-of-life/issues/7)
2 | Save UI configs     | TBD           | - <br> [Github issue here](https://github.com/hadan24/game-of-life/issues/13)
3 | Sound Effects       | TBD           | - <br> [Github issue here](https://github.com/hadan24/game-of-life/issues/14)


### Resources/Notes to Self
- [Wikipedia](https://en.wikipedia.org/wiki/Conway's_Game_of_Life)
- [Raylib Cheatsheet](https://www.raylib.com/cheatsheet/cheatsheet.html)
- for ImGUI, open up `imgui.h` and `imgui_demo.cpp`