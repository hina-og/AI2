#include "Enemy.h"
#include "./Stage.h"
#include "globals.h"
#include "Player.h"

Enemy::Enemy()
	:pos_({ CHA_WIDTH * 19,CHA_HEIGHT * 9 }), isAlive_(true)
{
	nextPos_ = pos_;
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	Point move = { 0,0 };

	Player* p = (Player*)FindGameObject<Player>();
	Point pPos = p->GetPos();

	Stage* stage = (Stage*)FindGameObject<Stage>();

	int chipX = pos_.x / CHA_WIDTH;
	int chipY = pos_.y / CHA_HEIGHT;

	int pChipX = pPos.x / CHA_WIDTH;
	int pChipY = pPos.y / CHA_HEIGHT;

	while (true)
	{
		stage->node[chipY - 1][chipX];
	}

    pos_ = { pos_.x + move.x, pos_.y + move.y };
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
