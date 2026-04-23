#include"Title.h"
#include"../Toolkit/Font/DrawString.h"
#include"../SceneManager.h"
#include"../Game/Game.h"
#include"../Toolkit/KeyManager.h"
#include"../Save/Save.h"
#include"../Fireworks/FireworksManager.h"
#include"../Mouse/Mouse.h"
#include"../main.h"
#include"../Background/Back.h"
#include"../Timer/Timer.h"
#include"../Toolkit/Button.h"

void Title::Init()
{
	if (!m_start)
	{
		m_start = new Button();
	}
	m_start->SetPos({ 0,0 });					//座標
	m_start->SetRadius({ 210,40 });			//半径
	m_start->SetColor({ 1,1,1,1 });			//通常の色	
	m_start->SetSelectColor({ 1,1,0,1 });	//選択時の色

	if (!m_back)
	{
		m_back = new Back();
		m_back->Init();
	}

	if (!m_fireworksManager)
	{
		m_fireworksManager = new FireworksManager();
	}
}


void Title::Update()
{
	//背景の更新
	if(m_back)m_back->Update();

	//デバッグ
	if (KEY.IsDown(VK_LBUTTON))
		m_fireworksManager->Shot(MOUSE.GetPosf() - Math::Vector2{ 400,400 }, MOUSE.GetPosf());
	m_fireworksManager->Update(Timer::Instance().GetDeltaTime());

	//スタートボタンの更新
	m_start->Update();
	if (m_start->IsSelect())
	{
		if (KEY.IsDown(VK_LBUTTON))
		{
			//ゲーム画面へ移行
			SceneManager::Instance().ChangeState(new Game(m_back,m_fireworksManager));
			m_back = nullptr;
			m_fireworksManager = nullptr;
		}
	}
}

void Title::Draw()
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

void Title::Release()
{
	if (m_back)
	{
		delete m_back;
		m_back = nullptr;
	}
}