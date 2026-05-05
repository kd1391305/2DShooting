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
	void Spawn();

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

	//敵をスポーンさせるための変数をまとめた構造体
	struct SpawnData
	{
		Math::Vector2 pos;						//出現する場所
		Math::Vector2 radius;					//敵の大きさ
		float moveSpeed;						//移動スピード
		float moveDeg;							//移動方向
		Math::Color normalColor;				//通常の色
		Math::Color hitColor;					//当たった時の色
		float hp;								//HP
		float bulletSpeed;						//弾のスピード
		float shotCoolTime;						//クールタイム
		float shotCoolTimeNoiseMax	= 0.0f;		//クールタイムのノイズ
		float spawnShotCoolTime		= 0.0f;		//スポーン時の追加クールタイム
	};

	Game* m_pGame;

	std::vector<std::shared_ptr<BaseEnemy>> m_enemyList;		//敵
	
	const float m_spawnCoolTime = 10.0f;	//10秒ごとに敵が出現パターンに沿ってスポーンする
	float m_spawnCoolTimer;				//敵がスポーンするクールタイム
	std::queue<SpawnPutturn>m_spawnPutturnHistory;
	float m_spawnProbability[SpawnPutturn::Kind];

	std::shared_ptr<Boss> m_boss = nullptr;

	bool m_bEmptySpawnFlg = false;		//敵がいないとき、スポーンするフラグ
};