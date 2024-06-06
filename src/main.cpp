#pragma warning(disable : 4996)
#include <iostream>
#include <fstream>
#include "raylib/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raylib/raygui.h"
#include "json.hpp"
#include "level loader.hpp"

#define SCREENWIDTH 800
#define SCREENHEIGHT 540

void printTiles(std::vector<Tile>& t);

int main()
{
    using json = nlohmann::json;

    std::ifstream jsonFileStream("levels/levelData/test level.json");
    if (!jsonFileStream.is_open())
        std::cout << "could not open level data" << std::endl;
    
    json jsonData = json::parse(jsonFileStream);

    //std::cout << "test:" << std::setw(2) << jsonData["defs"]["tilesets"] << std::endl;
    

    for (auto& item : jsonData["levels"])//this is how to get to the data for a tiles pos and src
    {
        for (auto& grid : item["layerInstances"])
            for (auto& data : grid["gridTiles"])
            {
                float posX{};
                float posY{};
                float srcX{};
                float srcY{};
				float id{};
				bool XorY = true;

                for (auto& pos : data["px"])
                {
					if (XorY)
					{
						posX = pos.get<int>();
						XorY = false;
					}
					else
					{
						posY = pos.get<int>();
						XorY = true;
					}
					
                }
                for (auto& pos : data["src"])
                {
                    if (XorY)
                    {
                        srcX = pos.get<int>();
                        XorY = false;
                    }
                    else
                    {
                        srcY = pos.get<int>();
                        XorY = true;
                    }
                }
                for (auto& pos : data["t"])
                {
					id = pos.get<int>();
                }
                    
				Tile newTile(Vector2{ srcX,srcY }, Vector2{ posX,posY }, 2, false, id);
				levelTiles.push_back(newTile);

            }
        
                    
    }


    
    InitWindow(SCREENWIDTH,SCREENHEIGHT, "level tester");
    SetTargetFPS(60);

	Level level=loadLevel(LoadTexture("levels/tileSet/sprites/world_tileset.png"), levelTiles); 

    while (!WindowShouldClose())
    {
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        renderLevel(level);

        EndDrawing();
    }

    //printTiles(levelTiles);
    CloseWindow();
    return 0;



      
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