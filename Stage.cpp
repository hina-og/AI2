#include "Stage.h"
#include "./globals.h"

Stage::Stage()
{
	stageData = vector(STAGE_HEIGHT, vector<STAGE_OBJ>(STAGE_WIDTH, STAGE_OBJ::EMPTY));

	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			if (y == 0 || y == STAGE_HEIGHT - 1 || x == 0 || x == STAGE_WIDTH - 1)
			{
				stageData[y][x] = STAGE_OBJ::WALL;
				node[y][x].isWall = true;
			}
			else
			{
				if (x % 2 == 0 && y % 2 == 0)
				{
					stageData[y][x] = STAGE_OBJ::WALL;
					node[y][x].isWall = true;
				}
				else
				{
					stageData[y][x] = STAGE_OBJ::EMPTY;
					node[y][x].isWall = false;
				}
			}
			node[y][x].pos = { x,y };
			node[y][x].parentNode = nullptr;
			node[y][x].isOpen = false;
		}
	}


	//–_“|‚µ–@‚Å•Ç¶¬
	int direction = 0;
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			if (!(x == 0 || x == STAGE_WIDTH - 1 ||
				y == 0 || y == STAGE_HEIGHT - 1) &&
				!(x % 2 == 1 || y % 2 == 1))
			{
				direction = GetRand(4);

				switch (direction)
				{
				case 0:
					stageData[y - 1][x] = STAGE_OBJ::WALL;
					node[y - 1][x].isWall = true;
					break;
				case 1:
					stageData[y][x - 1] = STAGE_OBJ::WALL;
					node[y][x - 1].isWall = true;
					break;
				case 2:
					stageData[y + 1][x] = STAGE_OBJ::WALL;
					node[y + 1][x].isWall = true;
					break;
				case 3:
					stageData[y][x + 1] = STAGE_OBJ::WALL;
					node[y][x + 1].isWall = true;
					break;
				default:
					break;
				}
			}
		}
	}

	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			if (y > 0) {

				node[y][x].neighborNode[0] = &node[y - 1][x];
			}
			else 
			{
				node[y][x].neighborNode[0] = nullptr;
			}
			if (y < STAGE_HEIGHT - 1) 
			{
				node[y][x].neighborNode[1] = &node[y + 1][x];
			}
			else 
			{
				node[y][x].neighborNode[1] = nullptr;
			}
			if (x > 0) 
			{
				node[y][x].neighborNode[2] = &node[y][x - 1];
			}
			else 
			{
				node[y][x].neighborNode[2] = nullptr;
			}
			if (x < STAGE_WIDTH - 1) 
			{
				node[y][x].neighborNode[3] = &node[y][x + 1];
			}
			else 
			{
				node[y][x].neighborNode[3] = nullptr;
			}
		}
	}

	setStageRects();
}

Stage::~Stage()
{
}

void Stage::Update()
{
}

void Stage::Draw()
{
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			switch (stageData[y][x])
			{
			case STAGE_OBJ::EMPTY:
				DrawBox(x * CHA_WIDTH, y * CHA_HEIGHT, x * CHA_WIDTH + CHA_WIDTH, y * CHA_HEIGHT + CHA_HEIGHT, GetColor(102, 205, 170), TRUE);
				break;
			case STAGE_OBJ::WALL:
				DrawBox(x * CHA_WIDTH, y * CHA_HEIGHT, x * CHA_WIDTH + CHA_WIDTH, y * CHA_HEIGHT + CHA_HEIGHT, GetColor(119, 136, 153), TRUE);
				break;
			case STAGE_OBJ::GOAL:
			
				break;
			default:
				break;
			}
		}
	}
}

void Stage::setStageRects()
{
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			if (stageData[y][x] == STAGE_OBJ::WALL)
			{
				stageRects.push_back(Rect(x * CHA_WIDTH, y * CHA_HEIGHT,  CHA_WIDTH, CHA_HEIGHT));
			}
		}
	}

}


