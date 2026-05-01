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
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(0.1f) * Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0));
	for (int i = 2; i >= 0; i--)
	{
		SHADER.m_spriteShader.DrawCircle(0, 0, 50 - i * 2 + m_power * 10, &Math::Color{ 0.8f,0.2f,0.8f,1 }, true);
	}
	D3D.SetBlendState(BlendMode::Alpha);
}