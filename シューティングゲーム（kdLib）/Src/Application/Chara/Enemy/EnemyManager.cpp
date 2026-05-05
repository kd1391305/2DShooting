#include"EnemyManager.h"
#include"../../main.h"
#include"../../Tools/RandEx/RandEx.h"
#include"../../Timer/Timer.h"
#include"../../Scene/GameScene/GameScene.h"
#include"Enemies/Enemy1.h"
#include"Enemies/Enemy2.h"
#include"Enemies/Enemy3.h"
#include"Boss/Boss.h"
#include"../Player/Player.h"

//ƒRƒ“ƒXƒgƒ‰ƒNƒ^
EnemyManager::EnemyManager()
{
	//4•bŠÔ‚Í“G‚ğƒXƒ|[ƒ“‚µ‚È‚¢
	m_spawnWaitTimer = 4.0f;

	//“G‚ÌƒXƒ|[ƒ“Šm—¦
	m_spawnProbability[SpawnPutturn::Circle]	= 0.3f;
	m_spawnProbability[SpawnPutturn::Row]		= 0.3f;
	m_spawnProbability[SpawnPutturn::Square]	= 0.4f;
}

//XV
void EnemyManager::Update(float deltaTime)
{
	//ƒXƒ|[ƒ“ƒ^ƒCƒ}[‚ği‚ß‚é
	m_spawnWaitTimer -= deltaTime;
	if (m_spawnWaitTimer < 0)
	{
		m_spawnWaitTimer = 0;
		m_bEmptySpawnFlg = true;
	}
	if (m_bEmptySpawnFlg)
	{
		if (m_enemyList.empty())m_spawnWaitTimer = 0;
	}
	if (m_spawnWaitTimer <= 0)
	{
		//“G‚ğƒXƒ|[ƒ“‚·‚é
		Spawn(deltaTime);

		float noise = randRange(0.0f, 5.0f);
		m_spawnWaitTimer = m_spawnWait + noise;
	}

	for (auto itr = m_enemyList.begin(); itr != m_enemyList.end();)
	{
		//Šˆ«ó‘Ô‚Ì“G‚ÍXVˆ—
		if ((*itr)->IsActive())
		{
			(*itr)->Update(deltaTime);
		}
		//”ñŠˆ«ó‘Ô‚Ì“G‚Í
		else
		{
			//ƒf[ƒ^‚ğíœ‚·‚é
			itr = m_enemyList.erase(itr);
			continue;
		}
		itr++;
	}


	if (Timer::Instance().GetTime() > 60)
	{
		if (!m_boss)
		{
			m_boss = std::make_shared<Boss>(m_pGame);
			m_boss->Init();
			m_boss->Spawn({ SCREEN_RIGHT + m_boss->GetRadius().x,0 }, { -200, 0 });
		}

	}


	//ƒ{ƒX‚ÌXV
	if (m_boss)
	{
		m_boss->Update(deltaTime);
	}
}

//•`‰æ
void EnemyManager::Draw()
{
	for (auto& b : m_enemyList)if (b->IsActive())b->Draw();
	if (m_boss)m_boss->Draw();
}


//ƒXƒ|[ƒ“‚·‚é
void EnemyManager::Spawn(float deltaTime)
{
	//ƒXƒ|[ƒ“‚·‚éƒ^ƒCƒv‚ğŒˆ‚ß‚é
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

	//“G‚ğƒXƒ|[ƒ“‚·‚é
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
	//“G‚ª‰æ–Ê‰E’[‚©‚ç‰~‚ğ•`‚­‚æ‚¤‚É¶‚ÉŒü‚©‚Á‚Ä“®‚­B
		//‚ ‚éˆê’è‚ÌêŠ‚Å“G‚Í~‚Ü‚é
		//“G‚ğƒXƒ|[ƒ“‚³‚¹‚é”
	int spawnNum = 6;

	Math::Vector2 startPos = { SCREEN_RIGHT + 200,0 };
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

		//V‚µ‚­“G‚ğì¬‚·‚é
		m_enemyList.push_back(temp);
		//“G‚ÌƒXƒ|[ƒ“
		m_enemyList.back()->Spawn();
	}

	m_spawnPutturnHistory.push(SpawnPutturn::Circle);
}

void EnemyManager::Spawn_Row()
{
	//“G‚ª—ñó‚É‚È‚Á‚Ä“G‚ªoŒ»
		//			ZZZZZ
		//		ZZZZZ
		//	ZZZZZ
		//		ZZZZZ
		//			ZZZZZ

	const int groupNum = 5;
	const int enemyNum = 5;

	std::shared_ptr<Enemy1>enemy[groupNum][enemyNum];
	for (int i = 0; i < groupNum; i++)
	{
		for (int j = 0; j < enemyNum; j++)
		{
			enemy[i][j] = std::make_shared<Enemy1>();
			enemy[i][j]->Init();
		}
	}

	//ŠeƒOƒ‹[ƒv‚²‚Æ‚ÌÀ•W‚ğ‹‚ß‚é
	float baseX = SCREEN_WIDTH + enemy[0][0]->GetRadius().x;
	enemy[0][0]->SetPos({ baseX,0 });
	enemy[1][0]->SetPos({ baseX + 100,100 });
	enemy[2][0]->SetPos({ baseX + 100 ,-100 });
	enemy[3][0]->SetPos({ baseX + 200,200 });
	enemy[4][0]->SetPos({ baseX + 200,-200 });

	//ƒOƒ‹[ƒv‚Ìæ“ªÀ•W‚ğ‚à‚Æ‚ÉAŒã‘±‚Ì“G‚ÌÀ•W‚ğƒZƒbƒg
	float gapX = enemy[0][0]->GetRadius().x * 2;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 1; j < 5; j++)
		{
			enemy[i][j]->SetPos({ enemy[i][0]->GetPos().x + gapX * j, enemy[i][0]->GetPos().y });
		}
	}

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			enemy[i][j]->Spawn();
			m_enemyList.push_back(enemy[i][j]);
		}
	}

	m_spawnPutturnHistory.push(SpawnPutturn::Row);
}

void EnemyManager::Spawn_Square()
{
	//‚©‚½‚¢“G‚ğoŒ»‚³‚¹‚é
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			std::shared_ptr<Enemy3> enemy;
			enemy = std::make_shared<Enemy3>();
			enemy->Init();
			enemy->SetPos(Math::Vector2{ SCREEN_RIGHT + (200.0f * i + 1),0 + 150.0f * (j - 2) });
			enemy->SetMove(Math::Vector2{ -100,0 });
			enemy->Spawn();
			m_enemyList.push_back(enemy);
		}
	}
}


