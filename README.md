# Personal LDtk Level Parser

## Highlights
- API takes a json file input from a LDtK level

## ℹ️ Overview

I recently discovered LDtk while looking for a 2D level creation tool. LDtk has many ways of parsing the data from your level but I decided to go the more personal route
and create my own parser for the json file format since LDtk has some great documentation on it. [RAYLIB](https://github.com/raysan5/raylib/tree/master) AND 
[NLOHMANN JSON](https://github.com/nlohmann/json) LIBRARIES ARE REQUIRED FOR THIS. Raylib is a fantastic C library for making games and nlohmann json allowed for easy parsing of the data.
This API uses the loadLevel() function as the work horse and returns a created level object. From there all that is needed is to call the renderLevel() function in the game 
loop and your level is there! More implementations will be added as I surely will come across more limitations. 

> [!IMPORTANT]
>  [RAYLIB](https://github.com/raysan5/raylib/tree/master) AND 
[NLOHMANN JSON](https://github.com/nlohmann/json) LIBRARIES ARE REQUIRED FOR THIS

### ✍️ Authors

Weston Lane


## 🚀 Usage



## ⬇️ Installation

Just download the level loader.hpp file from the include directory.

