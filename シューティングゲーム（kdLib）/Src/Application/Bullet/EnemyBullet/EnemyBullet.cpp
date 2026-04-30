#include "EnemyBullet.h"

void EnemyBullet::Draw()
{
	SHADER.m_spriteShader.ClearMatrix();
	D3D.SetBlendState(BlendMode::Add);
	for (int i = 10; i > 5; i--)
	{
		SHADER.m_spriteShader.DrawCircle(m_pos.x, m_pos.y, i, &Math::Color{ 0.2f,0.4f,1.0f,1 }, true);
	}
	D3D.SetBlendState(BlendMode::Alpha);
}
