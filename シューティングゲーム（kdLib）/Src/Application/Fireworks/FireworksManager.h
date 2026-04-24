#pragma once
#include"Fireworks.h"

//花火を管理するクラス
class FireworksManager
{
public:

	FireworksManager()
	{
		Init();
	}

	~FireworksManager() { Release(); }

	void Update(float deltaTime);

	void Draw();

	void Shot(Math::Vector2 pos,Math::Vector2 targetP); 

	//花火を指定した場所で爆発させる
	void Shot2(Math::Vector2 pos, Math::Vector2 targetP);

	std::vector<Fireworks>& GetFireworksList() { return m_fireworksList; }

private:

	void Init();

	void Release();

	//当たり判定ありの花火（当たったら弾ける）
	std::vector<Fireworks>	m_fireworksList;

	//当たり判定なし（指定した場所で弾ける）
	std::vector<Fireworks2>	m_fireworksList2;
	const int m_poolSize = 20;

	KdTexture m_tex;				 //Particle2用画像
};