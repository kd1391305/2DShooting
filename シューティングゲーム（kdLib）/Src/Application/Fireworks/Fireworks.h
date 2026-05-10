#pragma once
#include"BaseFireworks/BaseFireworks.h"

//===================================================
//‰Ô‰Î‚Pi‰~‚Ì‚İ‚Å•`‰æj
//===================================================
class Fireworks1 :public BaseFireworks
{
public:

	Fireworks1() {}
	~Fireworks1() { Release(); }

	//‰Šú‰»
	void Init();

	//XV
	void Update(float deltaTime);

	//•`‰æ
	void Draw(float backScale)override;

	//‰Ô‰Î‚ğ‘Å‚¿ã‚°‚é
	void Shoot(Math::Vector2& startPos, Math::Vector2& startMove, float beforeScale, float afterScale, Math::Color& color)override;

	//‰Ô‰Î‚ğ’e‚¯‚³‚¹‚é
	void Explode();

private:

	//‰ğ•ú‚·‚é
	void Release();

	std::vector<Particle> m_circleList;		//‰Ô‰Î‚Ì‰ÎiŠÛŒ`j
	int m_circleActiveNum;
};
//===================================================

//===================================================
//‰Ô‰Î‚Qi‰~‚Æ–_‚Å•`‰æj
////===================================================
//class Fireworks2:public BaseFireworks
//{
//public:
//
//	Fireworks2() {}
//	~Fireworks2() { Release(); }
//
//	//‰Šú‰»
//	void Init()override;
//
//	//XV
//	void Update(float deltaTime)override;
//
//	//•`‰æ
//	void Draw(float backScale)override;
//
//	//‰Ô‰Î‚ğ‘Å‚¿ã‚°‚é
//	void Shoot(Math::Vector2& startPos, Math::Vector2& startMove, float beforeScale, float afterScale, Math::Color& color)override;
//
//	//‰Ô‰Î‚ğ’e‚¯‚³‚¹‚é
//	void Explode()override;
//
//protected:
//
//
//	std::vector<Particle> m_circleList;		//‰Ô‰Î‚Ì‰ÎiŠÛŒ`j
//	int m_circleActiveNum;					//ŠÛŒ^‚ÌŠˆ«ó‘Ô‚Ì”		
//	std::vector<Particle2> m_lineList;		//‰Ô‰Î‚Ì‰Îi–_Œ`j
//	int m_lineActiveNum;					//ŠÛŒ^‚ÌŠˆ«ó‘Ô‚Ì”	
//
//	Math::Vector2 m_lineBaseScale;
//};
//===================================================

//===================================================
//‰Ô‰Î3i‰Ô•Ù‰æ‘œ‚¾‚¯‚Å•`‰æj
//===================================================
class Fireworks3 :public BaseFireworks
{
public:

	Fireworks3() {}
	~Fireworks3() { Release(); }

	//‰Šú‰»
	void Init()override;

	//XV
	void Update(float deltaTime)override;

	//•`‰æ
	void Draw(float backScale)override;

	//‰Ô‰Î‚ğ‘Å‚¿ã‚°‚é
	void Shoot( Math::Vector2& startPos, Math::Vector2&startMove, float beforeScale, float afterScale, Math::Color& color)override;

	//‰Ô‰Î‚ğ’e‚¯‚³‚¹‚é
	void Explode()override;

protected:

	std::vector<Particle3> m_circleList;		//‰~
	int m_circleActiveNum;

	//‰æ‘œ‚ÌŠî–{“I‚ÈŠgki100*100‚Ì‰æ‘œ‚ğ6*6‚Ì‘å‚«‚³‚É‚·‚é‚½‚ßj
	//‚±‚Ì’l‚Énoise‚Æ‚È‚éŠgk‚ğ‚©‚¯‚Ä‘å‚«‚³‚ğ’²®‚·‚é
	const float m_baseScale = 0.06f;			//•`‰æ‚·‚é‰æ‘œ‚ÌŠî€Šgk
};
//===================================================

//===================================================
//‰Ô‰Î4i‰Ô•Ù‰æ‘œ‚¾‚¯‚Å•`‰æj
//===================================================
class Fireworks4 :public BaseFireworks
{
public:

	Fireworks4() {}
	~Fireworks4() { Release(); }

	//‰Šú‰»
	void Init()override;

	//XV
	void Update(float deltaTime)override;

	//•`‰æ
	void Draw(float backScale)override;

	//‰Ô‰Î‚ğ‘Å‚¿ã‚°‚é
	void Shoot(Math::Vector2& startPos, Math::Vector2& startMove, float beforeScale, float afterScale, Math::Color& color)override;

	//‰Ô‰Î‚ğ’e‚¯‚³‚¹‚é
	void Explode()override;

protected:

	//‰ğ•ú‚·‚é
	void Release()override;

	std::vector<Particle> m_petalList;		//‰Ô‚Ñ‚ç
	int m_petalActiveNum;
};
//===================================================

//===================================================
//‰Ô‰Î5iƒgƒŒƒCƒ‹i‹OÕj•t‚«j
//===================================================
//class Fireworks5 :public BaseFireworks
//{
//public:
//
//	Fireworks5() {}
//	~Fireworks5() { Release(); }
//
//	//‰Šú‰»
//	void Init()override;
//
//	//XV
//	void Update(float deltaTime)override;
//
//	//•`‰æ
//	void Draw(float backScale)override;
//
//	//‰Ô‰Î‚ğ‘Å‚¿ã‚°‚é
//	void Shoot(Math::Vector2& startPos, Math::Vector2& startMove, float beforeScale, float afterScale, Math::Color& color)override;
//
//	//‰Ô‰Î‚ğ’e‚¯‚³‚¹‚é
//	void Explode()override;
//
//protected:
//
//	//‰ğ•ú‚·‚é
//	void Release()override;
//
//	std::vector<Particle4> m_trailList;		//‹OÕ
//	int m_trailActiveNum;
//};
//===================================================

//===================================================
//‰Ô‰Î6i‚µ‚¾‚ê–ö•—‚Ì‰Ô‰Îj
//===================================================
//class Fireworks6 :public BaseFireworks
//{
//public:
//
//	Fireworks6() {}
//	~Fireworks6() { Release(); }
//
//	//‰Šú‰»
//	void Init()override;
//
//	//XV
//	void Update(float deltaTime)override;
//
//	//•`‰æ
//	void Draw(float backScale)override;
//
//	//‰Ô‰Î‚ğ‘Å‚¿ã‚°‚é
//	void Shoot(Math::Vector2& startPos, Math::Vector2& startMove, float beforeScale, float afterScale, Math::Color& color)override;
//
//	//‰Ô‰Î‚ğ’e‚¯‚³‚¹‚é
//	void Explode()override;
//
//protected:
//
//	std::vector<Particle3> m_circleList;
//	int m_circleActiveNum;
//	const float m_baseScale = 0.02f;			//•`‰æ‚·‚é‰æ‘œ‚ÌŠî€Šgk
//
//	std::deque<TrailBuffer> m_trailBuffer;
//
//};
