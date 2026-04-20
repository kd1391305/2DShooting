#include "Pch.h"
#include "FormerPlayers.h"
#include"../../../Save/Save.h"

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
		SAVE.ReadPlayerPos(m_fPlayerList[i].GetPosAddress(), i + 1);
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
