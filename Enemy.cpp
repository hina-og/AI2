#include "Enemy.h"
#include "globals.h"
#include "Player.h"
#include <queue>
#include <vector>
#include <limits>

Enemy::Enemy()
	:pos_({ CHA_WIDTH * 19,CHA_HEIGHT * 9 }), isAlive_(true)
{
	nextPos_ = pos_;
    speed_ = 1;
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
    Player* p = (Player*)FindGameObject<Player>();
    stage = (Stage*)FindGameObject<Stage>();

    Point pPos = p->GetPos();

    int chipX = pos_.x / CHA_WIDTH;
    int chipY = pos_.y / CHA_HEIGHT;

    int pChipX = pPos.x / CHA_WIDTH;
    int pChipY = pPos.y / CHA_HEIGHT;

    Node* startNode = &stage->node[chipY][chipX];
    Node* goalNode = &stage->node[pChipY][pChipX];

    BFS(startNode, goalNode);
}

void Enemy::Draw()
{
	DrawBox(pos_.x, pos_.y, pos_.x + CHA_WIDTH, pos_.y + CHA_HEIGHT, GetColor(80, 89, 10), TRUE);
}

bool Enemy::CheckHit(const Rect& me, const Rect& other)
{

	return false;
}

bool Enemy::HitToChip(int _x, int _y)
{
	int chipX = _x / CHA_WIDTH;
	int chipY = _y / CHA_HEIGHT;


	Stage stage;
	Stage* stg = &stage;

	switch (stg->GetStageData(chipX, chipY))
	{
	case STAGE_OBJ::WALL:
		return true;
	default:
		break;
	}

	return false;
}

void Enemy::BFS(Node* root, Node* goal)
{
    for (int y = 0; y < STAGE_HEIGHT; y++)
    {
        for (int x = 0; x < STAGE_WIDTH; x++)
        {
            //全部初期化
            stage->node[y][x].isOpen = false;//あいてないよー
            stage->node[y][x].parentNode = nullptr;//おやいないよー
        }
    }
    
    //幅優先ででルート探索
    
    std::queue<Node*> nodeQueue;
    nodeQueue.push(root);
    root->isOpen = true;
    
    while (!nodeQueue.empty())
    {
        Node* current = nodeQueue.front();
        nodeQueue.pop();
    
        if (current == goal)
        {
            break; //ゴールまで行ったら抜ける
        }
    
        for (int i = 0; i < 4; i++)
        {
            Node* nextNode = current->neighborNode[i];
    
            if (!nextNode || nextNode->isWall || nextNode->isOpen)
            {
                continue;
            }
            nextNode->parentNode = current;
            nextNode->isOpen = true;
            nodeQueue.push(nextNode);
        }
    }
    
    
    std::vector<Point> path;
    Node* current = goal;
    
    while (current && current != root)//currentが有効かつcurrentとstartNodeが違うときにループ
    {
        path.push_back(current->pos);
        current = current->parentNode;
    }
    
    if (!path.empty())//パスが空じゃなければ
    {
        Point nextStep = path.back();//次の移動先を決める
        nextPos_ = { nextStep.x * CHA_WIDTH, nextStep.y * CHA_HEIGHT };
    }
    
    if (pos_.x < nextPos_.x)
    {
        pos_.x += speed_;
    }
    if (pos_.x > nextPos_.x)
    {
        pos_.x -= speed_;
    }
    if (pos_.y < nextPos_.y)
    {
        pos_.y += speed_;
    }
    if (pos_.y > nextPos_.y)
    {
        pos_.y -= speed_;
    }
}


void Enemy::Dijkstra(Node* root, Node* goal)
{
    for (int y = 0; y < STAGE_HEIGHT; y++)
    {
        for (int x = 0; x < STAGE_WIDTH; x++)
        {
            stage->node[y][x].dist = 0;
            stage->node[y][x].isOpen = false;
            stage->node[y][x].parentNode = nullptr;
        }
    }

    root->dist = 0;
    root->isOpen = false;

    std::queue<Node*> nodeQueue;
    nodeQueue.push(root);

    while (!nodeQueue.empty())
    {
        Node* currentNode = nodeQueue.front();
        nodeQueue.pop();

        // ゴールに到達したら終了
        if (currentNode == goal)
        {
            break;
        }

        for (int i = 0; i < 4; i++)
        {
            Node* nextNode = currentNode->neighborNode[i];

            if (!nextNode || nextNode->isWall || nextNode->isOpen)
            {
                continue;
            }
            
            int newDist = currentNode->dist + nextNode->cost;
            if (newDist < nextNode->dist)
            {
                nextNode->parentNode = currentNode;
                nextNode->dist = newDist;
                nodeQueue.push(nextNode);
                
            }
            //nextNode->parentNode = currentNode;
            //nextNode->isOpen = true;
            //nodeQueue.push(nextNode);
            //nextNode->dist = newDist;
        }
    }

    std::vector<Point> path;
    Node* current = goal;

    while (current && current != root)
    {
        path.push_back(current->pos);
        current = current->parentNode;
    }

    if (!path.empty())//パスが空じゃなければ
    {
        Point nextStep = path.back();
        nextPos_ = { nextStep.x * CHA_WIDTH, nextStep.y * CHA_HEIGHT };
    }

    if (pos_.x < nextPos_.x)
    {
        pos_.x += speed_;
    }
    if (pos_.x > nextPos_.x)
    {
        pos_.x -= speed_;
    }
    if (pos_.y < nextPos_.y)
    {
        pos_.y += speed_;
    }
    if (pos_.y > nextPos_.y)
    {
        pos_.y -= speed_;
    }
}