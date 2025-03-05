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

    ////���D��łŃ��[�g�T��

    //std::queue<Node*> nodeQueue;
    //nodeQueue.push(startNode);
    //startNode->isOpen = true;

    //while (!nodeQueue.empty())
    //{
    //    Node* current = nodeQueue.front();
    //    nodeQueue.pop();

    //    if (current == goalNode)
    //    {
    //        break; //�S�[���܂ōs�����甲����
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

    //while (current && current != startNode)//current���L������current��startNode���Ⴄ�Ƃ��Ƀ��[�v
    //{
    //    path.push_back(current->pos);
    //    current = current->parentNode;
    //}

    //if (!path.empty())//�p�X���󂶂�Ȃ����
    //{
    //    Point nextStep = path.back();//���̈ړ�������߂�
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

    // �_�C�N�X�g���@�ōŒZ�o�H��T��
    if (Dijkstra(startNode, goalNode))
    {
        Node* currentNode = goalNode;
        while (currentNode && currentNode != startNode)
        {
            // �ŒZ�o�H�����ǂ��Ă���
            currentNode = currentNode->parentNode;
        }

        // ���̈ړ���
        if (currentNode)
        {
            nextPos_ = { currentNode->pos.x * CHA_WIDTH, currentNode->pos.y * CHA_HEIGHT };
            pos_ = nextPos_;  // �G�̈ʒu���X�V
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
    // �_�C�N�X�g���@�ł́A�ŏ��R�X�g�����m�[�h��D��I�ɏ������邽�߂ɁA�D��x�t���L���[���g���܂�
    struct CompareNode
    {
        bool operator()(Node* a, Node* b)
        {
            return a->cost > b->cost;  // �R�X�g���傫��������ɏ���
        }
    };

    std::priority_queue<Node*, std::vector<Node*>, CompareNode> pq;

    // ������
    for (int y = 0; y < STAGE_HEIGHT; y++)
    {
        for (int x = 0; x < STAGE_WIDTH; x++)
        {
            stage->node[y][x].cost = std::numeric_limits<float>::infinity();  // �ŏ��͖�����ɐݒ�
            stage->node[y][x].parentNode = nullptr;  // �e�m�[�h��������
            stage->node[y][x].isOpen = false;  // �m�[�h���J���Ă��邩�ǂ���
        }
    }

    startNode->cost = 0;  // �X�^�[�g�m�[�h�̃R�X�g��0
    pq.push(startNode);

    while (!pq.empty())
    {
        Node* currentNode = pq.top();
        pq.pop();

        if (currentNode == goalNode)
        {
            return true;  // �S�[���ɓ��B������I��
        }

        // �אڃm�[�h��T��
        for (int i = 0; i < 4; i++)
        {
            Node* neighbor = currentNode->neighborNode[i];
            if (!neighbor || neighbor->isWall) continue;  // �ǂ△���ȃm�[�h�͖���

            float newCost = currentNode->cost + 1;  // �אڃm�[�h�ւ̈ړ��R�X�g���v�Z�i�����ł͉���1�Ƃ���j

            // �����V�����R�X�g�����݂̃R�X�g��菬�����ꍇ�A�R�X�g���X�V
            if (newCost < neighbor->cost)
            {
                neighbor->cost = newCost;
                neighbor->parentNode = currentNode;  // �e�m�[�h��ݒ�
                pq.push(neighbor);  // �X�V�����m�[�h��D��x�t���L���[�ɒǉ�
            }
        }
    }

    return false;  // �S�[���ɓ��B�ł��Ȃ������ꍇ
}