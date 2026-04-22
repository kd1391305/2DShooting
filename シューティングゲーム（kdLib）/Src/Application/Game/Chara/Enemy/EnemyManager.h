#pragma once
#include"Enemy.h"

class C_Game;		//前方宣言

//敵を管理するクラス
class C_EnemyManager
{
public:

	void Update();

	void Draw();

	std::vector<C_Enemy>& GetEnemyList() { return m_enemyList; }

	void SetGame(C_Game* set) { m_pGame = set; }

private:

	C_Game* m_pGame;

	KdTexture m_tex;

	static const int m_poolSize = 10;								//オブジェクトプール数
	std::vector<C_Enemy> m_enemyList;								//敵
	static constexpr float s_spawnProbability = 0.1f;			//敵発生確率（毎フレームあたり）
	
private:
	C_EnemyManager()
	{
		//画像の読み込み
		m_tex.Load("Texture/Enemy.png");

		//オブジェクトプール数の敵を作成しておく
		for (int i = 0; i < m_poolSize; i++)
			m_enemyList.push_back(C_Enemy());
	}
public:
	static C_EnemyManager& GetInstance()
	{
		static C_EnemyManager instance;
		return instance;
	}
};

#define ENEMY_MANAGER C_EnemyManager::GetInstance()
