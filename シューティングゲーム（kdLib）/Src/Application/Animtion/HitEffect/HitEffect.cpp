#include"HitEffect.h"

bool HitEffect::Update(float deltaTime)
{
	m_animTimer += s_animSpeed * deltaTime;

	m_pos += m_move * deltaTime;

	Math::Matrix scaleMat = Math::Matrix::CreateScale(s_scale);
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scaleMat * transMat;

	if (m_animTimer >= s_animTimeEnd)
	{
		return false;
	}

	//アニメーションが終わっていないことを返す
	return true;
}

void HitEffect::Draw(std::shared_ptr<KdTexture> tex)
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	Math::Rectangle rect = { (int)m_animTimer * s_animDiameter,s_animDiameter * 8,s_animDiameter,s_animDiameter };
	SHADER.m_spriteShader.DrawTex(tex.get(), 0, 0, &rect, &m_color);
}

void HitEffect::Emit(Math::Vector2& pos,Math::Vector2&move)
{
	m_pos = pos;
	m_move = move;

	Math::Matrix scaleMat = Math::Matrix::CreateScale(s_scale);
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scaleMat * transMat;

	m_color = { 0.7f,0.9f,0.9f,0.8f };		//水色
}
