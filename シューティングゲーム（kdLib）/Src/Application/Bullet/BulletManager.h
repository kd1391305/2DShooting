#pragma once

class EnemyBullet;
class PlayerBullet;

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
	void Shoot(Math::Vector2 pos, Math::Vector2 move);
	void Add(std::shared_ptr<PlayerBullet>playerBullet);

	std::vector<std::shared_ptr<EnemyBullet>>& GetEnemyList() { return m_enemyList; }
	std::vector<std::shared_ptr<PlayerBullet>>& GetPlayerList() { return m_playerList; }

private:

	std::vector<std::shared_ptr<EnemyBullet>> m_enemyList;		//敵の弾
	std::vector<std::shared_ptr<PlayerBullet>> m_playerList;	//敵の弾
	const int m_enemyPoolSize = 200;							//敵のオブジェクトプールサイズ
};