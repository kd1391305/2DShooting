#pragma once
#include"Fireworks.h"

//‰Ô‰Î‚ğŠÇ—‚·‚éƒNƒ‰ƒX
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

	void Shot(Math::Vector2 pos,Math::Vector2 move); 

	std::vector<Fireworks>& GetFireworksList() { return m_fireworksList; }

private:

	void Init();

	void Release();

	std::vector<Fireworks>	m_fireworksList;
	const int m_poolSize = 20;

	KdTexture m_tex;				 //Particle2—p‰æ‘œ
};