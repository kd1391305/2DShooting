#include "GameClearScene.h"
#include"../../Background/Back.h"
#include"../../Fireworks/FireworksManager.h"
#include"../../Timer/Timer.h"

void GameClearScene::Init()
{
}

void GameClearScene::Update()
{
	const float deltaTime = Timer::Instance().GetDeltaTime();

	m_back->Update(deltaTime);
	m_fireworksManager->Update(deltaTime);
}

void GameClearScene::Draw()
{
	m_back->Draw();
	m_fireworksManager->Draw();
}

void GameClearScene::Release()
{
}
