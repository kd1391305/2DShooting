#pragma once

struct BackObject
{
	void Update(float scrollX)
	{
		m_pos.x += scrollX;
		m_mat = Math::Matrix::CreateScale(m_scale, m_scale, 0) * Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	}

	void Draw(KdTexture* tex)
	{
		SHADER.m_spriteShader.SetMatrix(m_mat);
		SHADER.m_spriteShader.DrawTex_Src(tex,m_color);
	}

	Math::Vector2 m_pos;
	float m_scale;
	Math::Matrix m_mat;
	Math::Color m_color;

};


class Back
{
public:
	Back() {}

	void Init();

	void Update(float deltaTime);

	void Draw();

private:

	KdTexture m_tex;		
	static const int s_drawNum = 2;			//描画回数
	Math::Vector2 m_pos[s_drawNum];			//座標
	Math::Matrix m_mat[s_drawNum];			//行列

	BackObject m_lantern[10];			//ランタン（提灯）
	KdTexture m_lanternTex;

	BackObject m_fance[10];				//フェンス（柵）
	KdTexture m_fanceTex;				
};

