#include"EnemyManager.h"
#include"../../main.h"
#include"../../Tools/RandEx/RandEx.h"
#include"../../Timer/Timer.h"
#include"../../Scene/GameScene/GameScene.h"
#include"Enemy1.h"

//コンストラクタ
EnemyManager::EnemyManager()
{
	//画像の読み込み
	m_tex.Load("Texture/Enemy.png");

	m_spawnProbability = 10.0f;			//毎秒のスポーン確率		

	m_spawnCnt = 0;
}

//更新
void EnemyManager::Update(float deltaTime)
{
	for (auto itr = m_enemyList.begin(); itr != m_enemyList.end();)
	{
		//活性状態の敵は更新処理
		if ((*itr)->IsActive())
		{ 
			(*itr)->Update(deltaTime);
		}
		//非活性状態の敵は
		else
		{
			//データを削除する
			itr = m_enemyList.erase(itr);
			continue;
		}
		itr++;
	}

	//敵をスポーンさせるかどうかを決める
	bool spawnFlg = HitGacha(m_spawnProbability * deltaTime);
	if (spawnFlg)
	{
		//スポーンする
		SpawnPutturn(deltaTime);
	}
}

//描画
void EnemyManager::Draw()
{
	for (auto& b : m_enemyList)if (b->IsActive())b->Draw(&m_tex);
}


//スポーンする
void EnemyManager::SpawnPutturn(float deltaTime)
{
	if (HitGacha(0.1f * deltaTime))
	{
		//新しく敵を作成する
		m_enemyList.push_back(std::make_shared<Enemy1>());

		//敵の初期化
		m_enemyList.back()->Init();
		m_enemyList.back()->SetID(m_spawnCnt);

		//敵のスポーン
		m_enemyList.back()->Spawn();

		//スポーンカウンターを進める
		m_spawnCnt++;
	}
	else
	{
		//新しく敵を作成する
		m_enemyList.push_back(std::make_shared<BaseEnemy>());

		//敵の初期化
		m_enemyList.back()->Init();
		m_enemyList.back()->SetID(m_spawnCnt);

		//敵のスポーン
		m_enemyList.back()->Spawn();

		//スポーンカウンターを進める
		m_spawnCnt++;

	}
}


