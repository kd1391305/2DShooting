#include "AttackArea.h"

void AttackArea::Update(float deltaTime)
{
	if (m_bHitFlg)
	{
		m_hitTimer -= deltaTime;
		if (m_hitTimer <= 0)
		{
			m_bActive = false;
		}
		m_alpha -= 0.8f * deltaTime;
		if (m_alpha > 0.1f)m_alpha = 0.1f;
	}
	else
	{
		m_alpha += 0.8f * deltaTime;
		if (m_alpha > 0.5f)m_alpha = 0.5f;
	}
}

void AttackArea::Draw(KdTexture* tex)
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex_Src(tex, {1,1,1,m_alpha});
}

void AttackArea::Emit(Math::Vector2 pos, Math::Vector2 radius)
{
	m_bActive = true;

	m_pos = pos;
	m_radius = radius;
	Math::Vector2 texRadius = { 32,32 };

	m_scale = radius / texRadius;

	m_mat = Math::Matrix::CreateScale(m_scale.x, m_scale.y, 0)* Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_alpha = 0.01f;
}