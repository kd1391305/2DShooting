#include"Title.h"
#include"../Toolkit/Font/DrawString.h"
#include"../SceneManager.h"
#include"../Game/Game.h"
#include"../Toolkit/KeyManager.h"
#include"../Save/Save.h"
#include"../Game/Fireworks/FireworksManager.h"
#include"../Mouse/Mouse.h"

C_Title::C_Title() :
	//ゲームスタートボタンの初期化
	m_start(
		{ 0,0 },			//座標
		{ 210,40 },	//半径
		{ 1,1,1,1 },	//通常の色	
		{ 1,1,0,1 }),	//選択時の色

	m_bFirstMenu(true)		//最初の画面からスタートする
{
	//レベルボタンの初期化
	for (int i = 0; i < 3; i++)
	{
		float y = 200 - 200 * i;
		m_levelButton[i].SetPos({ 0, y });
		m_levelButton[i].SetRadius({ 210,40 });
		m_levelButton[i].SetColor({ 1,1,1,1 });
		m_levelButton[i].SetSelectColor({ 1,0,0,1 });
	}
}

void C_Title::Update()
{
	//デバッグ
	if (KEY.IsDown(VK_LBUTTON))
		FIREWORKS_MANAGER.Shot(MOUSE.GetPosf());
	FIREWORKS_MANAGER.Update();


	//最初の画面のとき
	if (m_bFirstMenu)
	{
		//スタートボタンの更新
		m_start.Update();
		if (m_start.IsSelect())
		{
			if (KEY.IsDown(VK_LBUTTON))
				//レベル選択画面へ移行
				m_bFirstMenu = false;
		}
	}
	//レベル選択画面のとき
	else
	{
		for (int i = 0; i < 5; i++)
		{
			//レベル選択ボタンの更新
			m_levelButton[i].Update();

			//選択されていたら
			if (m_levelButton[i].IsSelect())
			{
				if (KEY.IsDown(VK_LBUTTON))
					//ゲーム画面へ移行
					SCENE_MANAGER.ChangeState(new C_Game(i + 1));
			}
		}
	}
}

void C_Title::Draw()
{
	//最初の画面のとき
	if (m_bFirstMenu)
	{
		m_start.Draw();
		Math::Vector2 radius = m_start.GetRadius();
		if (m_start.IsSelect())radius *= m_start.GetSelectScale();
		float left = m_start.GetPos().x - radius.x;
		float top = m_start.GetPos().y + radius.y;
		float fontSize = C_DrawString::GetInstance().GetFontSize();
		if (m_start.IsSelect())
		{
			//文字サイズを変更して描画
			DRAW_STRING.Draw("ゲームスタート", { left,top }, fontSize);
		}
		//文字サイズそのままで描画
		else DRAW_STRING.Draw("ゲームスタート", { left,top });
	}
	//レベル選択画面のとき
	else
	{
		for (int i = 0; i < 3; i++)
		{
			m_levelButton[i].Draw();
			
			float left = m_levelButton[i].GetPos().x - m_levelButton[i].GetRadius().x;
			float top = m_levelButton[i].GetPos().y + m_levelButton[i].GetRadius().y;
			if		(i == 0)DRAW_STRING.Draw("Easy",	{ left,top }, { 0,0,0,1 });
			else if (i == 1)DRAW_STRING.Draw("Normal",	{left,top}, {0,0,0,1});
			else if (i == 2)DRAW_STRING.Draw("Hard",	{left,top}, {0,0,0,1});
		}
	}

	FIREWORKS_MANAGER.Draw();

}