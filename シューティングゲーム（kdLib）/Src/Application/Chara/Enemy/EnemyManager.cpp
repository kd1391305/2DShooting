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
	//ゲーム開始から25秒間は敵をスポーンしない
	m_spawnCoolTimer = 25.0f;

	//敵のスポーン確率
	//重みを代入
	m_spawnProbability[SpawnPutturn::Row]					= 2;
	m_spawnProbability[SpawnPutturn::Cross]					= 2;
	m_spawnProbability[SpawnPutturn::Upper_Lower]			= 2;
	m_spawnProbability[SpawnPutturn::Lower_Quick]			= 3;
	m_spawnProbability[SpawnPutturn::Rotation]				= 3;
	m_spawnProbability[SpawnPutturn::Explode]				= 1;
	m_spawnProbability[SpawnPutturn::Explode2]				= 1;
	m_spawnProbability[SpawnPutturn::Reflect]				= 1;
	m_spawnProbability[SpawnPutturn::MoveLine]				= 2;
	m_spawnProbability[SpawnPutturn::Line_Upper_Lower]		= 0.1;
	m_spawnProbability[SpawnPutturn::Random1]				= 0.3;
	m_spawnProbability[SpawnPutturn::Random2]				= 1;

	//重みの合計を求める
	float sum = 0;
	for (int i = 0; i < SpawnPutturn::Kind; i++)
	{
		sum += m_spawnProbability[i];
	}
	for (int i = 0; i < SpawnPutturn::Kind; i++)
	{
		m_spawnProbability[i] /= sum;
	}
}

//更新
void EnemyManager::Update(float deltaTime)
{
	//ボスがいないときだけ敵をスポーンさせる
	if (!m_boss && !m_bSpawnBoss)
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
	}

	for (int i = m_enemyList.size() - 1; i >= 0; i--)
	{
		if (m_enemyList[i]->IsActive())
		{
			m_enemyList[i]->Update(deltaTime);
		}
		//非活性状態の敵はデータを削除する
		else
		{
			m_enemyList.erase(m_enemyList.begin() + i);
		}
	}


	//ボス出現の条件
	
	if (Timer::Instance().GetTime() >120 && !m_pGame->GetGameClearFlg())
	{
		if (!m_boss)
		{
			m_bSpawnBoss = true;
		}
		if (m_bSpawnBoss && m_enemyList.empty())
		{
			Spawn_Boss();
			m_bSpawnBoss = false;
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
	SpawnPutturn spawnPutturn = SpawnPutturn::Row;
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
	case SpawnPutturn::Row:
		Spawn_Row();
		break;
	case SpawnPutturn::Cross:
		Spawn_Cross();
		break;
	case SpawnPutturn::Upper_Lower:
		Spawn_Upper_Lower();
		break;
	case SpawnPutturn::Lower_Quick:
		Spawn_Lower_Quick();
		break;
	case SpawnPutturn::Rotation:
		Spawn_Rotation();
		break;
	case SpawnPutturn::Explode:
		Spawn_Explode();
		break;
	case SpawnPutturn::Explode2:
		Spawn_Explode2();
		break;
	case SpawnPutturn::Reflect:
		Spawn_Reflect();
		break;
	case SpawnPutturn::MoveLine:
		Spawn_MoveLine();
		break;
	case SpawnPutturn::Line_Upper_Lower:
		Spawn_Line_Upper_Lower();
		break;
	case SpawnPutturn::Random1:
		Spawn_Random1();
		break;
	case SpawnPutturn::Random2:
		Spawn_Random2();
		break;
	}
}

//ボスを出現させる
void EnemyManager::Spawn_Boss()
{
	//スポーンに必要な共通の変数の初期化
	SpawnData spawnData;
	spawnData.radius = {160,160 };									//出現する大きさ
	spawnData.pos = { SCREEN_RIGHT + spawnData.pos.x,0 };			//出現座標
	spawnData.moveSpeed = 200;										//移動速度
	spawnData.moveDeg = 180;										//移動する方向
	spawnData.normalColor = { 1,1,1,1 };							//通常時の色
	spawnData.hitColor = { 1,0,0,1 };								//当たった時の色
	spawnData.hp = 3000;											//HP
	spawnData.bulletSpeed = 300;									//弾の速度
	spawnData.shotCoolTime = 0.2f;									//弾を撃つクールタイム
	spawnData.shotCoolTimeNoiseMax = 0.0f;							//弾を撃つクールタイムのランダム値
	spawnData.spawnShotCoolTime = 5.0f;								//（shotCoolTime　+ spawnShotCoolTime = スポーンしてから初めて弾を撃つまでの時間）

	m_boss = std::make_shared<Boss>(m_pGame);
	m_boss->Init();
	m_boss->InitOriginal();
	m_boss->AddFireworksNum(20);
	m_boss->Spawn(spawnData.pos, spawnData.radius, spawnData.moveSpeed, spawnData.moveDeg, spawnData.normalColor, spawnData.hitColor, spawnData.hp, spawnData.bulletSpeed, spawnData.shotCoolTime, spawnData.shotCoolTimeNoiseMax, spawnData.spawnShotCoolTime);
}

void EnemyManager::Spawn_Row()
{
	//敵が列状になって出現
	// 列の最後の敵に花火を５発持たせる
	//			〇〇〇〇〇
	//		〇〇〇〇〇
	//	〇〇〇〇〇
	//		〇〇〇〇〇
	//			〇〇〇〇〇

	const int groupNum = 5;
	const int enemyNum = 5;		//１グループあたりの敵の数

	//スポーンに必要な共通の変数の初期化
	SpawnData spawnData;	
	//spawnData.pos;								//出現座標
	spawnData.radius = {32,32};						//出現する大きさ
	spawnData.moveSpeed = 200;						//移動速度
	spawnData.moveDeg = 180;						//移動する方向
	spawnData.normalColor = {1.0f,0.2f,0.2f,1.0f};	//通常時の色
	spawnData.hitColor = { 1.0f,0.1f,0.1f,1.0f };	//当たった時の色
	spawnData.hp = 10;								//HP
	spawnData.bulletSpeed = 300;					//弾の速度
	spawnData.shotCoolTime = 3.0f;					//弾を撃つクールタイム
	spawnData.shotCoolTimeNoiseMax=2.0f;			//弾を撃つクールタイムのランダム値
	spawnData.spawnShotCoolTime = 0;				//（shotCoolTime　+ spawnShotCoolTime = スポーンしてから初めて弾を撃つまでの時間）

	Math::Vector2 spawnPos[groupNum][enemyNum];

	//各グループごとの座標を求める
	float baseX = SCREEN_WIDTH + spawnData.radius.x;
	spawnPos[0][0]={ baseX,		   0 };
	spawnPos[1][0]={ baseX + 120,	 150 };
	spawnPos[2][0]={ baseX + 120,	-150 };
	spawnPos[3][0]={ baseX + 240,	 250 };
	spawnPos[4][0]={ baseX + 240,	-250 };

	//グループの先頭座標をもとに、後続の敵の座標をセット
	float gapX = spawnData.radius.x * 2;
	for (int i = 0; i < groupNum; i++)
	{
		for (int j = 1; j < enemyNum; j++)
		{
			spawnPos[i][j] = { spawnPos[i][0].x + gapX * j, spawnPos[i][0].y };
		}
	}

	std::shared_ptr<Enemy1>enemy;
	for (int i = 0; i < groupNum; i++)
	{
		for (int j = 0; j < enemyNum; j++)
		{
			enemy = std::make_shared<Enemy1>();
			enemy->Init();
			//列の最後の敵には追加で花火を3発持たせる
			if (j == enemyNum - 1)enemy->AddFireworksNum(2);
			else enemy->AddFireworksNum(-3);
			enemy->Spawn(spawnPos[i][j], spawnData.radius, spawnData.moveSpeed, spawnData.moveDeg, spawnData.normalColor, spawnData.hitColor, spawnData.hp, spawnData.bulletSpeed, spawnData.shotCoolTime, spawnData.shotCoolTimeNoiseMax, spawnData.spawnShotCoolTime);
			m_enemyList.push_back(enemy);
		}
	}

	m_spawnPutturnHistory.push(SpawnPutturn::Row);
}

