#include "BootScene.h"
#include "../Library/SceneManager.h"

BootScene::BootScene()
{
}

BootScene::~BootScene()
{
}

void BootScene::Update()
{
	SceneManager::ChangeScene("PLAY"); // ‹N“®‚ªI‚í‚Á‚½‚çTitle‚ğ•\¦
}

void BootScene::Draw()
{
}
