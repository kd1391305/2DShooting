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
	void Draw(KdTexture* tex);

	//‰Ô‰Î‚ğ‘Å‚¿ã‚°‚é
	void Shot(Math::Vector2 startPos, Math::Vector2 targetPos, Math::Vector2 scale,const bool bTarget = false);

	//‰Ô‰Î‚ğ’e‚¯‚³‚¹‚é
	void Explode();

private:

	//‰ğ•ú‚·‚é
	void Release();

	std::vector<Particle> m_circleList;		//‰Ô‰Î‚Ì‰ÎiŠÛŒ`j
};
//===================================================

//===================================================
//‰Ô‰Î‚Qi‰~‚Æ–_‚Å•`‰æj
//===================================================
class Fireworks2:public BaseFireworks
{
public:

	Fireworks2() {}
	~Fireworks2() { Release(); }

	//‰Šú‰»
	void Init()override;

	//XV
	void Update(float deltaTime)override;

	//•`‰æ
	void Draw(KdTexture* tex)override;

	//‰Ô‰Î‚ğ‘Å‚¿ã‚°‚é
	void Shot(Math::Vector2 startPos, Math::Vector2 targetPos, Math::Vector2 scale,const bool bTarget = false)override;

	//‰Ô‰Î‚ğ’e‚¯‚³‚¹‚é
	void Explode()override;

protected:

	//‰ğ•ú‚·‚é
	void Release()override;

	std::vector<Particle> m_circleList;		//‰Ô‰Î‚Ì‰ÎiŠÛŒ`j
	std::vector<Particle2> m_lineList;		//‰Ô‰Î‚Ì‰Îi–_Œ`j
};
//===================================================

//===================================================
//‰Ô‰Î‚Ri‰~‚Æ–_‚Å•`‰æj
//‘_‚Á‚½‚Æ‚±‚ë‚Å”š”­‚·‚é 
//===================================================
class Fireworks3 :public Fireworks2
{
public:

	Fireworks3() {}
	~Fireworks3() { Release(); }

	//“–‚½‚Á‚½‚©
	bool IsHit();

	void SetTargetPos(Math::Vector2 set) { m_targetPos = set; }

private:

	Math::Vector2 m_targetPos;				//‘_‚¤À•W
};
//===================================================

