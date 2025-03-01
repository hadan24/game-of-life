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
1 | Render Grid & Cells | 3/3 - 3/7     | 
2 | Store Cell States   | 3/10 - 3/14   | 
3 | Tick vs Frame Rate  | 3/17 - 3/28   |
4 | Add Sim Rules       | 3/31 - 4/11   | 
5 | Toggle-able Cells   | 4/14 - 4/25   | 
6 | Create UI           | 4/28 - 5/2    | 

#### UI Components
- simulation speed slider
- toggle for differently colored cells based on cell's state

#### Stretch Goals

Number  | Goal          | Timeline      | Notes
--|---------------------|---------------|--------
1 |


### Resources/Notes to Self
- [Wikipedia](https://en.wikipedia.org/wiki/Conway's_Game_of_Life)
- [Raylib Cheatsheet](https://www.raylib.com/cheatsheet/cheatsheet.html)
- for ImGUI, open up `imgui.h` and `imgui_demo.cpp`