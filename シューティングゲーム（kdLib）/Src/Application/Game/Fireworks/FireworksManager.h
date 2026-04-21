#pragma once
#include"Fireworks.h"

//花火を管理するクラス
class C_FireworksManager
{
public:

	~C_FireworksManager() { Release(); }

	void Update();

	void Draw();

	void Release();

	void Shot(Math::Vector2 pos); 

private:
	std::vector<C_Fireworks>	m_fireworksList;
	const int m_poolSize = 10;

	KdTexture m_tex;            //Particle2用画像

	//シングルトン
private:
	C_FireworksManager()
	{
		for (int i = 0; i < m_poolSize; i++)
		{
			m_fireworksList.emplace_back();
		}

		//画像の読み込み
		m_tex.Load("Texture/Bullet3.png");
	}
public:
	static C_FireworksManager& GetInstance()
	{
		static C_FireworksManager instance;
		return instance;
	}
};

#define FIREWORKS_MANAGER C_FireworksManager::GetInstance()
