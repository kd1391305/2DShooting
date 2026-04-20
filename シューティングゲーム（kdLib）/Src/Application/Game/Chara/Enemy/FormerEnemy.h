#pragma once
#include"../FormerCharaBase.h"

class C_FormerEnemy :public C_FormerCharaBase
{
public:

	C_FormerEnemy();

	//XV
	void Update()override;

	//•`‰æ
	void Draw(KdTexture* tex)override;

	//oŒ»‚³‚¹‚é
	void Spawn(const Math::Vector2& pos, const Math::Vector2& move);

	//Šˆ«ó‘Ô‚©‚Ç‚¤‚©‚ğ•Ô‚·
	bool IsActive() { return m_bActive; }

	void SetActive(bool set) { m_bActive = set; }

private:

	bool m_bActive;						//Šˆ«ó‘Ô‚©‚Ç‚¤‚©
	Math::Vector2 m_move;		//ˆÚ“®—Ê
};
