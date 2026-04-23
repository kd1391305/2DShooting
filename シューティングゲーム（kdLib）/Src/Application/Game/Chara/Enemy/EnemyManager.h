#pragma once
#include"Enemy.h"

//敵を管理するクラス
class EnemyManager
{
public:
	EnemyManager();

	void Update(float deltaTime);

	void Draw();

	std::vector<Enemy>& GetEnemyList() { return m_enemyList; }

private:

	KdTexture m_tex;

	static const int m_poolSize = 10;							//オブジェクトプール数
	std::vector<Enemy> m_enemyList;								//敵
	static constexpr float s_spawnProbability = 0.1f;			//敵発生確率（毎フレームあたり）
};