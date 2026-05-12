#include"TitleScene.h"
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
	m_start->SetTex("Texture/Title/StartButton.png");

	if (!m_back)
	{
		m_back = std::make_shared<Back>();
		m_back->Init();
	}

	m_shotCoolTimer = 2;

	//BGMを流す
	std::shared_ptr<KdSoundInstance> bgm = SoundCache::Instance().Get("Sound/BGM/yukyunotokie.wav");
	bgm->SetVolume(0.02f);
	bgm->Play(true);

	m_bChangeScene = false;

	m_start->SetSelect(true);

	m_enterKeyAlpha = 0.4f;
	m_enterKeyDeltaAlpha = 0.2f;
	m_enterKeyMat = Math::Matrix::CreateScale(0.8f, 0.8f, 0) * Math::Matrix::CreateTranslation(280, -100, 0);
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

			std::shared_ptr<KdSoundInstance> bgm = SoundCache::Instance().Get("Sound/BGM/yukyunotokie.wav", SoundCache::SoundState::Playing);
			bgm->Stop();
		}
	}
	else
	{
		m_shotCoolTimer -= deltaTime;
		//マウスクリックで花火を出現
		if (KEY.IsDown(VK_LBUTTON))
		{
			//花火を打ち上げる色、大きさをランダムで
			if (m_shotCoolTimer <= 0)
			{
				Math::Vector2 pos = MOUSE.GetPosf() / m_back->GetAllScale();
				float afterScale = randRange(0.7f, 1.1f);
				Math::Color color = { randRange(0,0.8f),randRange(0.0f,0.8f),randRange(0.0f,0.8f),randRange(0.5f,0.7f) };
				Math::Color color2 = { randRange(0,0.8f),randRange(0.0f,0.8f),randRange(0.0f,0.8f),randRange(0.5f,0.7f) };
				FireworksManager::Type type = m_back->GetFireworks()->GetRandomType_Quick();
				float seVolume = 0.01f;
				for (int i = 0; i < 3; i++)
				{
					m_back->GetFireworks()->Explode(type,
						pos, afterScale, color, seVolume);
				}
				//二重花火にする（花火の中にちいさな花火を追加）
				m_back->GetFireworks()->Explode(type,
					pos, afterScale / 1.5f, color, seVolume);

				//クールタイムを設ける
				m_shotCoolTimer = randRange(0.1, 0.2);
			}
		}

		//スタートボタンの更新
		m_start->Update();
		if (m_start->IsSelect() || KEY.IsDown(VK_RETURN))
		{
			if (!m_bHitCursor)
			{
				m_bHitCursor = true;
				std::shared_ptr<KdSoundInstance> se = SoundCache::Instance().Get("Sound/SE/Cursor.wav");
				se->SetVolume(0.1f);
				se->Play(false);
			}

			if ((KEY.IsDown(VK_LBUTTON) && m_start->GetActionType() == Button::ActionType::Mouse) ||
				(KEY.IsDown(VK_RETURN)))
			{
				//効果音発生
				std::shared_ptr<KdSoundInstance> se = SoundCache::Instance().Get("Sound/SE/Click.wav");
				se->SetVolume(0.1f);
				se->Play(false);

				//シーン切り替えの準備をする
				m_bChangeScene = true;
				m_changeSceneWaitTimer = m_changeSceneWait;


				for (int i = 0; i < 15; i++)
				{
					Math::Vector2 pos = Math::Vector2{ randRange(SCREEN_LEFT,SCREEN_RIGHT),randRange(SCREEN_BOTTOM,SCREEN_TOP) };
					float afterScale = randRange(0.4f, 0.7f);
					Math::Color color = { randRange(0,0.8f),randRange(0.0f,0.8f),randRange(0.0f,0.8f),randRange(0.5f,0.7f) };
					Math::Color color2 = { randRange(0,0.8f),randRange(0.0f,0.8f),randRange(0.0f,0.8f),randRange(0.5f,0.7f) };
					FireworksManager::Type type = m_back->GetFireworks()->GetRandomType_Quick();
					float seVolume = 0.001f;
					for (int j = 0; j < 2; j++)
					{
						m_back->GetFireworks()->Explode(type,
							pos, afterScale, color, seVolume);
					}
					//二重花火にする（花火の中にちいさな花火を追加）
					m_back->GetFireworks()->Explode(type,
						pos, afterScale / 1.5f, color, seVolume);
				}
			}
		}
		else
		{
			m_bHitCursor = false;
		}
	}

	//エンターキー画像の処理
	m_enterKeyAlpha += m_enterKeyDeltaAlpha * deltaTime;
	if (m_enterKeyAlpha > 1.0f)
	{
		m_enterKeyDeltaAlpha *= -1;
	}
	if(m_enterKeyAlpha < 0.4f)
	{
		m_enterKeyDeltaAlpha *= -1;
	}
}

void TitleScene::Draw()
{
	if (m_back)m_back->Draw();

	m_start->Draw();

	SHADER.m_spriteShader.SetMatrix(m_nameMat);
	SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/Title/TitleName.png"));

	D3D.SetBlendState(BlendMode::Add);
	SHADER.m_spriteShader.SetMatrix(m_enterKeyMat);
	SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/Title/EnterKey.png"),Math::Color{1,1,1,m_enterKeyAlpha});
	D3D.SetBlendState(BlendMode::Alpha);
}

void TitleScene::Release()
{
	
}

void TitleScene::ExplodeFireworks()
{
	
}
