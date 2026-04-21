#include"EnemyManager.h"
#include"../../../main.h"
#include"../../../Toolkit/RandEx.h"
#include"../../../Save/Save.h"
#include"../../GameTimer.h"
#include"../../Game.h"

//更新
void C_EnemyManager::Update()
{
	//敵をスポーンさせるかどうかを決める
	bool spawnFlg = HitGacha(s_spawnProbability);
	
	for (auto itr = m_enemyList.begin(); itr != m_enemyList.end();)
	{
		//活性状態の敵は更新処理
		if (itr->IsActive())itr->Update();
		//非活性状態の敵は
		else
		{
			if (spawnFlg)
			{
				//敵をスポーンさせる
				float x = SCREEN_RIGHT + itr->GetRadius().x;
				float y = randRange(
					SCREEN_BOTTOM + itr->GetRadius().y,			//最小値
					SCREEN_TOP			- itr->GetRadius().y);		//最大値
				itr->Spawn({ x, y}, { -5,0 });
				spawnFlg = false;

				//スポーンした情報を保存する
				SAVE.WriteEnemy({ x,y }, GAME_TIMER.GetTime());
			}
			else
			{
				//オブジェクトプール数を超えていたらデータを削除する
				if (m_enemyList.size() > m_poolSize)
				{
					itr = m_enemyList.erase(itr);
					continue;
				}
			}
		}
		itr++;
	}

	//オブジェクトプールの中に非活性状態の敵がいなかったら
	if (spawnFlg)
	{
		//新しく敵を作成する
		m_enemyList.push_back(C_Enemy());
		float radius = m_enemyList.back().GetRadius().x;
		float x = SCREEN_RIGHT + radius;
		float y = randRange(
			SCREEN_BOTTOM + radius,			//最小値
			SCREEN_TOP - radius);		//最大値
		m_enemyList.back().Spawn({x, y}, {-5,0});

		//スポーンした情報を保存する
		SAVE.WriteEnemy({ x,y },GAME_TIMER.GetTime());
	}

	//過去の敵の更新処理
	for (auto& e : m_fEnemyList)
		//活性状態の敵は更新処理
		if (e.IsActive())e.Update();

	//スポーンする敵がいなくなるまでループを回す
	bool hit = false;
	while (SAVE.SearchSpawnEnemy(GAME_TIMER.GetTime(), m_pGame->GetLevel()))
	{
		for (auto& e : m_fEnemyList)
			if (!e.IsActive())
			{
				e.Spawn(SAVE.PopSpawnEnemy(m_pGame->GetLevel()), Math::Vector2{ -5,0 });
				hit = true;
				break;
			}
		//非活性状態の敵が見つからなかったら、敵を追加する
		if (!hit)
		{
			m_fEnemyList.push_back(C_FormerEnemy());
			m_fEnemyList.back().Spawn(SAVE.PopSpawnEnemy(m_pGame->GetLevel()), Math::Vector2{-5,0});
			hit = true;
		}
		
		hit = false;
	}
}

//描画
void C_EnemyManager::Draw()
{
	for (auto& b : m_enemyList)if (b.IsActive())b.Draw(&m_tex);
	for (auto& b : m_fEnemyList)if (b.IsActive())b.Draw(&m_tex);
}