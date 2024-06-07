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
bool checkCollision(Vector2& sqrPos,Rectangle&);
void pause();


struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame);

int main()
{
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "level tester");
    SetTargetFPS(60);

    Texture2D cyborgIdle = LoadTexture("assets/Cyborg/Cyborg_idle.png");
	Texture2D cyborgRun = LoadTexture("assets/Cyborg/Cyborg_run.png");

    Vector2 velocity{0};
    int moveSpeed{ 200 };
	double gravity = 1000;
	double jumpForce = -650;
    Vector2 pos{};

    int maxFramesIdle{4};
    int maxFramesRun{6};
    float scale = 2.f;
    int frame = 0;
    Vector2 lastFramePos{};

    AnimData idle
    {
       Rectangle{
            0,
            0,
            (static_cast<float>(cyborgIdle.width) / maxFramesIdle),
            (static_cast<float>(cyborgIdle.height)) },
        Vector2{0,0},
        0,
        1.f / 12.f,
        0
    };
	AnimData runSrc
	{
		Rectangle{
			0,
			0,
			(static_cast<float>(cyborgRun.width) / maxFramesRun),
			(static_cast<float>(cyborgRun.height)) },
		Vector2{0,0},
		0,
		1.f / 12.f,
		0
	};
	AnimData runDest
	{
		Rectangle{},
		Vector2{0,0},
		0,
		1.f / 12.f,
		0
	};

    Vector2 sqrPos{};

    Level level1 = loadLevel(LoadTexture("levels/tileSet/sprites/world_tileset.png"), "levels/levelData/layer testing.json",true);

    Camera2D camera = { 0 };
    camera.target = Vector2{ runDest.pos.x+runDest.rec.width/2, runDest.pos.y+runDest.rec.height/2};
    camera.offset = Vector2{ SCREENWIDTH / 2.0f, SCREENHEIGHT / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

   
    while (!WindowShouldClose())
    {
		const float DT = GetFrameTime();

		sqrPos = GetMousePosition();
        sqrPos.x = sqrPos.x - 16;
        sqrPos.y = sqrPos.y - 16;

        //camera.target = Vector2{ runDest.pos.x + runSrc.rec.width/2, runDest.pos.y + runSrc.rec.height/2};
		
        camera.target = Vector2{runDest.pos.x*scale+runDest.rec.width/2,runDest.pos.y*scale+runDest.rec.height/2};
        camera.offset = Vector2{ SCREENWIDTH / 2.0f, SCREENHEIGHT / 2.0f };

        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        BeginMode2D(camera);
        
        DrawLine((int)camera.target.x, -SCREENHEIGHT * 10, (int)camera.target.x, SCREENHEIGHT * 10, GREEN);
        DrawLine(-SCREENWIDTH * 10, (int)camera.target.y, SCREENWIDTH * 10, (int)camera.target.y, GREEN);
        
        renderLevel(level1);
		
        if (checkCollision(sqrPos, runDest.rec))
        {
            DrawRectangle(sqrPos.x, sqrPos.y, 16, 16, RED);
			DrawText("COLLISION", 10, 10, 20, RED);
			
        }

        else
        {
            DrawRectangle(sqrPos.x, sqrPos.y, 16, 16, GREEN);

            if (IsKeyDown(KEY_A))
            {
                runDest.pos.x -= moveSpeed * DT;
            }
            if (IsKeyDown(KEY_D))
            {
                runDest.pos.x += moveSpeed * DT;
            }
            if (IsKeyDown(KEY_S))
            {
                runDest.pos.y += moveSpeed * DT;
            }
            if (IsKeyDown(KEY_W))
            {
                runDest.pos.y -= moveSpeed * DT;
            }
            
        }

		runSrc=updateAnimData(runSrc, DT, maxFramesIdle);
        runSrc.rec = { runSrc.rec.x +1,runSrc.rec.y + 16,runSrc.rec.width - 20,runSrc.rec.height - 3 };

        runDest.rec = { runDest.pos.x * scale,
                       runDest.pos.y * scale,
                       runSrc.rec.width * scale,
                       runSrc.rec.height * scale };



		DrawTexturePro(cyborgRun, runSrc.rec,runDest.rec, Vector2{},0.f, WHITE);
		
		runSrc.rec = { runSrc.rec.x - 1,runSrc.rec.y - 16,runSrc.rec.width + 20,runSrc.rec.height + 3 };
       
        EndMode2D();
        
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

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)//maxFrame used for the variable frame values
{
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0;
        data.rec.x = data.frame * (data.rec.width);
        data.frame++;
        if (data.frame > maxFrame)
            data.frame = 0;
    }
    return data;

}

bool checkCollision(Vector2& sqrPos,Rectangle& player)
{
	for (auto tile : levelTiles)
    {
        if (tile.getCollision())
        {
            if (CheckCollisionRecs(tile.getScreenRec(), { sqrPos.x,sqrPos.y,16,16 })||CheckCollisionRecs(tile.getScreenRec(),player))
            {
                player.y=tile.getScreenPos().y-player.height;
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