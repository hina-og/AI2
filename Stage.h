#pragma once
#include "./Library/GameObject.h"
#include <vector>
#include <map>
#include <string>
#include <vector>
#include "./Source/Screen.h"
#include "./globals.h"

using std::vector;


constexpr int STAGE_WIDTH = (Screen::WIDTH / CHA_WIDTH) % 2 ? (Screen::WIDTH / CHA_WIDTH) : (Screen::WIDTH / CHA_WIDTH) - 1;
constexpr int STAGE_HEIGHT = (Screen::HEIGHT / CHA_HEIGHT) % 2 ? (Screen::HEIGHT / CHA_HEIGHT) : (Screen::HEIGHT / CHA_HEIGHT) - 1;

struct Node
{
	Point pos;
	Node* parentNode;

	bool isWall;
	//Node(Point p) : pos(p), parentNode(nullptr) {};
};

class Stage :
    public GameObject
{
	vector<vector<STAGE_OBJ>> stageData;
	vector<Rect> stageRects;

public:
	Node node[STAGE_HEIGHT][STAGE_WIDTH];

	Stage();
	~Stage();
	void Update() override;
	void Draw() override;
	STAGE_OBJ GetStageData(int x, int y) {return stageData[y][x];}
	vector<Rect> GetStageRects() { return stageRects; }
	void setStageRects();
};

