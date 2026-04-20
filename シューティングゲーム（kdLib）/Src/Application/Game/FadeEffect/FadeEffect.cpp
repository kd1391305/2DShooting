#include "FadeEffect.h"

//静的変数の初期化(これを書けばエラーが取れる。なぜ？)
KdTexture C_FadeEffect::s_tex;

C_FadeEffect::C_FadeEffect() :
	m_bActive(false)
{
	
}

//更新
void C_FadeEffect::Update()
{
	m_animCnt += s_animCntSpeed;

	if (m_animCnt >= s_animCntEnd)
	{
		m_bActive = false;
		return;
	}

	//行列作成
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_scale, m_scale, 0);
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scaleMat * transMat;
}

//描画
void C_FadeEffect::Draw()
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	Math::Rectangle rect = { (int)m_animCnt % (int)s_animCntEnd * s_texDiameter,14 * s_texDiameter,s_texDiameter,s_texDiameter };
	SHADER.m_spriteShader.DrawTex(&s_tex, 0, 0, &rect, &Math::Color{ 1,1,1,1 });
}

//発生
void C_FadeEffect::Emit(Math::Vector2 pos, float radius)
{
	m_pos = pos;
	m_scale = (radius * 2) / (float)s_texDiameter;
	m_bActive = true;
	m_animCnt = 0;
}


