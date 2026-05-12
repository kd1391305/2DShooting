#include "GameOverScene.h"
#include"../../Chara/Player/Player.h"
#include"../../Chara/Enemy/EnemyManager.h"
#include"../../Fireworks/FireworksManager.h"
#include"../../Bullet/BulletManager.h"
#include"../../Background/Back.h"
#include"../../main.h"
#include"../../Timer/Timer.h"
#include"../../Tools/Button/Button.h"
#include"../../Key/KeyManager.h"
#include"../SceneManager.h"
#include"../GameScene/GameScene.h"
#include"../TitleScene/TitleScene.h"
#include"../../Mouse/Mouse.h"
#include"../../TextureCache/TextureCache.h"

void GameOverScene::Init()
{
	m_color = { 0,0,0,0 };

	m_gameButton = std::make_shared<Button>();
	m_gameButton->SetPos({ 150,-100 });						//座標
	m_gameButton->SetRadius({ 110,40 });					//半径
	m_gameButton->SetColor({ 0.2f,0.2f,0.2f,1.0f });		//通常の色	
	m_gameButton->SetSelectScale({ 1.01f,1.01f });			//選択時の拡縮
	m_gameButton->SetSelectColor({ 0.4f,0.4f,0.4f,1.0f });	//選択時の色

	m_titleButton = std::make_shared<Button>();
	m_titleButton->SetPos({ -150,-100 });					//座標
	m_titleButton->SetRadius({110,40 });					//半径
	m_titleButton->SetColor({ 0.2f,0.2f,0.2f,1.0f });		//通常の色	
	m_titleButton->SetSelectScale({1.01f,1.01f});			//選択時の拡縮
	m_titleButton->SetSelectColor({0.4f,0.4f,0.4f,1.0f });	//選択時の色

	//ゲームに戻るボタンを選択中でゲームオーバーシーンを始める
	m_gameButton->SetSelect(true);

	m_game->GetBack()->Update(0);

	MOUSE.ShowCursorTex(true);
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
		if ((KEY.IsDown(VK_LBUTTON) && m_gameButton->GetActionType() == Button::ActionType::Mouse) ||
			(KEY.IsDown(VK_RETURN)))
		{
			SceneManager::Instance().ChangeState(m_game);
			m_game->GetPlayer()->Init();
			m_game->GetPlayer()->Invincible(1.0f);
			m_game->SetContinueFlg(true);
			MOUSE.ShowCursorTex(false);
		}
	}

	m_titleButton->Update();
	if (m_titleButton->IsSelect())
	{
		if ((KEY.IsDown(VK_LBUTTON) && m_titleButton->GetActionType() == Button::ActionType::Mouse) ||
			(KEY.IsDown(VK_RETURN)))
		{
			SceneManager::Instance().ChangeState(std::make_shared<TitleScene>(m_game->GetBack()));
		}
	}

	if (KEY.IsDown(VK_RIGHT))
	{
		m_titleButton	->SetSelect(false);
		m_gameButton	->SetSelect(true);
	}

	if (KEY.IsDown(VK_LEFT))
	{
		m_titleButton->SetSelect(true);
		m_gameButton->SetSelect(false);
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

	Math::Matrix scaleMat, transMat;
	{
		//ゲームオーバー
		scaleMat = Math::Matrix::CreateScale(0.5f);
		transMat = Math::Matrix::CreateTranslation(0, 200,0);
		SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
		SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/UI/GameOver.png"));

		//タイトルへ
		scaleMat = Math::Matrix::CreateScale(0.18f);
		transMat = Math::Matrix::CreateTranslation(m_titleButton->GetPos().x, m_titleButton->GetPos().y, 0);
		SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
		SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/UI/GoToTitle.png"));

		//コンティニュー
		scaleMat = Math::Matrix::CreateScale(0.18f);
		transMat = Math::Matrix::CreateTranslation(m_gameButton->GetPos().x, m_gameButton->GetPos().y, 0);
		SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
		SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/UI/Continue.png"));
	}
}

void GameOverScene::Release()
{

}

