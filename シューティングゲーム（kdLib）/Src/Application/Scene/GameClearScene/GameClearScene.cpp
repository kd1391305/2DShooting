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

void GameClearScene::Init()
{
	m_gameButton = std::make_shared<ButtonEx>();
	m_gameButton->SetPos({ 150,-100 });						//座標
	m_gameButton->SetRadius({ 100,50 });					//半径
	m_gameButton->SetColor({ 0.2f,0.2f,0.2f,1.0f });		//通常の色	
	m_gameButton->SetSelectScale({ 1.01f,1.01f });			//選択時の拡縮
	m_gameButton->SetSelectColor({ 0.4f,0.4f,0.4f,1.0f });	//選択時の色
	m_gameButton->SetTex("Texture/Button/ButtonBack.png");

	m_titleButton = std::make_shared<ButtonEx>();
	m_titleButton->SetPos({ -150,-100 });					//座標
	m_titleButton->SetRadius({ 100,50 });					//半径
	m_titleButton->SetColor({ 0.2f,0.2f,0.2f,1.0f });		//通常の色	
	m_titleButton->SetSelectScale({ 1.01f,1.01f });			//選択時の拡縮
	m_titleButton->SetSelectColor({ 0.4f,0.4f,0.4f,1.0f });	//選択時の色
	m_titleButton->SetTex("Texture/Button/ButtonBack.png");
}

void GameClearScene::Update()
{
	const float deltaTime = Timer::Instance().GetDeltaTime();

	m_back->Update(deltaTime);

	m_gameButton->Update();
	if (m_gameButton->IsSelect())
	{
		if (KEY.IsDown(VK_LBUTTON))
		{
			SceneManager::Instance().ChangeState(std::make_shared<Game>(m_back));
		}
	}

	m_titleButton->Update();
	if (m_titleButton->IsSelect())
	{
		if (KEY.IsDown(VK_LBUTTON))
		{
			SceneManager::Instance().ChangeState(std::make_shared<TitleScene>(m_back));
		}
	}
}

void GameClearScene::Draw()
{
	m_back->Draw();
	m_titleButton->Draw();
	DWriteCustom::Instance().Draw("タイトルへ", m_titleButton->GetPos());
	m_gameButton->Draw();
	DWriteCustom::Instance().Draw("もう一度", m_gameButton->GetPos());
}

void GameClearScene::Release()
{
}