void EnemyManager::Spawn_Cross()
{
	//敵をクロスさせて登場
	//	〇　〇
	//	　〇
	//	〇　〇

	const int enemyNum = 5;
	Math::Vector2 spawnPos[enemyNum];
	Math::Color normalColor[enemyNum];
	Math::Color hitColor[enemyNum];
	int		shotNum = 2;
	float	shotArcRadian = DirectX::XMConvertToRadians(10.0f);

	//スポーンに必要な共通の変数の初期化
	SpawnData spawnData;
	//spawnData.pos;					//出現座標
	spawnData.radius = { 64,64 };		//出現する大きさ
	spawnData.moveSpeed = 120;			//移動速度
	spawnData.moveDeg = 180;			//移動する方向
	//spawnData.normalColor = { 1,0,0,1 };//通常時の色
	//spawnData.hitColor = { 0.7f,0,0,1 };//当たった時の色
	spawnData.hp = 100;					//HP
	spawnData.bulletSpeed = 250;		//弾の速度
	spawnData.shotCoolTime = 2.0f;		//弾を撃つクールタイム
	spawnData.shotCoolTimeNoiseMax = 0.0f;//弾を撃つクールタイムのランダム値
	spawnData.spawnShotCoolTime = 0;	//（shotCoolTime　+ spawnShotCoolTime = スポーンしてから初めて弾を撃つまでの時間）

	//出現座標を決める
	{
		float offsetPos = spawnData.radius.x * 2;
		float centerPosX = { SCREEN_RIGHT + spawnData.radius.x * 3 + offsetPos };
		spawnPos[0] = { centerPosX, 0 };
		spawnPos[1] = { centerPosX - offsetPos,offsetPos };
		spawnPos[2] = { centerPosX + offsetPos,offsetPos };
		spawnPos[3] = { centerPosX - offsetPos,-offsetPos };
		spawnPos[4] = { centerPosX + offsetPos,-offsetPos };
	}

	//色を決める
	{
		//真ん中とそれ以外で色を分ける
		normalColor[0]	=	{ 0.8f,0.8f,0.8f,1.0f };
		hitColor[0]		=	{ 0.8f,0.6f,0.6f,1.0f };


		for (int i = 1; i < enemyNum; i++)
		{
			normalColor[i]	= { 0.8f,0.8f,0.0f,1.0f };
			hitColor[i]		= { 0.8f,0.5f,0.0f,1.0f };
		}
	}
	std::shared_ptr<Enemy2>enemy;
	for (int i = 0; i < 5; i++)
	{
		enemy = std::make_shared<Enemy2>();
		enemy->Init();
		enemy->InitOriginal(shotNum, shotArcRadian);
		if(i==0)enemy->AddFireworksNum(5);
		enemy->Spawn(spawnPos[i], spawnData.radius, spawnData.moveSpeed, spawnData.moveDeg, normalColor[i], hitColor[i], spawnData.hp, spawnData.bulletSpeed, spawnData.shotCoolTime, spawnData.shotCoolTimeNoiseMax, spawnData.spawnShotCoolTime);
		m_enemyList.push_back(enemy);
	}

	if (rand() % 2)
	{
		Math::Vector2 pos = { SCREEN_RIGHT + 100,250 };
		if (rand() % 2)pos.y *= -1;
		Spawn_Row2(randRange(1, 5.9), pos, 300, 180);
	}
}

