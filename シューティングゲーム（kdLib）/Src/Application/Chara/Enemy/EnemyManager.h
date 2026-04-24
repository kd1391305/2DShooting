#pragma once

class BaseEnemy;

//敵を管理するクラス
class EnemyManager
{
public:
	EnemyManager();

	void Update(float deltaTime);

	void Draw();

	std::vector<std::shared_ptr<BaseEnemy>>& GetEnemyList() { return m_enemyList; }

private:


	//スポーンする
	void SpawnPutturn(float deltaTime);

	KdTexture m_tex;

	std::vector<std::shared_ptr<BaseEnemy>> m_enemyList;		//敵
	float m_spawnProbability;									//敵発生確率（毎秒あたり）
	long m_spawnCnt;											//敵がスポーンした数を求める
};