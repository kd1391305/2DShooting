#include"FormerEnemy.h"

//コンストラクタ
C_FormerEnemy::C_FormerEnemy():
	m_move(0,0),m_bActive(false)
{
}

//出現
void C_FormerEnemy::Spawn(const Math::Vector2& pos, const Math::Vector2& move)
{
	m_pos = pos;
	m_move = move;
	m_bActive = true;
	Math::Matrix scaleMat = Math::Matrix::CreateScale(0.5f, 0.5f, 0);
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scaleMat * transMat;
}

//更新
void C_FormerEnemy::Update()
{
	//座標更新
	m_pos += m_move;
	//行列作成
	Math::Matrix scaleMat = Math::Matrix::CreateScale(0.5f, 0.5f, 0);
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scaleMat * transMat;
}

//描画
void C_FormerEnemy::Draw(KdTexture* tex)
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex_Src(tex, { 0.0f,0.7f,0.7f,0.8f });
}