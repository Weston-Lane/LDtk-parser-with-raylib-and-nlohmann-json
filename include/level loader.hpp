#pragma once	
#include "raylib/raylib.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <vector>


class Tile
{
public:
	
	Tile()
	{

	}


	Tile(Vector2 srcPos,Vector2 screenPos, int scale, bool col, int id)
	{
		this->srcPos = srcPos;
		this->screenPos = screenPos;
		this->scale = scale;
		this->collision = col;
		this->tileID = id;

		this->destRect={ screenPos.x*scale, screenPos.y*scale, 16.f*scale,16.f * scale };
		this->srcRect = { srcPos.x, srcPos.y, 16, 16 };
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


std::vector<Tile> levelTiles;
std::vector<int> collisionTileIDs;


typedef struct Level
{
	Texture2D tileset;
	std::vector<Tile> tiles;
};


Level loadLevel(Texture2D tileset,std::string jsonFile)
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

	return Level{ tileset,levelTiles };
}

void renderLevel(Level level)
{
	for (auto t : level.tiles)
	{
		DrawTexturePro(level.tileset, t.getSrcRec(), t.getScreenRec(), Vector2{}, 0.f, WHITE);

	}
}



