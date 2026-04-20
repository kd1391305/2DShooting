#pragma once
#include"BulletBase.h"

class C_PlayerBullet : public C_BulletBase
{
public:

	C_PlayerBullet() = delete;
	C_PlayerBullet(KdTexture* tex) { m_pTex = tex; }

	void Update()override;

	void Draw()override;

public:

	Math::Matrix m_pileMat[6];		//‰æ‘œ‚ğd‚Ë‚é—p
};
