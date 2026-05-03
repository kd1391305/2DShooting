#include"TitleScene.h"
#include"../../Font/DWriteCustom.h"
#include"../SceneManager.h"
#include"../GameScene/GameScene.h"
#include"../../Key/KeyManager.h"
#include"../../Fireworks/FireworksManager.h"
#include"../../Mouse/Mouse.h"
#include"../../main.h"
#include"../../Background/Back.h"
#include"../../Timer/Timer.h"
#include"../../Tools/Button/Button.h"
#include"../../Tools/RandEx/RandEx.h"
#include"../../TextureCache/TextureCache.h"
#include"../../SoundCache/SoundCache.h"

TitleScene::TitleScene(std::shared_ptr<Back> back)
{
	m_back = back;
	m_back->StartZoomOut();
}

void TitleScene::Init()
{
	m_namePos = { 0,200 };
	Math::Matrix scale = Math::Matrix::CreateScale(2.0, 2.0f, 0);
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_namePos.x, m_namePos.y, 0);
	m_nameMat = scale * trans;

	if (!m_start)
	{
		m_start = std::make_shared<ButtonEx>();
	}
	m_start->SetPos({ 0,-100 });					//座標
	m_start->SetRadius({ 210,80 });			//半径
	m_start->SetColor({ 1,1,1,1 });			//通常の色	
	m_start->SetSelectScale({ 1.01f,1.01f });	//選択時の拡縮
	m_start->SetSelectColor({ 1,1,0.8,1 });	//選択時の色
	m_start->SetTex("Texture/Start.png");

	if (!m_back)
	{
		m_back = std::make_shared<Back>();
		m_back->Init();
	}

	if (!m_fireworksManager)
	{
		m_fireworksManager = std::make_shared<FireworksManager>();
	}

	m_shotWaitTimer = 2;
	m_shotWait = 6;

	//BGMを流す
	std::shared_ptr<KdSoundInstance> temp = SoundCache::Instance().Get("Sound/BGM/Planetarium.wav");
	temp->SetVolume(0.1f);
	temp->Play(true);

}

void TitleScene::Update()
{
	float deltaTime = Timer::Instance().GetDeltaTime();

	//背景の更新
	if(m_back)m_back->Update(deltaTime);

	//花火の更新
	m_fireworksManager->Update(deltaTime);

	//花火を打ち上げるランダムで
	m_shotWaitTimer -= deltaTime;
	if (m_shotWaitTimer <= 0)
	{
		{
			int shotNum = 10;
			int noise = randRange(0, 5);
			shotNum += noise;
			for (int i = 0; i < shotNum; i++)
			{
				//タイトル名と被らないようにタイトルの横にする
				float startX = randRange(SCREEN_WIDTH / 4.0f, SCREEN_RIGHT);
				if (rand() % 2)startX *= -1;

				Math::Vector2 startPos = { startX,SCREEN_BOTTOM - 30 - randRange(0,200) };
				Math::Vector2 startMove = { 0,randRange(250,320) };
				float beforeScale = randRange(0.7f, 1.3f);
				float afterScale = randRange(0.7f, 1.3f);
				Math::Color color = { randRange(0.0f,0.6f),randRange(0.0f,0.6f),randRange(0.0f,0.6f),randRange(0.4f,0.6f) };
				m_fireworksManager->Shot((FireworksManager::Type)(rand() % FireworksManager::Type::Kind), 
					startPos, startMove, beforeScale, afterScale, color);
			}
		}

		//クールタイムを設ける
		float noise = randRange(0, 3);
		m_shotWaitTimer = m_shotWait + noise;
	}

	//スタートボタンの更新
	m_start->Update();
	if (m_start->IsSelect())
	{
		if (KEY.IsDown(VK_LBUTTON))
		{
			//ゲーム画面へ移行
			SceneManager::Instance().ChangeState(new Game(m_back, m_fireworksManager));
		}
	}
}

void TitleScene::Draw()
{
	if (m_back)m_back->Draw();

	m_start->Draw();

	m_fireworksManager->Draw();

	SHADER.m_spriteShader.SetMatrix(m_nameMat);
	SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/Title.png"));
}

void TitleScene::Release()
{
	
}