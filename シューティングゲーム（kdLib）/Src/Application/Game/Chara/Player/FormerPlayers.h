#pragma once
#include"FormerPlayer.h"

class C_FormerPlayers
{
public:

	void Init(int level, KdTexture* tex);

	void Update();

	void Draw();

private:
	std::vector<C_FormerPlayer> m_fPlayerList;

	KdTexture* m_pTex;

};
