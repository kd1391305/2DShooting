#pragma once
#include"../BaseChara/BaseChara.h"

class FireworksManager;

class Player :public BaseChara
{
public:

	Player();

	void Init(FireworksManager* set);

	//XV
	void Update(float deltaTime);

	//•`‰æ
	void Draw();

	void Dead();

	//s“®
	void Action(float deltaTime);

	//ƒvƒŒƒCƒ„[‚ğ–³“G‚É‚·‚é(ˆø”‚ª‰½•bŠÔ–³“G‚É‚·‚é‚©)
	void Invincible(float time)
	{
		m_bInvincible = true;
		m_invincibleTime = time;
		m_color = { 1,1,1,0.2f };
	}

	//–³“G‚©‚Ç‚¤‚©
	bool IsInvincible() { return m_bInvincible; }

	float GetHP() { return m_hp; }

	float* GetHPAddress() { return &m_hp; }
	float* GetHPMaxAddress() { return &m_hpMax; }
	Math::Vector2* GetPosAddress() { return &m_pos; }

	KdTexture* GetTex() { return &m_tex; }


private:

	static constexpr float s_speedMax = 200.0f;


	KdTexture m_tex;

	float m_shotWait;					//’e‚ğŒ‚‚Â‘Ò‹@ŠÔ

	bool m_bInvincible;					//–³“G‚©‚Ç‚¤‚©‚Ìƒtƒ‰ƒO
	float m_invincibleTime;				//–³“GŠÔ

	FireworksManager *m_pFireworksManager;


	float m_sumDeltaTime;				//Œo‰ß‚µ‚½ŠÔ‚ğ‘«‚µ‚Ä‚¢‚­(–³“Gó‘Ô‚Ì‚Æ‚«‚Ì“§–¾“x‚ğØ‚è‘Ö‚¦‚éˆ—‚Åg—p)
};