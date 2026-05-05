#include "GameOverScene.h"
#include"../../Chara/Player/Player.h"
#include"../../Chara/Enemy/EnemyManager.h"
#include"../../Fireworks/FireworksManager.h"
#include"../../Bullet/BulletManager.h"
#include"../../Background/Back.h"
#include"../../Font/DWriteCustom.h"
#include"../../main.h"
#include"../../Timer/Timer.h"
#include"../../Tools/Button/Button.h"
#include"../../Key/KeyManager.h"
#include"../SceneManager.h"
#include"../GameScene/GameScene.h"
#include"../TitleScene/TitleScene.h"

void GameOverScene::Init()
{
	m_color = { 0,0,0,0 };

	m_gameButton = std::make_shared<Button>();
	m_gameButton->SetPos({ 150,-100 });						//座標
	m_gameButton->SetRadius({ 100,50 });					//半径
	m_gameButton->SetColor({ 0.2f,0.2f,0.2f,1.0f });		//通常の色	
	m_gameButton->SetSelectScale({ 1.01f,1.01f });			//選択時の拡縮
	m_gameButton->SetSelectColor({ 0.4f,0.4f,0.4f,1.0f });	//選択時の色

	m_titleButton = std::make_shared<Button>();
	m_titleButton->SetPos({ -150,-100 });					//座標
	m_titleButton->SetRadius({100,50 });					//半径
	m_titleButton->SetColor({ 0.2f,0.2f,0.2f,1.0f });		//通常の色	
	m_titleButton->SetSelectScale({1.01f,1.01f});			//選択時の拡縮
	m_titleButton->SetSelectColor({0.4f,0.4f,0.4f,1.0f });	//選択時の色
}

void GameOverScene::Update()
{
	float deltaTime = Timer::Instance().GetDeltaTime();

	m_color.A(m_color.A() + m_deltaAlpha * deltaTime);
	if (m_color.A() >= m_maxAlpha)
	{
		m_color.A(m_maxAlpha);
	}

	m_gameButton->Update();
	if (m_gameButton->IsSelect())
	{
		if (KEY.IsDown(VK_LBUTTON))
		{
			SceneManager::Instance().ChangeState(m_game);
		}
	}

	m_titleButton->Update();
	if (m_titleButton->IsSelect())
	{
		if (KEY.IsDown(VK_LBUTTON))
		{
			SceneManager::Instance().ChangeState(std::make_shared<TitleScene>(m_game->GetBack()));
		}
	}
}

void GameOverScene::Draw()
{
	m_game->GetBack()->Draw();
	m_game->GetPlayer()->Draw();
	m_game->GetBulletManager()->Draw();
	m_game->GetEnemyManager()->Draw();

	//画面全体に薄く黒色をかぶせる処理
	SHADER.m_spriteShader.ClearMatrix();
	SHADER.m_spriteShader.DrawBox(0, 0, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, &m_color, true);

	m_gameButton->Draw();
	m_titleButton->Draw();

	float posY = -80;

	DWriteCustom::Instance().SetShadow({ -2. - 2 }, { 0.8f,0.2f,0.2f,0.8f });
	DWriteCustom::Instance().Draw("Game Over", { -120,100 }, 100, { 0.8f,0.8f,0.8f,1.0f });
	DWriteCustom::Instance().SetShadow({}, {});
	DWriteCustom::Instance().Draw("タイトルへ", Math::Vector2{ -240,posY }, 30);
	DWriteCustom::Instance().Draw("リトライ", { 60,posY }, 30);
}

void GameOverScene::Release()
{

}

