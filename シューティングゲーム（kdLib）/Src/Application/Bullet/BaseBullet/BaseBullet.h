#pragma once

class BaseBullet
{
public:

	BaseBullet() :
		m_pos(0, 0),
		m_move(0, 0),
		m_bActive(false),
		m_radius(0)
	{
	}

	virtual ~BaseBullet() {}
	
	//XV
	virtual void Update(float deltaTime);

	//•`‰æ
	virtual void Draw() {}

	//’e‚ğŒ‚‚Â
	void Shot(Math::Vector2 pos, Math::Vector2 move)
	{
		m_pos = pos;
		m_move = move;
		m_bActive = true;
	}

	//’e‚ªŒ‚‚½‚ê‚Ä‚¢‚é‚©‚Ç‚¤‚©
	bool IsActive() { return m_bActive; }

	Math::Vector2 GetPos() { return m_pos; }
	float GetRadius() { return m_radius; }

	void SetActive(bool set) { m_bActive = set; }
protected:

	Math::Matrix m_mat;
	Math::Vector2 m_pos;
	Math::Vector2 m_move;

	float m_radius;

	bool m_bActive;
};