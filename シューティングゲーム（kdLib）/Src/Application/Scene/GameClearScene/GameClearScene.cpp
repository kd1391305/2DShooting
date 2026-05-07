#include "GameClearScene.h"
#include"../../Background/Back.h"
#include"../../Fireworks/FireworksManager.h"
#include"../../Timer/Timer.h"
#include"../../Tools/Button/Button.h"
#include"../../Key/KeyManager.h"
#include"../SceneManager.h"
#include"../TitleScene/TitleScene.h"
#include"../GameScene/GameScene.h"
#include"../../Font/DWriteCustom.h"
#include"../../SoundCache/SoundCache.h"
#include"../../TextureCache/TextureCache.h"
#include"../../main.h"
#include"../../Tools/RandEx/RandEx.h"

void GameClearScene::Init()
{
	m_gameButton = std::make_shared<ButtonEx>();
	m_gameButton->SetPos({ 200,-280 });						//座標
	m_gameButton->SetRadius({ 110,50 });					//半径
	m_gameButton->SetColor({ 0.2f,0.2f,0.2f,0.7f });		//通常の色	
	m_gameButton->SetSelectScale({ 1.01f,1.01f });			//選択時の拡縮
	m_gameButton->SetSelectColor({ 0.4f,0.4f,0.4f,0.7f });	//選択時の色
	m_gameButton->SetTex("Texture/Button/ButtonBack.png");

	m_titleButton = std::make_shared<ButtonEx>();
	m_titleButton->SetPos({ -200,-280 });					//座標
	m_titleButton->SetRadius({ 110,50 });					//半径
	m_titleButton->SetColor({ 0.2f,0.2f,0.2f,0.7f });		//通常の色	
	m_titleButton->SetSelectScale({ 1.01f,1.01f });			//選択時の拡縮
	m_titleButton->SetSelectColor({ 0.4f,0.4f,0.4f,0.7f });	//選択時の色
	m_titleButton->SetTex("Texture/Button/ButtonBack.png");

	//BGMを流す
	std::shared_ptr<KdSoundInstance> bgm = SoundCache::Instance().Get("Sound/BGM/yukyunotokie.wav");
	bgm->SetVolume(0.001f);
	bgm->Play(true);

	m_shotCoolTimer = 0;
}

void GameClearScene::Update()
{
	const float deltaTime = Timer::Instance().GetDeltaTime();

	m_back->Update(deltaTime);

	m_gameButton->Update();
	if (m_gameButton->IsSelect())
	{
		if ((KEY.IsDown(VK_LBUTTON) && m_gameButton->GetActionType() == Button::ActionType::Mouse) ||
			(KEY.IsDown(VK_RETURN)))
		{
			SceneManager::Instance().ChangeState(std::make_shared<Game>(m_back));
		}
	}

	m_titleButton->Update();
	if (m_titleButton->IsSelect())
	{
		if ((KEY.IsDown(VK_LBUTTON) && m_titleButton->GetActionType() == Button::ActionType::Mouse) ||
			(KEY.IsDown(VK_RETURN)))
		{
			SceneManager::Instance().ChangeState(std::make_shared<TitleScene>(m_back));
		}
	}

	if (KEY.IsDown(VK_RIGHT))
	{
		m_titleButton->SetSelect(false);
		m_gameButton->SetSelect(true);
	}

	if (KEY.IsDown(VK_LEFT))
	{
		m_titleButton->SetSelect(true);
		m_gameButton->SetSelect(false);
	}

	//花火を打ち上げるランダムで
	m_shotCoolTimer -= deltaTime;
	if (m_shotCoolTimer <= 0)
	{
		{
			int shotNum = 12;
			int noise = randRange(-4, 4);
			shotNum += noise;
			FireworksManager::Type type = m_back->GetFireworks()->GetRandomType_Quick();
			for (int i = 0; i < shotNum; i++)
			{
				//タイトル名と被らないようにタイトルの横にする
				float startX = randRange(SCREEN_LEFT, SCREEN_LEFT + 250);
				if (i % 2)startX *= -1;

				Math::Vector2 startPos = { startX,SCREEN_BOTTOM - 30 - randRange(0,200) };
				Math::Vector2 startMove = { 0,randRange(270,370) };
				float beforeScale = randRange(0.3f, 0.5f);
				float afterScale = randRange(0.7f, 1.1f);
				Math::Color color = { randRange(0,1),randRange(0.0f,1),randRange(0.0f,1),randRange(0.5f,1) };
				Math::Color color2 = { randRange(0,0.8f),randRange(0.0f,0.8f),randRange(0.0f,0.8f),randRange(0.5f,0.7f) };
			
				for (int i = 0; i < 6; i++)
				{
					m_back->GetFireworks()->Shoot(type,
						startPos, startMove, beforeScale, afterScale, color);
				}
				//３重花火にする（花火の中にちいさな花火を追加）
				m_back->GetFireworks()->Shoot(type,
					startPos, startMove, beforeScale, afterScale / 1.2, color);
				m_back->GetFireworks()->Shoot(type,
					startPos, startMove, beforeScale, afterScale / 1.5, color);
			}
		}
		//クールタイムを設ける
		
		m_shotCoolTimer = randRange(8, 16);

		//タイトル専用の花火効果音を流す
		std::shared_ptr<KdSoundInstance> se = SoundCache::Instance().Get("Sound/SE/Fireworks_Title.wav");
		se->SetVolume(0.05f);
		se->Play(false);
	}
}

void GameClearScene::Draw()
{
	m_back->Draw();
	{
		Math::Matrix scaleMat = Math::Matrix::CreateScale(1.2f, 1.2f, 0);
		Math::Matrix transMat = Math::Matrix::CreateTranslation(0, 150, 0);
		SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
		SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/GameClear/GameClear.png"));
	}

	m_titleButton->Draw();
	DWriteCustom::Instance().Draw("タイトルへ", { -300 ,- 252 },40);
	m_gameButton->Draw();
	DWriteCustom::Instance().Draw("もう一度", { 110, -252 },40);
}

void GameClearScene::Release()
{
}
