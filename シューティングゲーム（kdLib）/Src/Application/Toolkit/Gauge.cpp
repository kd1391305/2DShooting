#include"Gauge.h"

void C_Gauge::Init(const Math::Vector2& pos, const Math::Vector2& radius,const float* pPointMax, const float* pPoint, const float speed)
{
	m_pos = pos;
	m_radius = radius;
	m_pPointMax = pPointMax;
	m_pPoint = pPoint;
	m_formerPoint = *m_pPoint;
	m_speed = speed;
}

void C_Gauge::Update()
{
	if (*m_pPoint != m_formerPoint)
	{
		//formerPoint(前の値)をpoint（現在の値）に近づける
		if (m_formerPoint > *m_pPoint)	
		{
			m_formerPoint -= m_speed;
			if (m_formerPoint < *m_pPoint)m_formerPoint = *m_pPoint;
		}
		else
		{
			m_formerPoint += m_speed;
			if (m_formerPoint > *m_pPoint)m_formerPoint = *m_pPoint;
		}
	}
}

void C_Gauge::Draw()
{
	//変換行列をリセットする
	SHADER.m_spriteShader.ClearMatrix();

	//ゲージを覆う枠を描画する
	SHADER.m_spriteShader.DrawBox(m_pos.x, m_pos.y, m_radius.x, m_radius.y, &Math::Color{ 0.7f,0.7f,0.7f,0.2f }, true);

	//現在のゲージ量を描画
	float left = m_pos.x - m_radius.x;						//左端座標	
	float radiusX = m_radius.x * (*m_pPoint / *m_pPointMax);	//描画する半径(x)
	SHADER.m_spriteShader.DrawBox(left + radiusX, m_pos.y, radiusX, m_radius.y, &Math::Color{ 0,1,0,1 }, true);		//描画

	//増減中のゲージ量を描画
	if (m_formerPoint != *m_pPoint)
	{
		float right = left + radiusX * 2;			//先ほど描画したゲージの右端座標
		float gap = m_formerPoint - *m_pPoint;		//現在と前の値の差
		radiusX = m_radius.x * (gap / *m_pPointMax);	//描画する半径(x)
		SHADER.m_spriteShader.DrawBox(right + radiusX, m_pos.y, radiusX, m_radius.y, &Math::Color{ 1,0,0,1 }, true);
	}
}