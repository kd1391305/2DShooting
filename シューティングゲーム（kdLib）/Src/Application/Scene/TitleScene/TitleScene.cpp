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

	m_shotCoolTimer = 2;
	m_shotCoolTime = 10;

	//BGMを流す
	std::shared_ptr<KdSoundInstance> bgm = SoundCache::Instance().Get("Sound/BGM/yukyunotokie.wav");
	bgm->SetVolume(0.01f);
	bgm->Play(true);

	m_bChangeScene = false;
}

void TitleScene::Update()
{
	float deltaTime = Timer::Instance().GetDeltaTime();

	//背景の更新
	if (m_back)m_back->Update(deltaTime);

	//シーン切り替えフラグが立っているとき
	if (m_bChangeScene)
	{
		m_changeSceneWaitTimer -= deltaTime;
		if (m_changeSceneWaitTimer <= 0)
		{
			//ゲーム画面へ移行
			SceneManager::Instance().ChangeState(std::make_shared<Game>(m_back));
		}
	}
	else
	{

		//花火を打ち上げるランダムで
		m_shotCoolTimer -= deltaTime;
		if (m_shotCoolTimer <= 0)
		{
			{
				int shotNum = 15;
				int noise = randRange(-2, 2);
				shotNum += noise;
				for (int i = 0; i < shotNum; i++)
				{
					//タイトル名と被らないようにタイトルの横にする
					float startX = randRange(SCREEN_WIDTH / 4.0f, SCREEN_RIGHT / 0.8f);
					if (i % 2)startX *= -1;

					Math::Vector2 startPos = { startX,SCREEN_BOTTOM - 30 - randRange(0,200) };
					Math::Vector2 startMove = { 0,randRange(270,370) };
					float beforeScale = randRange(0.3f, 0.5f);
					float afterScale = randRange(0.7f, 1.3f);
					Math::Color color = { randRange(0,0.8f),randRange(0.0f,0.8f),randRange(0.0f,0.8f),randRange(0.5f,0.7f) };
					Math::Color color2 = { randRange(0,0.8f),randRange(0.0f,0.8f),randRange(0.0f,0.8f),randRange(0.5f,0.7f) };
					FireworksManager::Type type = m_back->GetFireworks()->GetRandomType_Quick();
					for (int i = 0; i < 3; i++)
					{
						m_back->GetFireworks()->Shoot(type,
							startPos, startMove, beforeScale, afterScale, color);
					}
					//二重花火にする（花火の中にちいさな花火を追加）
					m_back->GetFireworks()->Shoot(type,
						startPos, startMove, beforeScale, afterScale / 1.5f, color);
				}
			}
			//クールタイムを設ける
			float noise = randRange(0, 6);
			m_shotCoolTimer = m_shotCoolTime + noise;

			//タイトル専用の花火効果音を流す
			std::shared_ptr<KdSoundInstance> se = SoundCache::Instance().Get("Sound/SE/Fireworks_Title.wav");
			se->SetVolume(0.05f);
			se->Play(false);
		}

		//スタートボタンの更新
		m_start->Update();
		if (m_start->IsSelect())
		{
			if (!m_bHitCursor)
			{
				m_bHitCursor = true;
				std::shared_ptr<KdSoundInstance> se = SoundCache::Instance().Get("Sound/SE/Cursor.wav");
				se->SetVolume(0.1f);
				se->Play(false);
			}

			if (KEY.IsDown(VK_LBUTTON))
			{

				//効果音発生
				std::shared_ptr<KdSoundInstance> se = SoundCache::Instance().Get("Sound/SE/Click.wav");
				se->SetVolume(0.1f);
				se->Play(false);

				//シーン切り替えの準備をする
				m_bChangeScene = true;
				m_changeSceneWaitTimer = m_changeSceneWait;
			}
		}
		else
		{
			m_bHitCursor = false;
		}
	}
}

void TitleScene::Draw()
{
	if (m_back)m_back->Draw();

	m_start->Draw();

	SHADER.m_spriteShader.SetMatrix(m_nameMat);
	SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/Title.png"));
}

void TitleScene::Release()
{
	std::shared_ptr<KdSoundInstance> bgm = SoundCache::Instance().Get("Sound/BGM/yukyunotokie.wav",SoundCache::SoundState::Playing);
	bgm->Stop();
}