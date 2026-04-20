#include"FormerPlayer.h"

void C_FormerPlayer::Update()
{
	//çsóÒçÏê¨
	Math::Matrix scaleMat = Math::Matrix::CreateScale(0.5f,0.5f,0);
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scaleMat * transMat;
}

void C_FormerPlayer::Draw(KdTexture *tex)
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex_Src(tex, { 0.7f,0.7f,0.7f,0.8f });
}