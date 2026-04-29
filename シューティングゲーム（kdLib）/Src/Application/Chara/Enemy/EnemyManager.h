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
	void Spawn(float deltaTime);

	//敵がスポーンするタイプ
	enum SpawnType
	{
		Normal,		//普通に右端からランダムにスポーン
		Circle,		//敵がぐるぐる回って出現する
		Row,		//敵が一直線に並んで出現
		Kind		//種類
	};


	std::vector<std::shared_ptr<BaseEnemy>> m_enemyList;		//敵
	float m_spawnProbability;									//敵発生確率（毎秒あたり）
	float m_spawnWaitTimer[SpawnType::Kind];					//スポーン待ち時間を測る（0になったらスポーンできる）
	float m_spawnWait[SpawnType::Kind];							//スポーン待ち時間
};