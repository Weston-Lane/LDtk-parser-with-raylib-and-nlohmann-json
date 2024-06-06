#pragma warning(disable : 4996)
#include <iostream>
#include <fstream>
#include "raylib/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raylib/raygui.h"
#include "json.hpp"
#include "level loader.hpp"

#define SCREENWIDTH 1280
#define SCREENHEIGHT 720

void printTiles(std::vector<Tile>& t);
bool checkCollision(Vector2& sqrPos);
void pause();
int main()
{


    InitWindow(SCREENWIDTH,SCREENHEIGHT, "level tester");
    SetTargetFPS(60);

    Vector2 sqrPos{};


    Level level1 = loadLevel(LoadTexture("levels/tileSet/sprites/world_tileset.png"), "levels/levelData/test level2.json");
    while (!WindowShouldClose())
    {
		sqrPos = GetMousePosition();
        sqrPos.x = sqrPos.x - 16;
        sqrPos.y = sqrPos.y - 16;

        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        renderLevel(level1);
	
        if (checkCollision(sqrPos))
            DrawRectangle(sqrPos.x, sqrPos.y, 16, 16, RED);
        else
            DrawRectangle(sqrPos.x, sqrPos.y, 16, 16, GREEN);

        EndDrawing();

        if (IsKeyPressed(KEY_P))
        {
            pause();
        }

    }

    //printTiles(levelTiles);
    CloseWindow();
    return 0;



      
}

bool checkCollision(Vector2& sqrPos)
{
	for (auto tile : levelTiles)
    {
        if (tile.getCollision())
        {
            if (CheckCollisionRecs(tile.getScreenRec(), { sqrPos.x,sqrPos.y,16,16 }))
            {
                return true;
			}
		}
	}
	return false;  
}

void pause()
{
    while (!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

		DrawText("PAUSED", SCREENWIDTH / 2 - 50, SCREENHEIGHT / 2, 20, RED);

        EndDrawing();

		if (IsKeyPressed(KEY_P))
        {
            return;
		}
    }
}

void printTiles(std::vector<Tile>& t)
{
    for (auto tile : t)
    {
		std::cout << "srcPos: " << tile.getSrcPos().x << " " << tile.getSrcPos().y << std::endl;
		std::cout << "screenPos: " << tile.getScreenPos().x << " " << tile.getScreenPos().y << std::endl;
		std::cout << "scale: " << tile.getScale() << std::endl;
		std::cout << "collision: " << tile.getCollision() << std::endl;
		std::cout << "tileID: " << tile.getTileID() << std::endl;
		std::cout << std::endl;
    }
}