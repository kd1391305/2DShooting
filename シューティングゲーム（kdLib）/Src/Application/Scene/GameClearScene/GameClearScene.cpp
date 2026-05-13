#include "GameClearScene.h"
#include"../../Background/Back.h"
#include"../../Fireworks/FireworksManager.h"
#include"../../Timer/Timer.h"
#include"../../Tools/Button/Button.h"
#include"../../Key/KeyManager.h"
#include"../SceneManager.h"
#include"../TitleScene/TitleScene.h"
#include"../GameScene/GameScene.h"
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
	bgm->SetVolume(0.01f);
	bgm->Play(true);

	m_shotCoolTimer = 8;

	m_gameButton->SetSelect(true);
}

void GameClearScene::Update()
{
	const float deltaTime = Timer::Instance().GetDeltaTime();

	m_back->Update(deltaTime);

	m_gameButton->Update();
	if (m_gameButton->IsSelect())
	{
		if (KEY.IsDown(VK_RETURN))
		{
			SceneManager::Instance().ChangeState(std::make_shared<Game>(m_back));
			std::shared_ptr<KdSoundInstance> bgm = SoundCache::Instance().Get("Sound/BGM/yukyunotokie.wav");
			bgm->Stop();
		}
	}

	m_titleButton->Update();
	if (m_titleButton->IsSelect())
	{
		if (KEY.IsDown(VK_RETURN))
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
		SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/UI/GameClear.png"));
	}

	//スコアを描画
	float explodeNumPosY = 60;
	float scorePosY		 = -40;
	float highScorePosY	 = -140;
	{
		Math::Matrix scaleMat, transMat;
		//打ち上げ数
		scaleMat = Math::Matrix::CreateScale(0.25f);
		transMat = Math::Matrix::CreateTranslation(-260, explodeNumPosY, 0);
		SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
		SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/UI/ExplodeNum.png"));

		//得点
		transMat = Math::Matrix::CreateTranslation(-200, scorePosY, 0);
		SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
		SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/UI/Score.png"));

		//ハイスコア
		transMat = Math::Matrix::CreateTranslation(-240, highScorePosY, 0);
		SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
		SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/UI/HighScore.png"));


		//回テキスト
		transMat = Math::Matrix::CreateTranslation(400, explodeNumPosY, 0);
		SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
		SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/UI/kai.png"));

		//点テキスト
		{
			KdTexture* tex = TextureCache::Instance().Get("Texture/UI/ten.png").get();
			transMat = Math::Matrix::CreateTranslation(400, scorePosY, 0);
			SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
			SHADER.m_spriteShader.DrawTex_Src(tex);
			transMat = Math::Matrix::CreateTranslation(400, highScorePosY, 0);
			SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
			SHADER.m_spriteShader.DrawTex_Src(tex);
		}

		//コロンテキスト
		{
			float colonPosX = -110;

			KdTexture* tex = TextureCache::Instance().Get("Texture/UI/colon.png").get();
			transMat = Math::Matrix::CreateTranslation(colonPosX, explodeNumPosY, 0);
			SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
			SHADER.m_spriteShader.DrawTex_Src(tex);
			transMat = Math::Matrix::CreateTranslation(colonPosX, scorePosY, 0);
			SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
			SHADER.m_spriteShader.DrawTex_Src(tex);
			transMat = Math::Matrix::CreateTranslation(colonPosX, highScorePosY, 0);
			SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
			SHADER.m_spriteShader.DrawTex_Src(tex);
		}
	}
	{
		float offsetY = 3;
		DrawScore(m_explodeNum, { -30,explodeNumPosY + offsetY });
		DrawScore(m_score, { -30,scorePosY + offsetY });
		DrawScore(m_highScore, { -30,highScorePosY + offsetY });
	}

	{
		//タイトルボタン
		m_titleButton->Draw();
		Math::Matrix scaleMat, transMat;
		scaleMat = Math::Matrix::CreateScale(0.22f);
		transMat = Math::Matrix::CreateTranslation(m_titleButton->GetPos().x, m_titleButton->GetPos().y, 0);
		SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
		SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/UI/GoToTitle.png"));

		//ゲームに戻るボタン
		m_gameButton->Draw();
		transMat = Math::Matrix::CreateTranslation(m_gameButton->GetPos().x, m_gameButton->GetPos().y, 0);
		SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
		SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/UI/Retry.png"));
	}
}

void GameClearScene::DrawScore(int score, Math::Vector2 pos)
{
	bool drawFlg = false;
	char scoreStr[10];
	char path[50];
	Math::Matrix scaleMat, transMat;
	scaleMat = Math::Matrix::CreateScale(0.4f);
	sprintf_s(scoreStr, sizeof(scoreStr), "%.8ld", score);
	float scorePosY = -160;
	//一桁ずつ描画
	for (int i = 0; i < 8; i++)
	{
		if (!drawFlg && scoreStr[i] == '0' && i != 7)continue;
		else drawFlg = true;
		sprintf_s(path, sizeof(path), "Texture/UI/Digit/%c.png", scoreStr[i]);
		transMat = Math::Matrix::CreateTranslation(pos.x + 50 * i, pos.y, 0);
		SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
		SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get(path));
	}
}

void GameClearScene::Release()
{
}
