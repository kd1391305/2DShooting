#include"BulletManager.h"
#include"../Bullet/PlayerBullet/PlayerBullet.h"
#include"../Bullet/EnemyBullet/EnemyBullet.h"
#include"../TextureCache/TextureCache.h"

//コンストラクタ
BulletManager::BulletManager()
{

	//オブジェクトプールサイズの弾を作成しておく
	for (int i = 0; i < m_enemyPoolSize; i++)
		m_enemyList.push_back(std::make_shared<EnemyBullet>());
}

//更新
void BulletManager::Update(float deltaTime)
{
	//プレイヤーの弾
	for (auto itr = m_playerList.begin();itr!= m_playerList.end();)
	{
		(*itr)->Update(deltaTime);
		if (!(*itr)->IsActive())
		{
			itr = m_playerList.erase(itr);
			continue;
		}
		itr++;
	}

	//敵の弾
	for (auto itr = m_enemyList.begin(); itr != m_enemyList.end();)
	{
		if ((*itr)->IsActive())(*itr)->Update(deltaTime);
		else
		{
			//オブジェクトプール数を超えていなかったら非活性状態にする
			if (m_enemyList.size() <= m_enemyPoolSize)(*itr)->SetActive(false);
			//オブジェクトプール数を超えていたらデータを削除する
			else
			{
				itr = m_enemyList.erase(itr);
				continue;
			}
		}
		itr++;
	}
}

//描画
void BulletManager::Draw()
{
	//プレイヤーの弾
	for (auto& p : m_playerList)
	{
		if (p->IsActive())
		{
			p->Draw();
		}
	}

	//敵の弾
	for (auto& e : m_enemyList)
	{
		if (e->IsActive())
		{
			e->Draw();
		}
	}
}

void BulletManager::Shoot(Math::Vector2 pos, Math::Vector2 move)
{
	//非活性状態の弾を探す
	for (auto& e : m_enemyList)
	{
		if (!e->IsActive())
		{
			e->Shoot(pos, move);
			//発射できたので処理を終了する
			return;
		}
	}
	//もし見つからなかったら
	//オブジェクト作成（このオブジェクトは非活性状態になったらこのクラス（BulletManager）のUpdate()で削除される）
	m_enemyList.push_back(std::make_shared<EnemyBullet>());
	m_enemyList.back()->Shoot(pos, move);
}

void BulletManager::Add(std::shared_ptr<PlayerBullet> playerBullet)
{
	m_playerList.push_back(playerBullet);
}


