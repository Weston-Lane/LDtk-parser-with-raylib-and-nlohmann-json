/*
//Dependencies: raylib, nlohmann json

This file is used to load LDtk levels into a game via a json parser. It uses the nlohmann json library to parse the json file and extract the necessary 
data to create a level.

Uses a Tile class to store the data of each tile, each tile consists of: a source rectangle, a screen rectangle, a scale, a collision bool and a tile ID. which
are all found in the json file.

Use the loadLevel() function to load a level, it takes a texture2D, a string for the json file path and a bool to initiate paralax scrolling. 
The function returns a Level struct

The level Struct holds all of the data for the level and is the main object created from this file to be used in other translation units.

Once in the load level function the parser will parse through the tiles with collision and store their IDs in a vector, then it will parse through the level data 
(getting the src pos, map pos, and tile ID) then creates a tile object for each tile in the map and stores them in a vector.
The loadLevel() function then checks if the tile collision Vector has any matching IDs with the tile vector, if so, then the collion bool for that tile is set to true.

The Background struct stores the textures for the background and positions for each background.
	has a paralax scrolling velocity if the level pralax bool is set to true.
	IMPORTANT: the background textures must be entered manually in the struct or left as empty strings.
	ALSO: make sure the scale is the same for all textures

The renderLevel() function renders the background and paralax if desired, then renders all the tiles in the level in their respective positions.

IF COLLISION IS DESIRED, USE THE TILE COLLISION BOOL TO CHECK COLLISION IN YOUR OWN COLLISION FUNCTION.

-----CAN CHANGE TILESIZE AND SCALE MACROS IF NEEDED-------

*/
#pragma once	
#include "raylib/raylib.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <vector>

#ifndef SCALE TILESIZE
#define SCALE 3.f //scale macro for this translation unit
#define TILESIZE 16.f
#endif 



class Tile//tile class to store tile data
{
public:
	
	Tile(Vector2 srcPos, Vector2 screenPos, int scale, bool col, int id)//tile constructor takes a source position, screen position, scale, collision bool and tile ID
	{
		this->srcPos = srcPos;
		this->screenPos = screenPos;
		this->scale = SCALE;
		this->collision = col;
		this->tileID = id;

		this->destRect={ screenPos.x*scale, screenPos.y*scale, TILESIZE*scale,TILESIZE * scale };
		this->srcRect = { srcPos.x, srcPos.y, TILESIZE, TILESIZE	};
	}

	Rectangle getSrcRec()
	{
		return srcRect;
	}
	Rectangle getScreenRec()
	{
		return destRect;
	}
	Vector2 getSrcPos()
	{
		return srcPos;
	}
	Vector2 getScreenPos()
	{
		return screenPos;
	}
	int getScale()
	{
		return scale;
	}
	bool getCollision()
	{
		return collision;
	}
	int getTileID()
	{
		return tileID;
	}

	void setScreenPos(Vector2 pos)
	{
		screenPos = pos;
	}

	void setCollision(bool col)
	{
		collision = col;
	}


private:
	Vector2 srcPos{};
	Vector2 screenPos{};

	unsigned int tileID{};
	float scale{};
	bool collision{};

	Rectangle srcRect{};
	Rectangle destRect{};


};



struct background
{
	Texture2D bg1 = LoadTexture("levels/backgrounds/nature_5/1.png");//background paths must be entered manually or empty string
	Texture2D bg2 = LoadTexture("levels/backgrounds/nature_5/2.png");
	Texture2D bg3 = LoadTexture("levels/backgrounds/nature_5/3.png");
	Texture2D bg4 = LoadTexture("");
	Texture2D bg5 = LoadTexture("");

	Vector2 bg1Pos{};
	Vector2 bg2Pos{};
	Vector2 bg3Pos{};
	Vector2 bg4Pos{};
	Vector2 bg5Pos{};
	float paralaxVel{};
};

typedef struct Level
{
	Texture2D tileset;
	std::vector<Tile> tiles;
	bool paralax = false;
	background bg;
};



std::vector<Tile> levelTiles;
std::vector<int> collisionTileIDs;

