# Conway's Game of Life (sHaDe's version)

A simple Game of Life simulator made using Raylib and ImGUI.
A personal project to get my feet wet with C++ app/game development!
Raylib is intended to handle window setup and rendering, while
ImGUI will be used to create a UI so users can speed up, stop, or
otherwise control the simulation to their liking.


### Building

For now, the dev build steps are simply:

1. Clone/download the repo
2. Open the `.sln` file with Visual Studio
3. Build and run


### Schedule

Number  | Goal          | Timeline      | Notes
--|---------------------|---------------|--------
1 | Render Grid & Cells | 3/3 - 3/7     | - done 3/7 <br> [Github issue here](https://github.com/hadan24/game-of-life/issues/1)
2 | Store Cell States   | 3/10 - 3/14   | - done 3/16 (technically 3/9, after was working on [adding sim rules](https://github.com/hadan24/game-of-life/issues/4) to ensure grid worked)<br> [Github issue here](https://github.com/hadan24/game-of-life/issues/2)
3 | Optimize Grid+Ticks | 3/17 - 3/28   | - <br> [Github issue here](https://github.com/hadan24/game-of-life/issues/8)
4 | Toggle-able Cells   | 3/31 - 4/4    | - <br> [Github issue here](https://github.com/hadan24/game-of-life/issues/5)
5 | Tick vs Frame Rate  | 4/7 - 4/18    | - <br> [Github issue here](https://github.com/hadan24/game-of-life/issues/3)
6 | Create UI           | 4/21 - 5/2    | - <br> [Github issue here](https://github.com/hadan24/game-of-life/issues/6)
7 | Zoom In/Out         | 5/5 - 5/16    | - <br> [Github issue here](https://github.com/hadan24/game-of-life/issues/7)

#### UI Components
- simulation speed slider
- toggle for differently colored cells based on cell's state
- diff color scheme options
- zoom in/out

#### Stretch Goals

Number  | Goal          | Timeline      | Notes
--|---------------------|---------------|--------
1 |


### Resources/Notes to Self
- [Wikipedia](https://en.wikipedia.org/wiki/Conway's_Game_of_Life)
- [Raylib Cheatsheet](https://www.raylib.com/cheatsheet/cheatsheet.html)
- for ImGUI, open up `imgui.h` and `imgui_demo.cpp`