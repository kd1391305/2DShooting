#pragma once

struct BackObject
{
	void Update(float scrollX)
	{
		m_pos.x += scrollX;
		m_mat = Math::Matrix::CreateScale(m_scale, m_scale, 0) * Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	}

	void Draw(std::shared_ptr<KdTexture>tex)
	{
		SHADER.m_spriteShader.SetMatrix(m_mat);
		SHADER.m_spriteShader.DrawTex_Src(tex,m_color);
	}

	Math::Vector2 m_pos;
	float m_scale;
	Math::Matrix m_mat;
	Math::Color m_color;
};

struct LightParticle
{
	Math::Vector2 m_pos;
	Math::Vector2 m_move;
	Math::Vector2 m_radius;
	float m_degree;
	float m_deltaDegree;
	float m_alphaMin;		//アルファ値の最大
	float m_alphaMax;		//アルファ値の最小
	Math::Color m_color;
	float m_life;			//生存時間（０になった瞬間に消さない）

	void Update(float deltaTime);

	void Draw();
};


class Back
{
public:
	Back() {}

	void Init();

	void Update(float deltaTime);

	void Draw();

private:

	void Respawn(LightParticle* light);

	static const int s_drawNum = 2;			//描画回数
	Math::Vector2 m_pos[s_drawNum];			//座標
	Math::Matrix m_mat[s_drawNum];			//行列

	BackObject m_lantern[3];			//ランタン（提灯）

	BackObject m_fance[13];				//フェンス（柵）
	float m_fanceWidth ;

	LightParticle m_lightParticleList[30];	//埃のような粒子
};

