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


private:
	Vector2 srcPos{};
	Vector2 screenPos{};

	unsigned int tileID{};
	float scale{};
	bool collision{};

	Rectangle srcRect{};
	Rectangle destRect{};


};

typedef struct Level
{
	Texture2D tileset;
	std::vector<Tile> tiles;
};


Level loadLevel(Texture2D tileset, std::vector<Tile>& tiles)
{
	Level level;
	level.tileset = tileset;
	level.tiles = tiles;

	return level;
}

void renderLevel(Level level)
{
	for (auto t : level.tiles)
	{
		DrawTexturePro(level.tileset, t.getSrcRec(), t.getScreenRec(), Vector2{}, 0.f, WHITE);

	}
}


std::vector<Tile> levelTiles;

