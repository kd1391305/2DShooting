#pragma once
#include"BulletBase.h"

class EnemyBullet : public BulletBase
{
public:

	EnemyBullet() = delete;
	EnemyBullet(KdTexture* tex) {}

	void Draw()override;
};
