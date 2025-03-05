#pragma once
#include "./Library/GameObject.h"
#include "./globals.h"
#include "./Stage.h"
#include <vector>

class Enemy :
    public GameObject
{
    Point pos_;
    bool isAlive_;
    float speed_;

    int movePattern{ 0 };
    int direction{ 0 };
    int frame{ 0 };

    Stage* stage;

    Point nextPos_;
public:
    Enemy();
    ~Enemy();

    void Update() override;
    void Draw() override;
    bool CheckHit(const Rect& me, const Rect& other);
    bool HitToChip(int _x, int _y);

    bool BFS(Node* root,Node* goal);
    bool Dijkstra(Node* startNode, Node* goalNode);
};

