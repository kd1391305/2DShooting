#include "FormerPlayers.h"
#include"../../../Save/Save.h"
#include"../../GameTimer.h"
#include"../../Bullet/BulletManager.h"

void C_FormerPlayers::Init(int level , KdTexture *tex)
{
	if (!m_fPlayerList.empty())m_fPlayerList.clear();

	for (int i = 1; i < level; i++)
	{
		m_fPlayerList.push_back(C_FormerPlayer());
	}

	m_pTex = tex;
}

void C_FormerPlayers::Update()
{
	for (int i = 0; i < m_fPlayerList.size(); i++) 
	{
		//弾を撃つ時間だったら
		int level = i + 1;			
		if (SAVE.GetPlayerShotTime(level) == GAME_TIMER.GetTime())
		{
			//弾を発射する
			BULLET_MANAGER.ShotOfPlayer(m_fPlayerList[i].GetPos(), { 15,0 });
			//次の弾を撃つタイミングに移行する
			SAVE.PopPlayerShotTime(level);
		}
		//座標を読み込む
		SAVE.ReadPlayerPos(m_fPlayerList[i].GetPosAddress(), i + 1);
		//更新
		m_fPlayerList[i].Update();
	}
}

void C_FormerPlayers::Draw()
{
	for (auto p : m_fPlayerList)
	{
		p.Draw(m_pTex);
	}
}
