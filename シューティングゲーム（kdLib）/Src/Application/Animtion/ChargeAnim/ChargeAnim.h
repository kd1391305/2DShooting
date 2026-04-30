#pragma once

struct ChargeParticle
{
	Math::Vector2 m_pos;
	Math::Vector2 m_move;
	Math::Vector2 m_radius;
	Math::Vector2 m_scale;
	Math::Matrix m_mat;
	Math::Color m_color;
	float m_life;

	void Update(float deltaTime);
	void Draw(std::string path);
};

class ChargeAnim
{

public:

	ChargeAnim() {}
	~ChargeAnim() { Release(); }

	void Init();
	void Update(float deltaTime);
	void Draw();

	void Start(Math::Vector2 pos, Math::Vector2 radius,float moveSpeedMin,float moveSpeedMax);

	//チャージがマックスになったら
	void StartChargeMaxAnim();

	bool IsEnd();

	void SetPos(Math::Vector2 pos) 
	{
		Math::Vector2 gap = pos - m_centerPos;
		m_centerPos = pos;
		for (auto& p : m_particleList)
		{
			p->m_pos += gap;
		}
	}

private:
	void Release();

	Math::Vector2 m_centerPos;			//中心座標
	Math::Vector2 m_radius;

	float m_moveSpeedMin;
	float m_moveSpeed;
	float m_moveSpeedMax;

	std::vector<std::shared_ptr<ChargeParticle>> m_particleList;

	float m_animTime;		//アニメーションが始まってからの時間
	float m_animEndTime;	//アニメーション終了時間
	bool m_chargeMaxFlg = false;
};
