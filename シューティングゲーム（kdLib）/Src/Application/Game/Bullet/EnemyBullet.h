#pragma once
#include"BulletBase.h"

class C_EnemyBullet : public C_BulletBase
{
public:

	C_EnemyBullet() = delete;
	C_EnemyBullet(KdTexture* tex) {}

	void Draw()override;
};
