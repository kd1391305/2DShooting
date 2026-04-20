#pragma once

//過去のキャラクラスの基底クラス
class C_FormerCharaBase
{
public:

	//コンストラクタ（メンバ変数の初期化）
	C_FormerCharaBase() :
		m_pos(0,0)
	{
		m_mat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	}

	virtual void Update()
	{
		//行列作成
		Math::Matrix scaleMat = Math::Matrix::CreateScale(0.5f, 0.5f, 0);
		Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
		m_mat = scaleMat * transMat;
	}

	virtual void Draw(KdTexture* tex)
	{
		SHADER.m_spriteShader.SetMatrix(m_mat);
		SHADER.m_spriteShader.DrawTex_Src(tex, { 0.7f,0.7f,0.7f,0.8f });
	}

	Math::Vector2* GetPosAddress() { return &m_pos; }

protected:

	Math::Vector2 m_pos;				//座標
	Math::Matrix m_mat;					//行列
};
