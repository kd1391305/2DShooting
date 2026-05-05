#pragma once

class BaseEnemy;

class Boss;

class Game;

//敵を管理するクラス
class EnemyManager
{
public:
	EnemyManager();

	void Update(float deltaTime);

	void Draw();

	void SetGame(Game* set) { m_pGame = set; }

	std::vector<std::shared_ptr<BaseEnemy>>& GetEnemyList() { return m_enemyList; }
	std::shared_ptr<Boss> GetBoss() { return m_boss; }

private:
	//スポーンする
	void Spawn(float deltaTime);

	void Spawn_Circle();
	void Spawn_Row();
	void Spawn_Square();

	//敵がスポーンするタイプ
	enum SpawnPutturn
	{
		Circle,		//敵がぐるぐる回って出現する
		Row,		//敵が一直線に並んで出現
		Square,
		Kind		//種類
	};

	Game* m_pGame;

	std::vector<std::shared_ptr<BaseEnemy>> m_enemyList;		//敵
	
	const float m_spawnWait = 10.0f;	//10秒ごとに敵が出現パターンに沿ってスポーンする
	float m_spawnWaitTimer;				//敵がスポーンするクールタイム
	std::queue<SpawnPutturn>m_spawnPutturnHistory;
	float m_spawnProbability[SpawnPutturn::Kind];

	std::shared_ptr<Boss> m_boss = nullptr;

	bool m_bEmptySpawnFlg = false;		//敵がいないとき、スポーンするフラグ
};