#include"PlayerBullet.h"

void C_PlayerBullet::Update()
{
	//座標更新
	m_pos += m_move;

	//画面外に出たら非活性状態にする
	if (m_pos.x < SCREEN_LEFT || m_pos.x > SCREEN_RIGHT ||
		m_pos.y > SCREEN_TOP || m_pos.y < SCREEN_BOTTOM)
	{
		m_bActive = false;
		return;
	}

	//行列作成
	Math::Matrix scaleMat = Math::Matrix::CreateScale(1, 0.5f, 0);
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scaleMat * transMat;

	for (int i = 0; i < 6; i++)
	{
		Math::Matrix scaleMat = Math::Matrix::CreateScale(1, 0.5f, 0);
		float posX = m_pos.x - 25 + i * 10;
		Math::Matrix transMat = Math::Matrix::CreateTranslation(posX, m_pos.y, 0);
		m_pileMat[i] = scaleMat * transMat;
	}
}

void C_PlayerBullet::Draw()
{
	D3D.SetBlendState(BlendMode::Add);
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex_Src(m_pTex, Math::Color{ 0.1f,0.1f,0.2f,0.7f });

	for (auto mat : m_pileMat)
	{
		SHADER.m_spriteShader.SetMatrix(mat);
		SHADER.m_spriteShader.DrawTex_Src(m_pTex, Math::Color{ 0.1f,0.1f,0.2f,0.7f });
	}
	D3D.SetBlendState(BlendMode::Alpha);
}