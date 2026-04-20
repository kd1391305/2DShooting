#include"Title.h"
#include"../Toolkit/Font/DrawString.h"
#include"../SceneManager.h"
#include"../Game/Game.h"
#include"../Toolkit/KeyManager.h"
#include"../Save/Save.h"

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
	for (int i = 0; i < 5; i++)
	{
		float y = 200 - 100 * i;
		m_levelButton[i].SetPos({ 0, y });
		m_levelButton[i].SetRadius({ 210,40 });
		m_levelButton[i].SetColor({ 1,1,1,1 });
		m_levelButton[i].SetSelectColor({ 1,0,0,1 });
	}

	//レベルボタンの反応を変更（解放されていないボタンは反応しない）
	for (int i = 1; i < 5; i++)
	{
		//解放されているレベルはとばす
		if (i < SAVE.GetReleaseLevel())continue;

		m_levelButton[i].SetColor({ 0.3f,0.3f,0.3f,0.7f });				//通常は灰色
		m_levelButton[i].SetSelectColor({ 0.3f,0.3f,0.3f,0.7f });		//選択中も灰色
		m_levelButton[i].SetSelectScale({ 1,1 });							//選択しても大きくならない
	}
}

void C_Title::Update()
{
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
				//解放されているレベルだったら
				if (i < SAVE.GetReleaseLevel())
				{
					if (KEY.IsDown(VK_LBUTTON))
						//ゲーム画面へ移行
						SCENE_MANAGER.ChangeState(new C_Game(i + 1));
				}
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
		for (int i = 0; i < 5; i++)
		{
			m_levelButton[i].Draw();
			char level[20];
			sprintf_s(level, sizeof(level), "Level %d", i + 1);
			float left = m_levelButton[i].GetPos().x - m_levelButton[i].GetRadius().x;
			float top = m_levelButton[i].GetPos().y + m_levelButton[i].GetRadius().y;
			DRAW_STRING.Draw(level, { left,top }, {0,0,0,1});
		}
	}
}