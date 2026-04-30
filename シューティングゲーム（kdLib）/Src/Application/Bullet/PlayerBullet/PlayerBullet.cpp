#include"PlayerBullet.h"

void PlayerBullet::Pierce()
{
	m_pierceNum++;
	m_power--;
	if (m_power < 0)m_power = 0;
}

void PlayerBullet::Draw()
{
	SHADER.m_spriteShader.ClearMatrix();
	D3D.SetBlendState(BlendMode::Add);
	for (int i = 10; i > 5; i--)
	{
		SHADER.m_spriteShader.DrawCircle(m_pos.x, m_pos.y, i + m_power*2, &Math::Color{ 1.0f,0.2f,0.8f,1 }, true);
	}
	D3D.SetBlendState(BlendMode::Alpha);
}