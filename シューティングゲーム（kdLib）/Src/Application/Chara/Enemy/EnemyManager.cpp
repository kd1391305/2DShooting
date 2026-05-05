#include"EnemyManager.h"
#include"../../main.h"
#include"../../Tools/RandEx/RandEx.h"
#include"../../Timer/Timer.h"
#include"../../Scene/GameScene/GameScene.h"
#include"Boss/Boss.h"
#include"../Player/Player.h"
#include"Enemies/Enemies.h"

//コンストラクタ
EnemyManager::EnemyManager()
{
	//ゲーム開始から8秒間は敵をスポーンしない
	m_spawnCoolTimer = 8.0f;

	//敵のスポーン確率
	m_spawnProbability[SpawnPutturn::Circle]	= 0.3f;
	m_spawnProbability[SpawnPutturn::Row]		= 0.3f;
	m_spawnProbability[SpawnPutturn::Square]	= 0.4f;
}

//更新
void EnemyManager::Update(float deltaTime)
{
	//スポーンタイマーを進める
	m_spawnCoolTimer -= deltaTime;
	if (m_spawnCoolTimer < 0)
	{
		m_spawnCoolTimer = 0;
		m_bEmptySpawnFlg = true;
	}
	if (m_bEmptySpawnFlg)
	{
		if (m_enemyList.empty())m_spawnCoolTimer = 0;
	}
	if (m_spawnCoolTimer <= 0)
	{
		//敵をスポーンする
		Spawn();

		float noise = randRange(0.0f, 5.0f);
		m_spawnCoolTimer = m_spawnCoolTime + noise;
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


	//ボス出現の条件
	if (Timer::Instance().GetTime() > 60)
	{
		if (!m_boss)
		{
			//ボスを出現させる
			m_boss = std::make_shared<Boss>(m_pGame);
			m_boss->Init();
			m_boss->Spawn({ SCREEN_RIGHT + m_boss->GetRadius().x,0 }, { -200, 0 });
		}
	}

	//ボスの更新
	if (m_boss)
	{
		m_boss->Update(deltaTime);
	}
}

//描画
void EnemyManager::Draw()
{
	for (auto& b : m_enemyList)if (b->IsActive())b->Draw();
	if (m_boss)m_boss->Draw();
}


//スポーンする
void EnemyManager::Spawn()
{
	//スポーンするタイプを決める
	SpawnPutturn spawnPutturn = SpawnPutturn::Circle;
	{
		float r = randRange(0, 1.0f);
		float sum = 0;
		for (int i = 0; i < SpawnPutturn::Kind; i++)
		{
			sum += m_spawnProbability[i];
			if (r <= sum)
			{
				spawnPutturn = (SpawnPutturn)i;
				break;
			}
		}
	}

	//敵をスポーンする
	switch (spawnPutturn)
	{
	case SpawnPutturn::Circle:
		Spawn_Circle();
		break;
	case SpawnPutturn::Row:
		Spawn_Row();
		break;
	case SpawnPutturn::Square:
		Spawn_Square();
		break;
	}
}

void EnemyManager::Spawn_Circle()
{
	//敵が画面右端から円を描くように左に向かって動く。
		//ある一定の場所で敵は止まる
		//敵をスポーンさせる数
	int spawnNum = 6;

	Math::Vector2 startPos = { SCREEN_RIGHT + 200,0 };
	Math::Vector2 endPos = { 200,0 };
	Math::Vector2 centerMoveSpeed = { -300,0 };
	float deltaRadian = DirectX::XMConvertToRadians(50);
	float gapCenter = 80;

	for (int i = 0; i < spawnNum; i++)
	{
		//float radian = DirectX::XMConvertToRadians((360 / (float)spawnNum) * i);

		//std::shared_ptr<appearPos2> temp = std::make_shared<appearPos2>();
		//temp->Init();
		//temp->InitSpawn(startPos, endPos, centerMoveSpeed, radian, deltaRadian, gapCenter);

		////新しく敵を作成する
		//m_appearPosList.push_back(temp);
		////敵のスポーン
		//m_appearPosList.back()->Spawn(startPos, centerMoveSpeed);
	}

	m_spawnPutturnHistory.push(SpawnPutturn::Circle);
}

void EnemyManager::Spawn_Row()
{
	//敵が列状になって出現
	//			〇〇〇〇〇
	//		〇〇〇〇〇
	//	〇〇〇〇〇
	//		〇〇〇〇〇
	//			〇〇〇〇〇

	const int groupNum = 5;
	const int appearPosNum = 5;		//１グループあたりの敵の数

	//スポーンに必要な共通の変数の初期化
	SpawnData spawnData;	
	spawnData.pos;						//出現座標
	spawnData.radius;					//出現する大きさ
	spawnData.moveSpeed;				//移動速度
	spawnData.moveDeg;					//移動する方向
	spawnData.normalColor;				//通常時の色
	spawnData.hitColor;					//当たった時の色
	spawnData.hp;						//HP
	spawnData.bulletSpeed;				//弾の速度
	spawnData.shotCoolTime;				//弾を撃つクールタイム
	spawnData.shotCoolTimeNoiseMax;		//弾を撃つクールタイムのランダム値
	spawnData.spawnShotCoolTime;		//（shotCoolTime　+ spawnShotCoolTime = スポーンしてから初めて弾を撃つまでの時間）

	Math::Vector2 appearPos[groupNum][appearPosNum];


	//各グループごとの座標を求める
	float baseX = SCREEN_WIDTH + spawnData.radius.x;;
	appearPos[0][0]={ baseX,		   0 };
	appearPos[1][0]={ baseX + 100,	 100 };
	appearPos[2][0]={ baseX + 100,	-100 };
	appearPos[3][0]={ baseX + 200,	 200 };
	appearPos[4][0]={ baseX + 200,	-200 };

	//グループの先頭座標をもとに、後続の敵の座標をセット
	float gapX = spawnData.radius.x * 2;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 1; j < 5; j++)
		{
			appearPos[i][j] = { appearPos[i][0].x + gapX * j, appearPos[i][0].y };
		}
	}

	std::shared_ptr<Enemy1>enemy;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			enemy = std::make_shared<Enemy1>();
			enemy->Init();
			enemy->Spawn(spawnData.pos, spawnData.radius, spawnData.moveSpeed, spawnData.moveDeg, spawnData.normalColor, spawnData.hitColor, spawnData.bulletSpeed, spawnData.shotCoolTime, spawnData.shotCoolTimeNoiseMax, spawnData.spawnShotCoolTime);
			m_enemyList.push_back(enemy);
		}
	}

	m_spawnPutturnHistory.push(SpawnPutturn::Row);
}

void EnemyManager::Spawn_Square()
{
	//かたい敵を出現させる
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			
		}
	}
}


