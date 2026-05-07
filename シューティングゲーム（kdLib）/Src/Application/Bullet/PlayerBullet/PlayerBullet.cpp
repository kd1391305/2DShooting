#include"PlayerBullet.h"

void PlayerBullet::Pierce()
{
	m_pierceNum++;
	m_power -= 2;
	if (m_power < 0)
	{
		m_power = 0;
		m_bActive = false;
	}
}

void PlayerBullet::Draw()
{
	SHADER.m_spriteShader.ClearMatrix();
	D3D.SetBlendState(BlendMode::Add);
	float scale = 0.1f;
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(scale)* Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0));
	for (int i = 0; i < 4; i++)
	{
		SHADER.m_spriteShader.DrawCircle(0, 0, (m_radius + m_power / 2.0f + i * 2) / scale / 1.2f, &Math::Color{ 0.7f,0.2f,0.7f,1 }, true);
	}
	D3D.SetBlendState(BlendMode::Alpha);
}