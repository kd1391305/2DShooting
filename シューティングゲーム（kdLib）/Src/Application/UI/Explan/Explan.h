#pragma once

class Explan
{
public:

	void Init(Math::Vector2* pPlayerPos);
	void Update(float deltaTime);
	void Draw();

	bool IsEnd()
	{
		//完全に消えて、終了時間を過ぎていたら
		return m_color.A() <= 0 && m_timer >= m_endTime;
	}

private:

	float m_timer;					//経過時間を測る

	float m_endTime;				//終了時間を測る

	

	Math::Vector2 *m_pPlayerPos;	//プレイヤーの座標

	float m_deltaAlpha;				//透明度の変化量（一秒あたり）
	Math::Color m_color;			//UIの色


	struct BaseExplan
	{
		Math::Vector2 m_playerOffset;	//プレイヤーの座標との差（ギャップ）

		Math::Vector2 m_pos;
		Math::Vector2 m_radius;
		Math::Vector2 m_scale;
		Math::Matrix m_mat;

		Math::Vector2 m_backPos;
		Math::Vector2 m_backRadius;
		Math::Vector2 m_backScale;
		Math::Matrix m_backMat;		//背景描画用の行列
	};

	float fontSize;

	BaseExplan m_moveExplan;
	BaseExplan m_chargeExplan;
	


};
