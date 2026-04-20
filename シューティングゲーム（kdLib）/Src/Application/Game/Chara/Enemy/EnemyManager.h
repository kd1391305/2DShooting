#pragma once
#include"Enemy.h"
#include"FormerEnemy.h"

class C_Game;		//前方宣言

//敵を管理するクラス
class C_EnemyManager
{
public:

	void Update();

	void Draw();

	std::vector<C_Enemy>& GetEnemyList() { return m_enemyList; }
	std::vector<C_FormerEnemy>& GetFormerEnemyList() { return m_fEnemyList; }

	void SetGame(C_Game* set) { m_pGame = set; }

private:

	C_Game* m_pGame;

	KdTexture m_tex;

	static const int m_poolSize = 10;								//オブジェクトプール数
	std::vector<C_Enemy> m_enemyList;						//敵
	static constexpr float s_spawnProbability = 0.1f;			//敵発生確率（毎フレームあたり）
	
	//過去の敵(formerをfと略している)
	static const int m_fPoolSize = 10 * 4;
	std::vector<C_FormerEnemy> m_fEnemyList;

private:
	C_EnemyManager()
	{
		//画像の読み込み
		m_tex.Load("Texture/Enemy.png");

		//オブジェクトプール数の敵を作成しておく
		for (int i = 0; i < m_poolSize; i++)
			m_enemyList.push_back(C_Enemy());

		//オブジェクトプール数の過去の敵を作成しておく
		for (int i = 0; i < m_fPoolSize; i++)
			m_fEnemyList.push_back(C_FormerEnemy());
	}
public:
	static C_EnemyManager& GetInstance()
	{
		static C_EnemyManager instance;
		return instance;
	}
};

#define ENEMY_MANAGER C_EnemyManager::GetInstance()
