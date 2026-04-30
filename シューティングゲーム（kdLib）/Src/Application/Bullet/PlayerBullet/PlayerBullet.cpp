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
		SHADER.m_spriteShader.DrawCircle(m_pos.x, m_pos.y, i - 3 + m_power, &Math::Color{ 0.1f,0.8f,1.0f,1.0 }, true);
	}
	D3D.SetBlendState(BlendMode::Alpha);
}