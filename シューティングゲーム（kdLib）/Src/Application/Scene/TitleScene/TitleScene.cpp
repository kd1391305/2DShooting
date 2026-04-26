#include"TitleScene.h"
#include"../../Font/DrawString.h"
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

void TitleScene::Init()
{
	m_nameTex.Load("Texture/Title.png");
	m_namePos = { 0,200 };
	Math::Matrix scale = Math::Matrix::CreateScale(2.0, 2.0f, 0);
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_namePos.x, m_namePos.y, 0);
	m_nameMat = scale * trans;

	if (!m_start)
	{
		m_start = std::make_shared<Button>();
	}
	m_start->SetPos({ 0,0 });					//座標
	m_start->SetRadius({ 210,40 });			//半径
	m_start->SetColor({ 1,1,1,1 });			//通常の色	
	m_start->SetSelectColor({ 1,1,0,1 });	//選択時の色

	if (!m_back)
	{
		m_back = std::make_shared<Back>();
		m_back->Init();
	}

	if (!m_fireworksManager)
	{
		m_fireworksManager = std::make_shared<FireworksManager>();
	}
}


void TitleScene::Update()
{
	float deltaTime = Timer::Instance().GetDeltaTime();

	//背景の更新
	if(m_back)m_back->Update(deltaTime);

	//花火を打ち上げるランダムで
	
	m_fireworksManager->Update(deltaTime);

	if (HitGacha(10 * deltaTime))
	{
		float startX = randRange(SCREEN_LEFT, SCREEN_RIGHT);
		Math::Vector2 startPos = {  startX,SCREEN_BOTTOM - 30 };
		Math::Vector2 targetPos = { startX,randRange(100,SCREEN_TOP) };
		float speed = 400;
		Math::Vector2 beforeScale = { 0.7f,0.7f };
		Math::Vector2 afterScale = { randRange(0.5f,1.5f),randRange(0.5f,1.5f) };
		Math::Color color = { randRange(0.0f,0.5f),randRange(0.0f,0.5f),randRange(0.0f,0.5f),randRange(0.3f,0.5f) };
		int type = randRange(0,2);
		m_fireworksManager->Shot((FireworksManager::Type)type, startPos, targetPos, speed, beforeScale, afterScale, color, true);
	}

	//スタートボタンの更新
	m_start->Update();
	if (m_start->IsSelect())
	{
		if (KEY.IsDown(VK_LBUTTON))
		{
			//ゲーム画面へ移行
			SceneManager::Instance().ChangeState(new Game(m_back, m_fireworksManager));
			m_back = nullptr;
			m_fireworksManager = nullptr;
		}
	}
}

void TitleScene::Draw()
{
	if(m_back)m_back->Draw();

	m_start->Draw();
	Math::Vector2 radius = m_start->GetRadius();
	if (m_start->IsSelect())radius *= m_start->GetSelectScale();
	float left = m_start->GetPos().x - radius.x;
	float top = m_start->GetPos().y + radius.y;
	float fontSize = DWriteCustom::Instance().GetFontSize() * m_start->GetSelectScale().x;
	Math::Color fontColor = { 0.1f,0.1f,0.1f,1 };
	if (m_start->IsSelect())
	{
		//文字サイズを変更して描画
		DWriteCustom::Instance().Draw("開始", { left,top }, fontSize, fontColor);
	}
	//文字サイズそのままで描画
	else DWriteCustom::Instance().Draw("開始", { left,top }, fontColor);


	m_fireworksManager->Draw();

	/*DWriteCustom::Instance().Draw("花 火 繚 乱", { -150 + 5,300 + 5 }, fontSize, Math::Color{ 0,0,0,1 });
	DWriteCustom::Instance().Draw("花 火 繚 乱", { -150 - 5,300 - 5 }, fontSize, Math::Color{ 1,0.2f,0.2f,1.0f });
	DWriteCustom::Instance().Draw("花 火 繚 乱", { -150,300 }, fontSize, Math::Color{ 0.9f,0.9f,0.9f,1.0f });*/
	SHADER.m_spriteShader.SetMatrix(m_nameMat);
	SHADER.m_spriteShader.DrawTex_Src(&m_nameTex);
}

void TitleScene::Release()
{
	
}