//上と下から敵を出現させる
void EnemyManager::Spawn_Upper_Lower()
{
	//		〇　〇
	//		　〇
	//		　↓
	//			
	//
	//					↑
	//					〇
	//				　〇　〇
	//				
	// 
	
	const int groupNum = 2;
	const int enemyNum = 3;

	
	int firstGroup, secondGroup;
	firstGroup = rand() % groupNum;
	secondGroup = (firstGroup + 1) % groupNum;

	//左から先に出現するか右からか？
	bool leftFirst = true;
	if (rand() % 2)leftFirst = false;

	//上から先に出現させるか？下からか？
	bool topFirst = true;
	if (rand() % 2)topFirst = false;

	//出現座標と大きさは各キャラごとに違う
	Math::Vector2 spawnPos[groupNum][enemyNum];
	Math::Vector2 radius[groupNum][enemyNum];
	float hp[groupNum][enemyNum];

	//色と進む向きはグループごとに変化
	Math::Color normalColor[groupNum];
	Math::Color hitColor[groupNum];
	float moveDeg[groupNum];

	//スポーンに必要な共通の変数の初期化
	SpawnData spawnData;
	//spawnData.pos;					//出現座標
	//spawnData.radius = { 64,64 };		//出現する大きさ
	spawnData.moveSpeed = 80;			//移動速度
	//spawnData.moveDeg = 180;			//移動する方向
	//spawnData.normalColor = { 1,0,0,1 };//通常時の色
	//spawnData.hitColor = { 0.7f,0,0,1 };//当たった時の色
	//spawnData.hp = 100;					//HP
	spawnData.bulletSpeed = 150;		//弾の速度
	spawnData.shotCoolTime = 2.0f;		//弾を撃つクールタイム
	spawnData.shotCoolTimeNoiseMax = 0.0f;//弾を撃つクールタイムのランダム値
	spawnData.spawnShotCoolTime = 0;	//（shotCoolTime　+ spawnShotCoolTime = スポーンしてから初めて弾を撃つまでの時間）

	//大きさ、HPを決める
	//グループの先頭に立つ敵
	for (int i = 0; i < groupNum; i++)
	{
		radius[i][0] = { 64, 64 };
		hp[i][0] = 80;
	}

	//グループの先頭以外の敵
	for (int i = 0; i < groupNum; i++)
	{
		for (int j = 1; j < enemyNum; j++)
		{
			radius[i][j] = { 48,48 };
			hp[i][j] = 40;
		}
	}

	//グループの色
	normalColor[0]	= { 0.7f,0.9f,0.1f,1.0f };
	hitColor[0]		= { 0.7f,0.7f,0.0f,1.0f };
	normalColor[1]	= { 0.5f,0.7f,0.7f,1.0f };
	hitColor[1]		= { 0.5f,0.6f,0.6f,1.0f };
	
	//座標を決める
	float leftSpawnPos = randRange(SCREEN_LEFT / 3.0f, 0);
	float rightSpawnPos = randRange(50, SCREEN_RIGHT / 2.0f);
	float topSpawnPos = SCREEN_TOP + radius[0][0].y;
	float bottomSpawnPos = SCREEN_BOTTOM - radius[0][0].y;
	
	float offsetPos = radius[0][0].x * 2;

	float waitTime = 3.0f;		//3秒後に出現
	float waitPos = waitTime * spawnData.moveSpeed;	//出現場所のずれる量

	if (leftFirst)
	{
		spawnPos[firstGroup][0].x = leftSpawnPos;
		spawnPos[firstGroup][1].x = leftSpawnPos + offsetPos;
		spawnPos[firstGroup][2].x = leftSpawnPos - offsetPos;
		spawnPos[secondGroup][0].x = rightSpawnPos;
		spawnPos[secondGroup][1].x = rightSpawnPos + offsetPos;
		spawnPos[secondGroup][2].x = rightSpawnPos - offsetPos;
	}
	else
	{
		spawnPos[secondGroup][0].x = leftSpawnPos;
		spawnPos[secondGroup][1].x = leftSpawnPos + offsetPos;
		spawnPos[secondGroup][2].x = leftSpawnPos - offsetPos;
		spawnPos[firstGroup][0].x = rightSpawnPos;
		spawnPos[firstGroup][1].x = rightSpawnPos + offsetPos;
		spawnPos[firstGroup][2].x = rightSpawnPos - offsetPos;
	}

	if (topFirst)
	{
		spawnPos[firstGroup][0].y = topSpawnPos;
		spawnPos[firstGroup][1].y= topSpawnPos + offsetPos;
		spawnPos[firstGroup][2].y = topSpawnPos + offsetPos;
		spawnPos[secondGroup][0].y = bottomSpawnPos - waitPos;
		spawnPos[secondGroup][1].y = bottomSpawnPos - offsetPos -waitPos;
		spawnPos[secondGroup][2].y = bottomSpawnPos - offsetPos - waitPos;
		moveDeg[firstGroup] = -90;
		moveDeg[secondGroup] = 90;
	}
	else
	{
		spawnPos[secondGroup][0].y = topSpawnPos + waitPos;
		spawnPos[secondGroup][1].y = topSpawnPos + offsetPos + waitPos;
		spawnPos[secondGroup][2].y = topSpawnPos + offsetPos + waitPos;
		spawnPos[firstGroup][0].y = bottomSpawnPos;
		spawnPos[firstGroup][1].y = bottomSpawnPos - offsetPos;
		spawnPos[firstGroup][2].y = bottomSpawnPos - offsetPos;
		moveDeg[firstGroup] = 90;
		moveDeg[secondGroup] = -90;
	}


	int shotNum = randRange(2, 4);
	float shotArcRadian = DirectX::XMConvertToRadians(10 * shotNum);

	std::shared_ptr<Enemy2>enemy;
	for (int i = 0; i < groupNum; i++)
	{
		for (int j = 0; j < enemyNum; j++)
		{
			enemy = std::make_shared<Enemy2>();
			enemy->Init();
			enemy->InitOriginal(shotNum, shotArcRadian);
			if (j == 0)enemy->AddFireworksNum(3);
			enemy->Spawn(spawnPos[i][j], radius[i][j], spawnData.moveSpeed, moveDeg[i], normalColor[i], hitColor[i], hp[i][j], spawnData.bulletSpeed, spawnData.shotCoolTime, spawnData.shotCoolTimeNoiseMax, spawnData.spawnShotCoolTime);
			m_enemyList.push_back(enemy);
		}
	}

}

void EnemyManager::Spawn_Lower_Quick()
{
	//スポーンに必要な共通の変数の初期化
	SpawnData spawnData;
	spawnData.radius = { 32,32 };																						//出現する大きさ
	//spawnData.pos = { SCREEN_RIGHT + spawnData.radius.x, randRange(SCREEN_BOTTOM / 2.0f,SCREEN_TOP / 2.0f) };				//出現座標
	spawnData.moveSpeed = 300;							//移動速度
	//spawnData.moveDeg = 180;							//移動する方向
	spawnData.normalColor = { 0.5f,0.1f,0.6f,1.0f };	//通常時の色
	spawnData.hitColor = { 0.5f,0.0f,0.5f,1.0f };		//当たった時の色
	spawnData.hp = 10;									//HP
	spawnData.bulletSpeed = 300;							//弾の速度
	spawnData.shotCoolTime = 2.0f;						//弾を撃つクールタイム
	spawnData.shotCoolTimeNoiseMax = 0;				//弾を撃つクールタイムのランダム値
	spawnData.spawnShotCoolTime = 0.1f;					//（shotCoolTime　+ spawnShotCoolTime = スポーンしてから初めて弾を撃つまでの時間）

	//進む向き（右下から左上 or 左下から右上）
	float moveDeg = randRange(40, 50);
	if (rand() % 2)moveDeg += 90;
	float moveRadian = DirectX::XMConvertToRadians(moveDeg);

	const int enemyNum = 6;
	Math::Vector2 spawnPos[enemyNum];
	float offset = spawnData.radius.x * 2;

	//座標を決める
	{
		Math::Vector2 vec;
		vec.x = cosf(moveRadian);
		vec.y = sinf(moveRadian);
		vec *= -1;
		float baseX;
		if (vec.x > 0)
		{
			baseX = randRange(100, 300);
		}
		else
		{
			baseX = randRange(0, 100);
		}

		for (int i = 0; i < enemyNum; i++)
		{
			spawnPos[i] = vec * offset * i;
			spawnPos[i].x += baseX;
			spawnPos[i].y += SCREEN_BOTTOM - spawnData.radius.y;
		}
	}

	std::shared_ptr<Enemy2>enemy;
	int shotNum = randRange(2, 4);
	int shotArcRadian = DirectX::XMConvertToRadians(20 * shotNum);
	for (int i = 0; i < enemyNum; i++)
	{
		enemy = std::make_shared<Enemy2>();
		enemy->Init();
		enemy->InitOriginal(shotNum,shotArcRadian);
		enemy->Spawn(spawnPos[i], spawnData.radius, spawnData.moveSpeed, moveDeg, spawnData.normalColor, spawnData.hitColor, spawnData.hp, spawnData.bulletSpeed, spawnData.shotCoolTime, spawnData.shotCoolTimeNoiseMax, spawnData.spawnShotCoolTime);
		m_enemyList.push_back(enemy);
	}
}

