#include "GameOverScene.h"
#include"../../Chara/Player/Player.h"
#include"../../Chara/Enemy/EnemyManager.h"
#include"../../Fireworks/FireworksManager.h"
#include"../../Bullet/BulletManager.h"
#include"../../Background/Back.h"
#include"../../Font/DWriteCustom.h"
#include"../../main.h"
#include"../../Timer/Timer.h"

GameOverScene::GameOverScene(std::shared_ptr<Player> player,
	std::shared_ptr<EnemyManager> enemy,
	std::shared_ptr<FireworksManager> fireworks,
	std::shared_ptr<BulletManager> bullet,
	std::shared_ptr<Back> back) 
{
	m_player = player;
	m_enemyManager = enemy;
	m_fireworksManager = fireworks;
	m_bulletManager = bullet;
	m_back = back;
}

void GameOverScene::Init()
{
	m_color = { 0,0,0,0 };
}

void GameOverScene::Update()
{
	float deltaTime = Timer::Instance().GetDeltaTime();

	m_color.A(m_color.A() + m_deltaAlpha * deltaTime);
	if (m_color.A() >= m_maxAlpha)
	{
		m_color.A(m_maxAlpha);
	}
}

void GameOverScene::Draw()
{
	m_back->Draw();
	m_player->Draw();
	m_enemyManager->Draw();
	m_fireworksManager->Draw();
	m_bulletManager->Draw();

	SHADER.m_spriteShader.ClearMatrix();
	SHADER.m_spriteShader.DrawBox(0, 0, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, &m_color, true);

	DWriteCustom::Instance().Draw("リトライ", {});
	DWriteCustom::Instance().Draw("タイトルへ",Math::Vector2{0, -200 });
}

void GameOverScene::Release()
{

}

