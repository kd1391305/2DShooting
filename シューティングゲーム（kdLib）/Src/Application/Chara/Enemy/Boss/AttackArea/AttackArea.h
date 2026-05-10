#pragma once

class AttackArea
{
public:

	void Update(float deltaTime);

	void Draw(KdTexture* tex);

	void Emit(Math::Vector2 pos, Math::Vector2 radius);

	void SetHitFlg(bool hitFlg,float hitTime) { 
		m_bHitFlg = hitFlg;
		m_hitTimer = hitTime;
	}


	Math::Vector2& GetPos() { return m_pos; }
	Math::Vector2& GetRadius() { return m_radius; }

	bool GetHitFlg() { return  m_bHitFlg; }
	bool IsActive() { return m_bActive; }

private:

	Math::Vector2 m_pos;
	Math::Vector2 m_radius;
	Math::Vector2 m_scale;
	Math::Matrix m_mat;

	float m_hitTimer;		//“–‚½‚éŠÔ
	float m_bHitFlg;		//“–‚½‚è”»’è‚ª‚ ‚é‚©H

	bool m_bActive;

	float m_alpha;
};
