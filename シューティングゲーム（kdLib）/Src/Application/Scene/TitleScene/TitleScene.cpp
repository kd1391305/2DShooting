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
	//背景の更新
	if(m_back)m_back->Update();

	//花火を打ち上げるランダムで
	float deltaTime = Timer::Instance().GetDeltaTime();
	m_fireworksManager->Update(deltaTime);

	if (HitGacha(300 * deltaTime))
	{
		for (int i = 0; i < 10; i++)
		{
			float x = randRange(SCREEN_LEFT, SCREEN_RIGHT);
			float startY = randRange(-300, 0);
			float endY = randRange(0, 360);
			m_fireworksManager->Shot2({ x,startY }, { x,endY });
		}
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

	DWriteCustom::Instance().Draw("花火繚乱", { -150,300 }, fontSize);

}

void TitleScene::Release()
{
	
}