void EnemyManager::Spawn_Rotation()
{
	//スポーンに必要な共通の変数の初期化
	SpawnData spawnData;
	spawnData.radius = { 40,40 };									//出現する大きさ
	//spawnData.pos = { SCREEN_RIGHT + spawnData.pos.x,0 };			//出現座標
	spawnData.moveSpeed = 100;										//移動速度
	//spawnData.moveDeg = 180;										//移動する方向
	spawnData.normalColor = { 1,1,1,1 };							//通常時の色
	spawnData.hitColor = { 1,0.5f,0.5f,1 };								//当たった時の色
	spawnData.hp = 50;											//HP
	spawnData.bulletSpeed = 200;									//弾の速度
	spawnData.shotCoolTime = 0.4f;									//弾を撃つクールタイム
	spawnData.shotCoolTimeNoiseMax = 0.2f;							//弾を撃つクールタイムのランダム値
	spawnData.spawnShotCoolTime = 1.0f;								//（shotCoolTime　+ spawnShotCoolTime = スポーンしてから初めて弾を撃つまでの時間）


	//この座標データを作成しておく
	// このデータのうち３つだけを取り出し、使用する
	// 〇 〇 〇
	// 〇 〇 〇
	// 〇 〇 〇
	const int enemyNum = 3;

	const int height = enemyNum;
	const int width = enemyNum;
	Math::Vector2 pos[height][width];
	float offset = 100;
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			pos[h][w] = { offset * h,offset * w };
		}
	}
	std::vector<int> hNumberList;
	std::vector<int> wNumberList;
	for (int i = 0; i < enemyNum; i++)
	{
		hNumberList.push_back(i);
		wNumberList.push_back(i);
	}

	//敵がスポーンする座標
	Math::Vector2 spawnPos[enemyNum];

	int h;
	int w;
	int r;
	for (int i = 0; i < enemyNum; i++)
	{
		//行の番号を取り出す
		r = rand() % hNumberList.size();
		h = hNumberList[r];
		hNumberList.erase(hNumberList.begin() + r);

		//列の番号を取り出す
		r = rand() % wNumberList.size();
		w = wNumberList[r];
		wNumberList.erase(wNumberList.begin() + r);
		spawnPos[i] = pos[h][w];
	}

	//取り出した座標に敵が出てくる方向の座標を足す
	
	int direction = rand() % 4;
	int offsetX = randRange(SCREEN_LEFT, SCREEN_RIGHT) * 0.3f;
	int offsetY = randRange(SCREEN_BOTTOM, SCREEN_TOP) * 0.3f;
	switch (direction)
	{
		//左から出現
	case 0:
		for (int i = 0; i < enemyNum; i++)
		{
			spawnPos[i].x += SCREEN_LEFT - spawnData.radius.x - offset * enemyNum;
			spawnPos[i].y += offsetY;
		}
		break;
		//右から出現
	case 1:
		for (int i = 0; i < enemyNum; i++)
		{
			spawnPos[i].x += SCREEN_RIGHT + spawnData.radius.x;
			spawnPos[i].y += offsetY;
		}
		break;
		//上から出現
	case 2:
		for (int i = 0; i < enemyNum; i++)
		{
			spawnPos[i].x += offsetX;
			spawnPos[i].y += SCREEN_TOP + spawnData.radius.y;
		}
		break;
		//左から出現
	case 3:
		for (int i = 0; i < enemyNum; i++)
		{
			spawnPos[i].x += offsetX;
			spawnPos[i].y += SCREEN_BOTTOM - spawnData.radius.y- offset * enemyNum;
		}
		break;
	}

	int moveDeg[4] = { 0,180,270,90 };
	
	float deltaRadian = DirectX::XMConvertToRadians(50);
	float shotNum = 2;
	float shotArcRadian = DirectX::XMConvertToRadians(10);
	std::shared_ptr<Enemy5>enemy;
	for (int i = 0; i < enemyNum; i++)
	{
		enemy = std::make_shared<Enemy5>();
		enemy->Init();
		enemy->InitOriginal(deltaRadian,shotNum,shotArcRadian);
		enemy->Spawn(spawnPos[i], spawnData.radius, spawnData.moveSpeed, moveDeg[direction], spawnData.normalColor, spawnData.hitColor, spawnData.hp, spawnData.bulletSpeed, spawnData.shotCoolTime, spawnData.shotCoolTimeNoiseMax, spawnData.spawnShotCoolTime);
		m_enemyList.push_back(enemy);
	}

	if (rand() % 2)
	{
		Math::Vector2 pos = { SCREEN_RIGHT + 300,randRange(-200,200)};
		if (rand() % 2)pos.y *= -1;
		Spawn_Row3(randRange(2, 5), pos, randRange(250, 350), 180);
	}
}

void EnemyManager::Spawn_Explode()
{
	//スポーンに必要な共通の変数の初期化
	SpawnData spawnData;
	spawnData.radius = { 40,40 };																						//出現する大きさ
	//spawnData.pos = { SCREEN_RIGHT + spawnData.radius.x, randRange(SCREEN_BOTTOM / 2.0f,SCREEN_TOP / 2.0f) };				//出現座標
	spawnData.moveSpeed = 300;							//移動速度
	spawnData.moveDeg = 180;							//移動する方向
	spawnData.normalColor = { 0.7f,0.9f,0.1f,1.0f };	//通常時の色
	spawnData.hitColor = { 0.7f,0.8f,0.0f,1.0f };		//当たった時の色
	spawnData.hp = 999;									//HP
	spawnData.bulletSpeed = 0;							//弾の速度
	spawnData.shotCoolTime = 999;						//弾を撃つクールタイム
	spawnData.shotCoolTimeNoiseMax = 999;				//弾を撃つクールタイムのランダム値
	spawnData.spawnShotCoolTime = 999;					//（shotCoolTime　+ spawnShotCoolTime = スポーンしてから初めて弾を撃つまでの時間）

	std::shared_ptr<Enemy3>enemy;

	float topPos = randRange(SCREEN_BOTTOM / 2.0f, SCREEN_TOP / 2.0f);
	float offset = 150;

	for (int i = -1; i < 2; i++)
	{
		spawnData.pos = { SCREEN_RIGHT + spawnData.radius.x + fabs(offset * i), topPos - offset * i };
		float explodePosX = 0 + fabs(offset * i);
		float explodeEnemyNum = randRange(16, 25);
		float explodeEnemySpeed = randRange(400, 500);

		enemy = std::make_shared<Enemy3>(this);
		enemy->Init();
		enemy->InitOriginal(explodePosX, explodeEnemyNum, explodeEnemySpeed);
		enemy->Spawn(spawnData.pos, spawnData.radius, spawnData.moveSpeed, spawnData.moveDeg, spawnData.normalColor, spawnData.hitColor, spawnData.hp, spawnData.bulletSpeed, spawnData.shotCoolTime, spawnData.shotCoolTimeNoiseMax, spawnData.spawnShotCoolTime);
		m_enemyList.push_back(enemy);
	}
}

