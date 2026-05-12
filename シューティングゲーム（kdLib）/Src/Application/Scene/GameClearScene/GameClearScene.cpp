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
#include"../../Mouse/Mouse.h"

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
	bgm->SetVolume(0.01f);
	bgm->Play(true);

	m_shotCoolTimer = 8;

	MOUSE.ShowCursorTex(true);
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
			std::shared_ptr<KdSoundInstance> bgm = SoundCache::Instance().Get("Sound/BGM/yukyunotokie.wav");
			bgm->Stop();
		}
	}

	m_titleButton->Update();
	if (m_titleButton->IsSelect())
	{
		if ((KEY.IsDown(VK_LBUTTON) && m_titleButton->GetActionType() == Button::ActionType::Mouse) ||
			(KEY.IsDown(VK_RETURN)))
		{
			SceneManager::Instance().ChangeState(std::make_shared<TitleScene>(m_back));
			std::shared_ptr<KdSoundInstance> bgm = SoundCache::Instance().Get("Sound/BGM/yukyunotokie.wav");
			bgm->Stop();
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
			int shotNum = 10;
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
				float afterScale = randRange(0.4f, 0.6f);
				Math::Color color = { randRange(0,1),randRange(0.0f,1),randRange(0.0f,1),randRange(0.5f,1) };
				Math::Color color2 = { randRange(0,0.8f),randRange(0.0f,0.8f),randRange(0.0f,0.8f),randRange(0.5f,0.7f) };
			
				for (int j = 0; j < 2; j++)
				{
					m_back->GetFireworks()->Shoot(type,
						startPos, startMove, beforeScale, afterScale, color);
				}
				//３重花火にする（花火の中にちいさな花火を追加）
				m_back->GetFireworks()->Shoot(type,
					startPos, startMove, beforeScale, afterScale / 1.2, color);
				m_back->GetFireworks()->Shoot(type,
					startPos, startMove, beforeScale, afterScale / 1.4, color);
			}
		}
		//クールタイムを設ける
		m_shotCoolTimer = randRange(10, 16);

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
		Math::Matrix transMat = Math::Matrix::CreateTranslation(0, 200, 0);
		SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
		SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/GameClear/GameClear.png"));
	}

	DWriteCustom::Instance().SetShadow({ -1,-1 }, { 0,1,1,1 });
	//スコアを描画
	{
		DWriteCustom::Instance().SetFontSize(40);
		DWriteCustom::Instance().Draw("打ち上げ数 :", { -320,80 });
		DWriteCustom::Instance().Draw("得点 :", { -200,-20 });
		DWriteCustom::Instance().Draw("最高得点 :", { -280,-120 });

		DWriteCustom::Instance().Draw("回", { 390,80 });
		DWriteCustom::Instance().Draw("点", { 390,-20 });
		DWriteCustom::Instance().Draw("点", { 390,-120 });
	}
	{
		DWriteCustom::Instance().ChangeFont(FontName::Orbitron);
		DWriteCustom::Instance().SetFontSize(55);
		DrawScore(m_explodeNum, { -30,88 });
		DrawScore(m_score, { -30,-12 });
		DrawScore(m_highScore, { -30,-112 });
		DWriteCustom::Instance().ChangeFont(FontName::KleeOne);
	}

	DWriteCustom::Instance().SetShadow({},{});
	m_titleButton->Draw();
	DWriteCustom::Instance().Draw("タイトルへ", { -300 ,- 252 },40);
	m_gameButton->Draw();
	DWriteCustom::Instance().Draw("もう一度", { 110, -252 },40);
}

void GameClearScene::DrawScore(int score, Math::Vector2 pos)
{
	bool drawFlg = false;
	char scoreStr[10];
	sprintf_s(scoreStr, sizeof(scoreStr), "%.8ld", score);
	float scorePosY = -160;
	//一桁ずつ描画
	for (int i = 0; i < 8; i++)
	{
		if (!drawFlg && scoreStr[i] == '0' && i != 7)continue;
		else drawFlg = true;

		std::string digit;
		digit = scoreStr[i];
		if (scoreStr[i] == '1')
		{
			//+4右にずらす
			DWriteCustom::Instance().Draw(digit, { pos.x + i * 50.0f + 5,pos.y });
		}
		else
		{
			DWriteCustom::Instance().Draw(digit, { pos.x + i * 50.0f,	pos.y });
		}
	}
}

void GameClearScene::Release()
{
}
