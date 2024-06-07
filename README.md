# Personal LDtk Level Parser

## Highlights
- API takes a json file input from a LDtK level
- written in C++
## ℹ️ Overview

I recently discovered LDtk while looking for a 2D level creation tool. LDtk has many ways of parsing the data from your level but I decided to go the more personal route
and create my own parser for the json file format since LDtk has some great documentation on it. [RAYLIB](https://github.com/raysan5/raylib/tree/master) AND 
[NLOHMANN JSON](https://github.com/nlohmann/json) LIBRARIES ARE REQUIRED FOR THIS. Raylib is a fantastic C library for making games and nlohmann json allowed for easy parsing of the data.
This API uses the loadLevel() function as the work horse and returns a created level object. From there all that is needed is to call the renderLevel() function in the game 
loop and your level is there! More implementations will be added as I surely will come across more limitations. This is honestly probably only useful to me but I put it on here because it is one of the first
useful tools I have made.

> [!IMPORTANT]
>  [RAYLIB](https://github.com/raysan5/raylib/tree/master) AND 
[NLOHMANN JSON](https://github.com/nlohmann/json) LIBRARIES ARE REQUIRED FOR THIS
>>
> WHEN USING LDTK EDITOR YOU MUST DESIGNATE AN ENUM AS COLLISION AND MARK ALL THE DESIRED COLLISION TILES
FOR THE COLLISION FUCNTION TO WORK

### ✍️ Authors

Weston Lane


## 🚀 Look
![LDtk](https://github.com/Weston-Lane/level-loader-json-parser/blob/main/github%20Images/LDtk%20capture.PNG)
![Loaded Level](https://github.com/Weston-Lane/level-loader-json-parser/blob/main/github%20Images/level%20capture.PNG)


## ⬇️ Installation

Just download the level loader.hpp file from main directory or from the include directory. The rest of the files were from me just testing