void EnemyManager::Spawn_Explode2()
{
	//スポーンに必要な共通の変数の初期化
	SpawnData spawnData;
	spawnData.radius = { 40,40 };																						//出現する大きさ
	//spawnData.pos = { SCREEN_RIGHT + spawnData.radius.x, randRange(SCREEN_BOTTOM / 2.0f,SCREEN_TOP / 2.0f) };				//出現座標
	spawnData.moveSpeed = 300;							//移動速度
	spawnData.moveDeg = 180;							//移動する方向
	spawnData.normalColor = { 0.5f,0.7f,0.0f,1.0f };	//通常時の色
	spawnData.hitColor = { 0.7f,0.8f,0.0f,1.0f };		//当たった時の色
	spawnData.hp = 20;									//HP
	spawnData.bulletSpeed = 0;							//弾の速度
	spawnData.shotCoolTime = 0.5f;						//弾を撃つクールタイム
	spawnData.shotCoolTimeNoiseMax = 0;				//弾を撃つクールタイムのランダム値
	spawnData.spawnShotCoolTime = 2.0f;					//（shotCoolTime　+ spawnShotCoolTime = スポーンしてから初めて弾を撃つまでの時間）

	std::shared_ptr<Enemy4>enemy;

	float offset = 150;
	float explodePosX = randRange(0, 300);
	float explodeEnemyNum = randRange(12, 18);
	float explodeEnemySpeed = randRange(400, 500);
	float life = randRange(5.0f, 8.0f);
	for (int i = 0; i < 2; i++)
	{
		spawnData.pos = { SCREEN_RIGHT+spawnData.radius.x, SCREEN_TOP/2.0f - SCREEN_HEIGHT/2.0f * i };
		enemy = std::make_shared<Enemy4>(this);
		enemy->Init();
		enemy->InitOriginal(explodePosX, explodeEnemyNum, explodeEnemySpeed,life);
		enemy->AddFireworksNum(3);
		enemy->Spawn(spawnData.pos, spawnData.radius, spawnData.moveSpeed, spawnData.moveDeg, spawnData.normalColor, spawnData.hitColor, spawnData.hp, spawnData.bulletSpeed, spawnData.shotCoolTime, spawnData.shotCoolTimeNoiseMax, spawnData.spawnShotCoolTime);
		m_enemyList.push_back(enemy);
	}
}

void EnemyManager::Spawn_Reflect()
{
	//スポーンに必要な共通の変数の初期化
	SpawnData spawnData;
	spawnData.radius = { 40,40 };																						//出現する大きさ

	spawnData.moveSpeed = 250;							//移動速度

	spawnData.normalColor = { 0.6f,0.4f,0.9f,1.0f };	//通常時の色
	spawnData.hitColor = { 0.6f,0.3f,0.8f,1.0f };		//当たった時の色
	spawnData.hp = 20;									//HP
	spawnData.bulletSpeed = 0;							//弾の速度
	spawnData.shotCoolTime = 1.0f;						//弾を撃つクールタイム
	spawnData.shotCoolTimeNoiseMax = 0;				//弾を撃つクールタイムのランダム値
	spawnData.spawnShotCoolTime = 2.0f;					//（shotCoolTime　+ spawnShotCoolTime = スポーンしてから初めて弾を撃つまでの時間）

	std::shared_ptr<Enemy6>enemy;

	for (int i = 0; i < 10; i++)
	{
		spawnData.pos = { SCREEN_RIGHT + spawnData.radius.x, randRange(SCREEN_BOTTOM / 2.0f,SCREEN_TOP / 2.0f) };				//出現座標7
		spawnData.moveDeg = randRange(120, 240);							//移動する方向
		enemy = std::make_shared<Enemy6>();
		enemy->Init();
		enemy->InitOriginal(2);
		enemy->Spawn(spawnData.pos, spawnData.radius, spawnData.moveSpeed, spawnData.moveDeg, spawnData.normalColor, spawnData.hitColor, spawnData.hp, spawnData.bulletSpeed, spawnData.shotCoolTime, spawnData.shotCoolTimeNoiseMax, spawnData.spawnShotCoolTime);
		m_enemyList.push_back(enemy);
	}

	
}

void EnemyManager::Spawn_MoveLine()
{
	//スポーンに必要な共通の変数の初期化
	SpawnData spawnData;
	spawnData.radius = { 40,40 };																						//出現する大きさ
	//spawnData.pos = { SCREEN_RIGHT + spawnData.radius.x,0 };			//出現座標
	spawnData.moveSpeed = 260;							//移動速度
	spawnData.moveDeg = 180;							//移動する方向
	spawnData.normalColor = { 0.8f,0.2f,0.8f,1.0f };	//通常時の色
	spawnData.hitColor = { 0.5f,0.1f,0.7f,1.0f };		//当たった時の色
	//spawnData.hp = 20;									//HP
	spawnData.bulletSpeed = 200;						//弾の速度
	spawnData.shotCoolTime = 2.0f;						//弾を撃つクールタイム
	spawnData.shotCoolTimeNoiseMax = 0;					//弾を撃つクールタイムのランダム値
	spawnData.spawnShotCoolTime = -1.0;					//（shotCoolTime　+ spawnShotCoolTime = スポーンしてから初めて弾を撃つまでの時間）
	
	Math::Vector2 offset;
	offset.x = 100;
	offset.y = 50;

	std::vector< std::vector<Math::Vector2>> nextPosList = {
		{ {400,SCREEN_TOP		- spawnData.radius.y - 20},{400,0 + offset.y},{SCREEN_LEFT - spawnData.radius.x,0 + offset.y} },		//一つ目の動き
		{ {400,SCREEN_BOTTOM	+ spawnData.radius.y + 20},{400,0 - offset.y},{SCREEN_LEFT + spawnData.radius.x,0 - offset.y} }			//二つ目の動き
	};

	std::shared_ptr<Enemy7>enemy;


	int groupNum = nextPosList.size();
	int enemyNum = 5;

	for (int i = 0; i < groupNum; i++)
	{
		//まっすぐに進む敵をスポーンさせるためにY座標値はnextPosListの最初の座標
		spawnData.pos.y = nextPosList[i][0].y;

		for (int j = 0; j < enemyNum; j++)
		{
			enemy = std::make_shared<Enemy7>();
			enemy->Init();
			for (int k = 0; k < nextPosList[i].size(); k++)
			{
				enemy->PushNextPos(nextPosList[i][k]);
			}

			spawnData.pos.x = SCREEN_RIGHT + spawnData.radius.x + offset.x * j;
			spawnData.hp = 20 * (enemyNum - j);
			if (j == enemyNum - 1)enemy->AddFireworksNum(5);
			else enemy->AddFireworksNum(-2);
			enemy->Spawn(spawnData.pos,spawnData.radius, spawnData.moveSpeed, spawnData.moveDeg, spawnData.normalColor, spawnData.hitColor, spawnData.hp, spawnData.bulletSpeed, spawnData.shotCoolTime, spawnData.shotCoolTimeNoiseMax, spawnData.spawnShotCoolTime);
			enemy->PostInit();
			m_enemyList.push_back(enemy);
		}
	}
}

