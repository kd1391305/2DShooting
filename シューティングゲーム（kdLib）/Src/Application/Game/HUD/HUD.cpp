#include"HUD.h"

void C_HUD::Init(C_Player* player, int level)
{
	m_player.Init(player);
	m_score.Init(level);
}
void C_HUD::Update()
{
	m_player.Update();
}

void C_HUD::Draw()
{
	m_back.Draw();
	m_player.Draw();
	m_score.Draw();
	HUDTimer::Draw();
}