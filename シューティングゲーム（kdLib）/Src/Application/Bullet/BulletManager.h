#pragma once
#include"EnemyBullet.h"

//弾を管理するクラス（プレイヤーとエネミーの弾、別々で管理する）
class BulletManager
{
public:

	BulletManager();
	//更新
	void Update(float deltaTime);
	
	//描画
	void Draw();
	
	//敵の弾を撃つ
	void Shot(Math::Vector2 pos, Math::Vector2 move);

	std::vector<EnemyBullet>& GetEnemyList() { return m_enemyList; }

private:

	std::vector<EnemyBullet> m_enemyList;		//敵の弾
	const int m_enemyPoolSize = 200;			//敵のオブジェクトプールサイズ

};