void EnemyManager::Spawn_Line_Upper_Lower()
{
	//スポーンに必要な共通の変数の初期化
	SpawnData spawnData;
	spawnData.radius = { 60,60 };								//出現する大きさ
	//spawnData.pos = { SCREEN_RIGHT + spawnData.radius.x,0 };	//出現座標
	spawnData.moveSpeed = 280;									//移動速度
	//spawnData.moveDeg = 180;									//移動する方向
	spawnData.normalColor = {1,1,1,1 };							//通常時の色
	spawnData.hitColor = { 1,0.8,0.8,1.0f };					//当たった時の色
	spawnData.hp = 120;											//HP
	spawnData.bulletSpeed = 150;								//弾の速度
	spawnData.shotCoolTime = 2.0f;								//弾を撃つクールタイム
	spawnData.shotCoolTimeNoiseMax = 0;							//弾を撃つクールタイムのランダム値
	spawnData.spawnShotCoolTime = 1.5f;							//（shotCoolTime　+ spawnShotCoolTime = スポーンしてから初めて弾を撃つまでの時間）


	float offsetX = spawnData.radius.x * 2 + 20;
	const int groupNum = 2;
	const int enemyNum = SCREEN_WIDTH / offsetX;


	int shotNum = 4;
	float shotArcRadian = DirectX::XMConvertToRadians(90);

	std::shared_ptr<Enemy2>enemy;
	for (int i = 0; i < groupNum; i++)
	{
		for (int j = 0; j < enemyNum; j++)
		{
			if (i == 0)
			{
				spawnData.pos.x = SCREEN_RIGHT + spawnData.radius.x + offsetX * j;
				spawnData.pos.y = 300;
				spawnData.moveDeg = 180;
			}
			else
			{
				spawnData.pos.x = SCREEN_LEFT - spawnData.radius.x - offsetX * j;
				spawnData.pos.y = -300;
				spawnData.moveDeg = 0;
			}
			enemy = std::make_shared<Enemy2>();
			enemy->Init();
			enemy->InitOriginal(shotNum, shotArcRadian);
			enemy->Spawn(spawnData.pos, spawnData.radius, spawnData.moveSpeed, spawnData.moveDeg, spawnData.normalColor, spawnData.hitColor, spawnData.hp, spawnData.bulletSpeed, spawnData.shotCoolTime, spawnData.shotCoolTimeNoiseMax, spawnData.spawnShotCoolTime);
			m_enemyList.push_back(enemy);
		}
	}
}

