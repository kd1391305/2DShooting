#include"EnemyManager.h"
#include"../../main.h"
#include"../../Tools/RandEx/RandEx.h"
#include"../../Timer/Timer.h"
#include"../../Scene/GameScene/GameScene.h"
#include"Enemies/Enemy1.h"
#include"Enemies/Enemy2.h"

//コンストラクタ
EnemyManager::EnemyManager()
{
	m_spawnWait[SpawnType::Normal]	=  1.0f;
	m_spawnWait[SpawnType::Circle]	= 20.0f;
	m_spawnWait[SpawnType::Row]		= 10.0f;

	for (int i = 0; i < SpawnType::Kind; i++)
	{
		m_spawnWaitTimer[i] = m_spawnWait[i];
	}
}

//更新
void EnemyManager::Update(float deltaTime)
{
	for (auto&t:m_spawnWaitTimer)
	{
		t -= deltaTime;
		if (t < 0)t = 0;
	}


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

	//敵をスポーンするか決める
	//スポーン処理も行う
	Spawn(deltaTime);
}

//描画
void EnemyManager::Draw()
{
	for (auto& b : m_enemyList)if (b->IsActive())b->Draw();
}


//スポーンする
void EnemyManager::Spawn(float deltaTime)
{
	//スポーン待機時間が０になったら確率でスポーンさせる

	//敵が画面右端から左端に向けて動く
	if (m_spawnWaitTimer[SpawnType::Normal] <= 0)
	{
		if (HitGacha(10.0f * deltaTime))
		{
			//新しく敵を作成する
			std::shared_ptr<Enemy1>enemy;
	
			enemy = std::make_shared<Enemy1>();
			enemy->Init();
			enemy->SetPos({ SCREEN_RIGHT + enemy->GetRadius().x,randRange(SCREEN_BOTTOM + enemy->GetRadius().y,SCREEN_TOP - enemy->GetRadius().y) });
			enemy->Spawn();
			m_enemyList.push_back(enemy);
			
			m_spawnWaitTimer[SpawnType::Normal] = m_spawnWait[SpawnType::Normal];
		}
	}

	//敵が画面右端から円を描くように左に向かって動く。
	//ある一定の場所で敵は止まる
	if (m_spawnWaitTimer[SpawnType::Circle] <= 0)
	{
		if (HitGacha(10.0f * deltaTime))
		{
			//敵をスポーンさせる数
			int spawnNum = 6;

			Math::Vector2 startPos = { SCREEN_RIGHT + 300,0 };
			Math::Vector2 endPos = { 200,0 };
			Math::Vector2 centerMoveSpeed = { -300,0 };
			float deltaRadian = DirectX::XMConvertToRadians(50);
			float gapCenter = 80;

			for (int i = 0; i < spawnNum; i++)
			{
				float radian = DirectX::XMConvertToRadians((360 / (float)spawnNum) * i);

				std::shared_ptr<Enemy2> temp = std::make_shared<Enemy2>();
				temp->Init();
				temp->InitSpawn(startPos, endPos, centerMoveSpeed, radian, deltaRadian, gapCenter);

				//新しく敵を作成する
				m_enemyList.push_back(temp);
				//敵のスポーン
				m_enemyList.back()->Spawn();
			}

			m_spawnWaitTimer[SpawnType::Circle] = m_spawnWait[SpawnType::Circle];
		}
	}

	//敵が列状になって敵が出現
	if (m_spawnWaitTimer[SpawnType::Row] <= 0)
	{
		if (HitGacha(10.0f * deltaTime))
		{
			//列にする敵の数
			int enemyNum = 5;
			float y = randRange(-300, 300);
			std::shared_ptr<Enemy1>enemy;
			for (int i = 0; i < enemyNum; i++)
			{
				enemy = std::make_shared<Enemy1>();
				enemy->Init();
				enemy->SetPos({ SCREEN_RIGHT + 100 + 50.0f * i,y });
				enemy->Spawn();
				m_enemyList.push_back(enemy);
			}

			m_spawnWaitTimer[SpawnType::Row] = m_spawnWait[SpawnType::Row];
		}
	}
}


