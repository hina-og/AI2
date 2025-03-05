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

    //for (int y = 0; y < STAGE_HEIGHT; y++)
    //{
    //    for (int x = 0; x < STAGE_WIDTH; x++)
    //    {
    //        stage->node[y][x].isOpen = false;
    //        stage->node[y][x].parentNode = nullptr;
    //    }
    //}

    ////幅優先ででルート探索

    //std::queue<Node*> nodeQueue;
    //nodeQueue.push(startNode);
    //startNode->isOpen = true;

    //while (!nodeQueue.empty())
    //{
    //    Node* current = nodeQueue.front();
    //    nodeQueue.pop();

    //    if (current == goalNode)
    //    {
    //        break; //ゴールまで行ったら抜ける
    //    }

    //    for (int i = 0; i < 4; i++)
    //    {
    //        Node* nextNode = current->neighborNode[i];

    //        if (!nextNode || nextNode->isWall || nextNode->isOpen)
    //            continue;

    //        nextNode->parentNode = current;
    //        nextNode->isOpen = true;
    //        nodeQueue.push(nextNode);
    //    }
    //}


    //std::vector<Point> path;
    //Node* current = goalNode;

    //while (current && current != startNode)//currentが有効かつcurrentとstartNodeが違うときにループ
    //{
    //    path.push_back(current->pos);
    //    current = current->parentNode;
    //}

    //if (!path.empty())//パスが空じゃなければ
    //{
    //    Point nextStep = path.back();//次の移動先を決める
    //    nextPos_ = { nextStep.x * CHA_WIDTH, nextStep.y * CHA_HEIGHT };
    //}

    //if (pos_.x < nextPos_.x)
    //{
    //    pos_.x += speed_;
    //}
    //if (pos_.x > nextPos_.x)
    //{
    //    pos_.x -= speed_;
    //}
    //if (pos_.y < nextPos_.y)
    //{
    //    pos_.y += speed_;
    //}
    //if (pos_.y > nextPos_.y)
    //{
    //    pos_.y -= speed_;
    //}

    // ダイクストラ法で最短経路を探索
    if (Dijkstra(startNode, goalNode))
    {
        Node* currentNode = goalNode;
        while (currentNode && currentNode != startNode)
        {
            // 最短経路をたどっていく
            currentNode = currentNode->parentNode;
        }

        // 次の移動先
        if (currentNode)
        {
            nextPos_ = { currentNode->pos.x * CHA_WIDTH, currentNode->pos.y * CHA_HEIGHT };
            pos_ = nextPos_;  // 敵の位置を更新
        }
    }
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

bool Enemy::BFS(Node* root, Node* goal)
{
    if (root == goal)
    {
        pos_ = { goal->pos.x * CHA_WIDTH, goal->pos.y * CHA_HEIGHT };
        return true;
    }

    root->isOpen = true;

    for (int i = 0; i < 4; i++)
    {
        Node* nextNode = root->neighborNode[i];

        if (!nextNode || nextNode->isWall || nextNode->isOpen)
        {
            continue;
        }

        pos_ = { nextNode->pos.x * CHA_WIDTH, nextNode->pos.y * CHA_HEIGHT };

        nextNode->parentNode = root;
        nextNode->isOpen = true;

        if (BFS(nextNode, goal))
        {
            return true;
        }
    }

    return false;
}


bool Enemy::Dijkstra(Node* startNode, Node* goalNode)
{
    // ダイクストラ法では、最小コストを持つノードを優先的に処理するために、優先度付きキューを使います
    struct CompareNode
    {
        bool operator()(Node* a, Node* b)
        {
            return a->cost > b->cost;  // コストが大きい方を後に処理
        }
    };

    std::priority_queue<Node*, std::vector<Node*>, CompareNode> pq;

    // 初期化
    for (int y = 0; y < STAGE_HEIGHT; y++)
    {
        for (int x = 0; x < STAGE_WIDTH; x++)
        {
            stage->node[y][x].cost = std::numeric_limits<float>::infinity();  // 最初は無限大に設定
            stage->node[y][x].parentNode = nullptr;  // 親ノードを初期化
            stage->node[y][x].isOpen = false;  // ノードが開いているかどうか
        }
    }

    startNode->cost = 0;  // スタートノードのコストは0
    pq.push(startNode);

    while (!pq.empty())
    {
        Node* currentNode = pq.top();
        pq.pop();

        if (currentNode == goalNode)
        {
            return true;  // ゴールに到達したら終了
        }

        // 隣接ノードを探索
        for (int i = 0; i < 4; i++)
        {
            Node* neighbor = currentNode->neighborNode[i];
            if (!neighbor || neighbor->isWall) continue;  // 壁や無効なノードは無視

            float newCost = currentNode->cost + 1;  // 隣接ノードへの移動コストを計算（ここでは仮に1とする）

            // もし新しいコストが現在のコストより小さい場合、コストを更新
            if (newCost < neighbor->cost)
            {
                neighbor->cost = newCost;
                neighbor->parentNode = currentNode;  // 親ノードを設定
                pq.push(neighbor);  // 更新したノードを優先度付きキューに追加
            }
        }
    }

    return false;  // ゴールに到達できなかった場合
}