void EnemyManager::Spawn_Random1()
{
	//まっすぐ行く敵を3~6体
	//回転する敵1~4
	//弾を２発撃つ敵を3~6体配置

	//スポーンする範囲
	Math::Rectangle spawnRect = { 700,250,400,500 };

	//スポーンに必要な共通の変数の初期化
	SpawnData normalEnemy;
	normalEnemy.radius = { 32,32 };										//出現する大きさ
	//normalEnemy.pos = { SCREEN_RIGHT + normalEnemy.radius.x,0 };		//出現座標
	normalEnemy.moveSpeed = 110;										//移動速度
	normalEnemy.moveDeg = 180;											//移動する方向
	normalEnemy.normalColor = { 1.0f,0.5f,0.5f,1 };						//通常時の色
	normalEnemy.hitColor = { 1,0.5f,0.5f,1 };							//当たった時の色
	normalEnemy.hp = 20;												//HP
	normalEnemy.bulletSpeed = 200;										//弾の速度
	normalEnemy.shotCoolTime = 3.0f;									//弾を撃つクールタイム
	normalEnemy.shotCoolTimeNoiseMax = 0;								//弾を撃つクールタイムのランダム値
	normalEnemy.spawnShotCoolTime = 1.5f;								//（shotCoolTime　+ spawnShotCoolTime = スポーンしてから初めて弾を撃つまでの時間）

	//スポーンに必要な共通の変数の初期化
	SpawnData rotationEnemy;
	rotationEnemy.radius = { 30,30 };									//出現する大きさ
	//rotationEnemy.pos = { SCREEN_RIGHT + rotationEnemy.radius.x,0 };	//出現座標
	rotationEnemy.moveSpeed = 100;										//移動速度
	rotationEnemy.moveDeg = 180;										//移動する方向
	rotationEnemy.normalColor = { 1,1,1,1 };							//通常時の色
	rotationEnemy.hitColor = { 1,0.5f,0.5f,1 };							//当たった時の色
	rotationEnemy.hp = 100;												//HP
	rotationEnemy.bulletSpeed = 150;									//弾の速度
	rotationEnemy.shotCoolTime =2.0f;									//弾を撃つクールタイム
	rotationEnemy.shotCoolTimeNoiseMax = 0;								//弾を撃つクールタイムのランダム値
	rotationEnemy.spawnShotCoolTime = 1.5f;								//（shotCoolTime　+ spawnShotCoolTime = スポーンしてから初めて弾を撃つまでの時間）

	//スポーンに必要な共通の変数の初期化
	SpawnData manyBulletEnemy;
	manyBulletEnemy.radius = { 38,38 };										//出現する大きさ
	//manyBulletEnemy.pos = { SCREEN_RIGHT + manyBulletEnemy.radius.x,0 };	//出現座標
	manyBulletEnemy.moveSpeed = 100;										//移動速度
	manyBulletEnemy.moveDeg = 180;											//移動する方向
	manyBulletEnemy.normalColor = { 0.7f,0.9f,0.1f,1.0f };								//通常時の色
	manyBulletEnemy.hitColor = { 0.7f,0.7f,0.0f,1.0f };							//当たった時の色
	manyBulletEnemy.hp = 50;												//HP
	manyBulletEnemy.bulletSpeed = 150;										//弾の速度
	manyBulletEnemy.shotCoolTime = 2.0f;									//弾を撃つクールタイム
	manyBulletEnemy.shotCoolTimeNoiseMax = 0;								//弾を撃つクールタイムのランダム値
	manyBulletEnemy.spawnShotCoolTime = 1.2f;								//（shotCoolTime　+ spawnShotCoolTime = スポーンしてから初めて弾を撃つまでの時間）

	
	//出現させる数
	int r;

	//普通の敵（なんのひねりもない敵）
	r = randRange(3, 6);
	{
		std::shared_ptr<Enemy1>enemy;
		for (int i = 0; i < r; i++)
		{
			normalEnemy.pos.x = randRange(spawnRect.x, spawnRect.x + spawnRect.width);
			normalEnemy.pos.y = randRange(spawnRect.y - spawnRect.height, spawnRect.y);

			enemy = std::make_shared<Enemy1>();
			enemy->Init();
			enemy->Spawn(normalEnemy.pos, normalEnemy.radius, normalEnemy.moveSpeed, normalEnemy.moveDeg, normalEnemy.normalColor, normalEnemy.hitColor, normalEnemy.hp, normalEnemy.bulletSpeed, normalEnemy.shotCoolTime, normalEnemy.shotCoolTimeNoiseMax, normalEnemy.spawnShotCoolTime);
			m_enemyList.push_back(enemy);
		}
	}

	r = randRange(3, 6);
	{
		int shotNum = 2;
		float shotArcRadian = DirectX::XMConvertToRadians(45);

		std::shared_ptr<Enemy2>enemy;
		for (int i = 0; i < r; i++)
		{
			manyBulletEnemy.pos.x = randRange(spawnRect.x, spawnRect.x + spawnRect.width);
			manyBulletEnemy.pos.y = randRange(spawnRect.y - spawnRect.height, spawnRect.y);

			enemy = std::make_shared<Enemy2>();
			enemy->Init();
			enemy->InitOriginal(shotNum, shotArcRadian);
			enemy->Spawn(manyBulletEnemy.pos, manyBulletEnemy.radius, manyBulletEnemy.moveSpeed, manyBulletEnemy.moveDeg, manyBulletEnemy.normalColor, manyBulletEnemy.hitColor, manyBulletEnemy.hp, manyBulletEnemy.bulletSpeed, manyBulletEnemy.shotCoolTime, manyBulletEnemy.shotCoolTimeNoiseMax, manyBulletEnemy.spawnShotCoolTime);
			m_enemyList.push_back(enemy);
		}
	}

	r = randRange(1, 4);
	{
		int shotNum = randRange(1, 3);
		float deltaRadian = DirectX::XMConvertToRadians(50);
		float shotArcRadian = DirectX::XMConvertToRadians(45);
		std::shared_ptr<Enemy5>enemy;
		for (int i = 0; i < r; i++)
		{
			rotationEnemy.pos.x = randRange(spawnRect.x, spawnRect.x + spawnRect.width);
			rotationEnemy.pos.y = randRange(spawnRect.y - spawnRect.height, spawnRect.y);

			enemy = std::make_shared<Enemy5>();
			enemy->Init();
			enemy->InitOriginal(deltaRadian,shotNum, shotArcRadian);
			enemy->Spawn(rotationEnemy.pos, rotationEnemy.radius, rotationEnemy.moveSpeed, rotationEnemy.moveDeg, rotationEnemy.normalColor, rotationEnemy.hitColor, rotationEnemy.hp, rotationEnemy.bulletSpeed, rotationEnemy.shotCoolTime, rotationEnemy.shotCoolTimeNoiseMax, rotationEnemy.spawnShotCoolTime);
			m_enemyList.push_back(enemy);
		}
	}

	
}

