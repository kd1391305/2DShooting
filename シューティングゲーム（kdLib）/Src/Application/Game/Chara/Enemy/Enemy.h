#pragma once
#include"../CharaBase.h"

class BulletManager;

class Enemy :public CharaBase
{
public:

	Enemy();

	//更新
	void Update(float deltaTime);

	//この関数は呼び出さないように注意する
	void Draw()override {}

	//描画
	void Draw(KdTexture* tex);

	bool IsActive() { return m_bActive; }

	void SetActive(bool set) { m_bActive = set; }

	void Spawn(Math::Vector2 pos,Math::Vector2 move);

	Math::Vector2 GetPos() { return m_pos; }

	static void SetBulletManager(BulletManager* set) { s_pBulletManager = set; }

private:
	static BulletManager* s_pBulletManager;

	bool m_bActive;			//敵の活性状態
	INT8	m_shotWait;														//撃つまでのクールタイム
	static constexpr float s_shotProbability = 0.01f;			//撃つ確率
};
