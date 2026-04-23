#include"HUD.h"

void HUD::Init(Player* player)
{
	m_player.Init(player);
	m_score.Init();
}
void HUD::Update()
{
	m_player.Update();
}

void HUD::Draw()
{
	m_back.Draw();
	m_player.Draw();
	m_score.Draw();
	HUDTimer::Draw();
}