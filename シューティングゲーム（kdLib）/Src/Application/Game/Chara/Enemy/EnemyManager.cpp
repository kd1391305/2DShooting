#include"EnemyManager.h"
#include"../../../main.h"
#include"../../../Toolkit/RandEx.h"
#include"../../../Save/Save.h"

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
				float y = rand_Range(
					SCREEN_BOTTOM + itr->GetRadius().y,			//最小値
					SCREEN_TOP			- itr->GetRadius().y);		//最大値
				itr->Spawn({ x, y}, { -5,0 });
				spawnFlg = false;
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
		float radius = m_enemyList[m_enemyList.size() - 1].GetRadius().x;
		float x = SCREEN_RIGHT + radius;
		float y = rand_Range(
			SCREEN_BOTTOM + radius,			//最小値
			SCREEN_TOP - radius);		//最大値
		m_enemyList[m_enemyList.size() - 1].Spawn({ x, y }, { -5,0 });
	}

	//過去の敵の更新処理
	for (auto e : m_fEnemyList)
	{
		while(SAVE.)


		//活性状態の敵は更新処理
		if (e.IsActive())e.Update();
	}
}

//描画
void C_EnemyManager::Draw()
{
	for (auto& b : m_enemyList)if (b.IsActive())b.Draw(&m_tex);
	for (auto& b : m_fEnemyList)if (b.IsActive())b.Draw(&m_tex);
}