void EnemyManager::Spawn_Random2()
{
	std::vector<int> hNumber = { 0,1,2,3,4,5,6 };
	std::vector<int> wNumber = { 0,1,2,3,4,5,6 };
	Math::Rectangle spawnRect = { (long)SCREEN_RIGHT + 100,(long)SCREEN_BOTTOM+100,(long)SCREEN_WIDTH / 2,(long)SCREEN_HEIGHT - 200 };

	Math::Vector2 offset = { (float)spawnRect.width / wNumber.size(),(float)spawnRect.height / hNumber.size() };

	//敵を３体並べてスポーンさせるか？
	if (rand() % 2)
	{
		int enemyNum = 3;
		int w = rand() % (wNumber.size() - enemyNum);
		int h = rand() % hNumber.size();

		//スポーンに必要な共通の変数の初期化
		SpawnData spawnData;
		spawnData.radius = { 32,32 };										//出現する大きさ
		//spawnData.pos = { SCREEN_RIGHT + spawnData.radius.x,0 };		//出現座標
		spawnData.moveSpeed = 120;										//移動速度
		spawnData.moveDeg = 180;											//移動する方向
		spawnData.normalColor = {0.7f,0.7f,1.0f,1.0f };						//通常時の色
		spawnData.hitColor = { 0.7f,0.6f,0.8f,1.0f };							//当たった時の色
		spawnData.hp = 20;												//HP
		spawnData.bulletSpeed = 200;										//弾の速度
		spawnData.shotCoolTime = 3.0f;									//弾を撃つクールタイム
		spawnData.shotCoolTimeNoiseMax = 0;								//弾を撃つクールタイムのランダム値
		spawnData.spawnShotCoolTime = 1.5f;								//（shotCoolTime　+ spawnShotCoolTime = スポーンしてから初めて弾を撃つまでの時間）

		std::shared_ptr<Enemy2>enemy;
		int shotNum = randRange(2, 3);
		int shotArcRadian = DirectX::XMConvertToRadians(45);
		for (int i = 0; i < enemyNum; i++)
		{
			//座標を決める
			spawnData.pos.x = { spawnRect.x + offset.x * wNumber[w] };
			spawnData.pos.y = { spawnRect.y + offset.y * hNumber[h] };

			//出現した座標は取り出す
			wNumber.erase(wNumber.begin() + w);

			enemy = std::make_shared<Enemy2>();
			enemy->Init();
			enemy->InitOriginal(shotNum, shotArcRadian);
			enemy->AddFireworksNum(i * 2);
			enemy->Spawn(spawnData.pos, spawnData.radius, spawnData.moveSpeed, spawnData.moveDeg, spawnData.normalColor, spawnData.hitColor, spawnData.hp, spawnData.bulletSpeed, spawnData.shotCoolTime, spawnData.shotCoolTimeNoiseMax, spawnData.spawnShotCoolTime);
			m_enemyList.push_back(enemy);
		}
		hNumber.erase(hNumber.begin() + h);
	}
	else
	{
		int enemyNum = 3;

		//スポーンに必要な共通の変数の初期化
		SpawnData spawnData;
		spawnData.radius = { 40,40 };										//出現する大きさ
		//spawnData.pos = { SCREEN_RIGHT + spawnData.radius.x,0 };		//出現座標
		spawnData.moveSpeed = 110;										//移動速度
		spawnData.moveDeg = 180;											//移動する方向
		spawnData.normalColor = { 1.0f,0.5f,0.5f,1 };						//通常時の色
		spawnData.hitColor = { 1,0.5f,0.5f,1 };							//当たった時の色
		spawnData.hp = 20;												//HP
		spawnData.bulletSpeed = 200;										//弾の速度
		spawnData.shotCoolTime = 2.0f;									//弾を撃つクールタイム
		spawnData.shotCoolTimeNoiseMax = 0;								//弾を撃つクールタイムのランダム値
		spawnData.spawnShotCoolTime = 1.5f;								//（shotCoolTime　+ spawnShotCoolTime = スポーンしてから初めて弾を撃つまでの時間）

		std::shared_ptr<Enemy2>enemy;
		int shotNum = randRange(2, 3);
		int shotArcRadian = DirectX::XMConvertToRadians(45);
		for (int i = 0; i < enemyNum; i++)
		{
			int w = rand() % wNumber.size();
			int h = rand() % hNumber.size();
			//座標を決める
			spawnData.pos.x = { spawnRect.x + offset.x * wNumber[w] };
			spawnData.pos.y = { spawnRect.y + offset.y * hNumber[h] };

			//出現した座標は取り出す
			hNumber.erase(hNumber.begin() + h);
			wNumber.erase(wNumber.begin() + w);

			enemy = std::make_shared<Enemy2>();
			enemy->Init();
			enemy->InitOriginal(shotNum, shotArcRadian);
			enemy->Spawn(spawnData.pos, spawnData.radius, spawnData.moveSpeed, spawnData.moveDeg, spawnData.normalColor, spawnData.hitColor, spawnData.hp, spawnData.bulletSpeed, spawnData.shotCoolTime, spawnData.shotCoolTimeNoiseMax, spawnData.spawnShotCoolTime);
			m_enemyList.push_back(enemy);
		}
	}

	//回転する敵をランダムで配置するか？
	if (rand() % 2)
	{
		int enemyNum = randRange(1, 3);
		
		//スポーンに必要な共通の変数の初期化
		SpawnData spawnData;
		spawnData.radius = { 50,50 };										//出現する大きさ
		//spawnData.pos = { SCREEN_RIGHT + spawnData.radius.x,0 };		//出現座標
		spawnData.moveSpeed = 110;										//移動速度
		spawnData.moveDeg = 180;											//移動する方向
		spawnData.normalColor = { 1.0f,0.5f,0.5f,1 };						//通常時の色
		spawnData.hitColor = { 1,0.5f,0.5f,1 };							//当たった時の色
		spawnData.hp = 50;												//HP
		spawnData.bulletSpeed = 200;										//弾の速度
		spawnData.shotCoolTime = 3.0f;									//弾を撃つクールタイム
		spawnData.shotCoolTimeNoiseMax = 0;								//弾を撃つクールタイムのランダム値
		spawnData.spawnShotCoolTime = 1.5f;								//（shotCoolTime　+ spawnShotCoolTime = スポーンしてから初めて弾を撃つまでの時間）
	
		std::shared_ptr<Enemy5>enemy;
		int shotNum = randRange(1, 2);
		int shotArcRadian = DirectX::XMConvertToRadians(45);
		float deltaRadian = DirectX::XMConvertToRadians(40);

		for (int i = 0; i < enemyNum; i++)
		{
			//出現場所の縦横の番号
			int w = rand() % wNumber.size();
			int h = rand() % hNumber.size();

			//座標を決める
			spawnData.pos.x = { spawnRect.x + offset.x * wNumber[w] };
			spawnData.pos.y = { spawnRect.y + offset.y * hNumber[h] };

			//出現した座標は取り出す
			hNumber.erase(hNumber.begin() + h);
			wNumber.erase(wNumber.begin() + w);

			enemy = std::make_shared<Enemy5>();
			enemy->Init();
			enemy->InitOriginal(deltaRadian, shotNum, shotArcRadian);
			enemy->Spawn(spawnData.pos, spawnData.radius, spawnData.moveSpeed, spawnData.moveDeg, spawnData.normalColor, spawnData.hitColor, spawnData.hp, spawnData.bulletSpeed, spawnData.shotCoolTime, spawnData.shotCoolTimeNoiseMax, spawnData.spawnShotCoolTime);
			m_enemyList.push_back(enemy);
		}
	}

}

void EnemyManager::Spawn_Row2(int enemyNum, Math::Vector2 pos,float moveSpeed,float moveDeg)
{
	std::shared_ptr<Enemy1>enemy;

	Math::Color normalColor = { 0,1,1,1 };
	Math::Color	hitColor = { 0,0.8f,0.8f,1 };

	//座標を求める
	Math::Vector2 spawnPos;
	Math::Vector2 enemyRadius = { 32,32 };
	float radian = DirectX::XMConvertToRadians(moveDeg + 180);
	spawnPos.x = pos.x + cosf(radian) * enemyRadius.x * 1.5;
	spawnPos.y = pos.y + sinf(radian) * enemyRadius.y * 1.5;

	for (int i = 0; i < enemyNum; i++)
	{
		enemy = std::make_shared<Enemy1>();
		enemy->Init();
		enemy->Spawn(spawnPos, enemyRadius, moveSpeed, moveDeg, normalColor, hitColor, 20, moveSpeed * 1.4f, 3, 1, randRange(0, 1.0f));
		m_enemyList.push_back(enemy);
	}
}

void EnemyManager::Spawn_Row3(int enemyNum, Math::Vector2 pos, float moveSpeed, float moveDeg)
{
	std::shared_ptr<Enemy1>enemy;

	Math::Color normalColor = { 0,1,1,1 };
	Math::Color	hitColor = { 0,0.8f,0.8f,1 };

	//座標を求める
	Math::Vector2 spawnPos;
	Math::Vector2 enemyRadius = { 32,32 };
	float radian = DirectX::XMConvertToRadians(moveDeg + 180);
	

	for (int i = 0; i < enemyNum; i++)
	{
		spawnPos.x = pos.x + cosf(radian) * enemyRadius.x * 1.5 * i;
		spawnPos.y = pos.y + sinf(radian) * enemyRadius.y * 1.5 * i;

		enemy = std::make_shared<Enemy1>();
		enemy->Init();
		enemy->Spawn(spawnPos, enemyRadius, moveSpeed, moveDeg, normalColor, hitColor, 20, moveSpeed * 1.4f, 3, 1, randRange(0, 1.0f));
		m_enemyList.push_back(enemy);
	}
}







