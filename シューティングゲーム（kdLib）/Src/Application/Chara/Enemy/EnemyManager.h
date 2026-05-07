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
		float shotCoolTimeNoiseMax = 0.0f;		//クールタイムのノイズ
		float spawnShotCoolTime = 0.0f;		//スポーン時の追加クールタイム
	};

private:
	//スポーンする
	void Spawn();

	void Spawn_Boss();

	void Spawn_Row();
	void Spawn_Cross();
	void Spawn_Upper_Lower();
	void Spawn_Lower_Quick();
	void Spawn_Rotation();
	void Spawn_Explode();
	void Spawn_Explode2();
	void Spawn_Reflect();
	void Spawn_MoveLine();
	void Spawn_Line_Upper_Lower();
	void Spawn_Random1();


	//敵がスポーンするタイプ
	enum SpawnPutturn
	{
		Row,				//列になって出現
		Cross,				//敵がクロス（×）みたいにでてくる
		Upper_Lower,		//敵が上下から出てくる
		Lower_Quick,		//敵が下から素早く出てくる
		Rotation,			//敵が回転しながら出てくる
		Explode,			//敵が定位置で弾けて、敵が飛び出す
		Explode2,			//敵が定位置で敵を射出。生存時間orHpが0で終了
		Reflect,			//敵が画面内を反射しながら動き回る
		MoveLine,			//事前に決めたラインに沿って動く
		Line_Upper_Lower,	//敵が上下に出てきて避けるゲーム
		Random1,			//ランダムに敵が出てくる
		Kind
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