Level loadLevel(Texture2D tileset,std::string jsonFile, bool paralax)
{
	//initializes json object
	using json = nlohmann::json;

	std::ifstream jsonFileStream(jsonFile);
	if (!jsonFileStream.is_open())
		std::cout << "could not open level data" << std::endl;
	//beigns parsing json file
	json jsonData = json::parse(jsonFileStream);
	//checks for collision tiles Ids and puts them into vector
	for (auto& tilesets : jsonData["defs"]["tilesets"])
	{
		
		for (auto& enumTags : tilesets["enumTags"])
		{
	
			for (auto& tileID : enumTags["tileIds"])
			{
				
					int id = tileID.get<int>();
					collisionTileIDs.push_back(id);
			}
				
		}
	}
	//initailizes tile objects from json data and puts them into vector
	for (auto& levels : jsonData["levels"])
	{
		for (auto& layerInstances : levels["layerInstances"])
			for (auto& grid : layerInstances["gridTiles"])
			{
				float posX{};
				float posY{};
				float srcX{};
				float srcY{};
				float id{};
				bool XorY = true;

				for (auto& pos : grid["px"])
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
				for (auto& pos : grid["src"])
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
				for (auto& pos : grid["t"])
				{
					id = pos.get<int>();
				}

				Tile newTile(Vector2{ srcX,srcY }, Vector2{ posX,posY }, 3, false, id);
				levelTiles.push_back(newTile);

			}

	}
	//checks if the tile is a collision tile and sets it to true
	for (auto& tile : levelTiles)
	{
		for (auto& id : collisionTileIDs)
		{
			if (tile.getTileID() == id)
			{
				tile.setCollision(true);
			}
		}
	}

	return Level{ tileset,levelTiles,paralax};
}


void renderBackground(Level& level)
{
	if (level.paralax)
	{
		level.bg.bg1Pos.x -= level.bg.paralaxVel * GetFrameTime();
		level.bg.paralaxVel += 20;
		level.bg.bg2Pos.x -= level.bg.paralaxVel * GetFrameTime();
		level.bg.paralaxVel += 20;
		level.bg.bg3Pos.x -= level.bg.paralaxVel * GetFrameTime();
		level.bg.paralaxVel += 20;
		level.bg.bg4Pos.x -= level.bg.paralaxVel * GetFrameTime();
		level.bg.paralaxVel += 20;
		level.bg.bg5Pos.x -= level.bg.paralaxVel * GetFrameTime();
		level.bg.paralaxVel = 0;

		if (level.bg.bg1Pos.x <= -level.bg.bg1.width * 2)
			level.bg.bg1Pos.x = 0;
		if (level.bg.bg2Pos.x <= -level.bg.bg2.width * 2)
			level.bg.bg2Pos.x = 0;
		if (level.bg.bg3Pos.x <= -level.bg.bg3.width * 2)
			level.bg.bg3Pos.x = 0;
		if (level.bg.bg4Pos.x <= -level.bg.bg4.width * 2)
			level.bg.bg4Pos.x = 0;
		if (level.bg.bg5Pos.x <= -level.bg.bg5.width * 2)
			level.bg.bg5Pos.x = 0;
	}
		DrawTextureEx(level.bg.bg1, level.bg.bg1Pos, 0.f, SCALE, WHITE);
		if(level.paralax)
			DrawTextureEx(level.bg.bg1, { level.bg.bg1Pos.x + level.bg.bg1.width * SCALE }, 0.f, SCALE, WHITE);

		DrawTextureEx(level.bg.bg2, level.bg.bg2Pos, 0.f, SCALE, WHITE);
		if (level.paralax)
			DrawTextureEx(level.bg.bg2, { level.bg.bg2Pos.x + level.bg.bg2.width * SCALE }, 0.f, SCALE, WHITE);

		DrawTextureEx(level.bg.bg3, level.bg.bg3Pos, 0.f, SCALE, WHITE);
		if (level.paralax)
			DrawTextureEx(level.bg.bg3, { level.bg.bg3Pos.x + level.bg.bg3.width * SCALE }, 0.f, SCALE, WHITE);

		DrawTextureEx(level.bg.bg4, level.bg.bg4Pos, 0.f, SCALE, WHITE);
		if (level.paralax)
			DrawTextureEx(level.bg.bg4, { level.bg.bg4Pos.x + level.bg.bg4.width * SCALE }, 0.f, SCALE, WHITE);

		DrawTextureEx(level.bg.bg5, level.bg.bg5Pos, 0.f, SCALE, WHITE);
		if (level.paralax)
			DrawTextureEx(level.bg.bg5, { level.bg.bg5Pos.x + level.bg.bg5.width * SCALE }, 0.f, SCALE, WHITE);
	

}

void renderLevel(Level& level)
{
	renderBackground(level);

	for (auto t : level.tiles)
	{
		DrawTexturePro(level.tileset, t.getSrcRec(), t.getScreenRec(), Vector2{}, 0.f, WHITE);
	}
}




