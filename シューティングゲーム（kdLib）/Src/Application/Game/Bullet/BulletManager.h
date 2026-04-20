#pragma once
#include"PlayerBullet.h"
#include"EnemyBullet.h"

//弾を管理するクラス（プレイヤーとエネミーの弾、別々で管理する）
class C_BulletManager
{
public:
	//更新
	void Update();
	
	//描画
	void Draw();
	
	
	//プレイヤーの弾を撃つ
	void ShotOfPlayer(Math::Vector2 pos, Math::Vector2 move);

	//敵の弾を撃つ
	void ShotOfEnemy(Math::Vector2 pos, Math::Vector2 move);

	std::vector<C_PlayerBullet>& GetPlayerList() { return m_playerList; }
	std::vector<C_EnemyBullet>& GetEnemyList() { return m_enemyList; }

private:
	KdTexture m_playerTex;						//プレイヤーの弾画像
	std::vector<C_PlayerBullet> m_playerList;		//プレイヤーの弾
	const int m_playerPoolSize = 20;			//プレイヤーのオブジェクトプールサイズ

	KdTexture m_enemyTex;						//敵の弾画像
	std::vector<C_EnemyBullet> m_enemyList;		//敵の弾
	const int m_enemyPoolSize = 200;			//敵のオブジェクトプールサイズ
	//シングルトン
private:
	C_BulletManager();
public:
	static C_BulletManager& GetInstance()
	{
		static C_BulletManager instance;
		return instance;
	}
};

#define BULLET_MANAGER C_BulletManager::GetInstance()
