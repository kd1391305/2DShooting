#include"BulletManager.h"

//更新
void C_BulletManager::Update()
{
	for (auto itr = m_playerList.begin(); itr != m_playerList.end();)
	{
		if (itr->IsActive())itr->Update();
		else
		{
			//オブジェクトプール数を超えていなかったら非活性状態にする
			if (m_playerList.size() <= m_playerPoolSize)itr->SetActive(false);
			//オブジェクトプール数を超えていたらデータを削除する
			else
			{
				itr = m_playerList.erase(itr);
				continue;
			}
		}
		itr++;
	}

	for (auto itr = m_enemyList.begin(); itr != m_enemyList.end();)
	{
		if (itr->IsActive())itr->Update();
		else
		{
			//オブジェクトプール数を超えていなかったら非活性状態にする
			if (m_enemyList.size() <= m_enemyPoolSize)itr->SetActive(false);
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
void C_BulletManager::Draw()
{
	//プレイヤーの弾
	for (auto& b : m_playerList)if (b.IsActive())b.Draw();
	//敵の弾
	for (auto& b : m_enemyList)if (b.IsActive())b.Draw();
}

//プレイヤーの弾を撃つ
void C_BulletManager::ShotOfPlayer(Math::Vector2 pos, Math::Vector2 move)
{
	//非活性状態の弾を探す
	for (auto& b : m_playerList)
	{
		if (!b.IsActive())
		{
			b.Shot(pos, move);
			//発射できたので処理を終了する
			return;
		}
	}
	//もし見つからなかったら
	//オブジェクト作成（このオブジェクトは非活性状態になったらこのクラス（C_BulletManager）のUpdate()で削除される）
	m_playerList.push_back(C_PlayerBullet(&m_playerTex));
	m_playerList[m_playerList.size() - 1].Shot(pos, move);
}

void C_BulletManager::ShotOfEnemy(Math::Vector2 pos, Math::Vector2 move)
{
	//非活性状態の弾を探す
	for (auto& b : m_enemyList)
	{
		if (!b.IsActive())
		{
			b.Shot(pos, move);
			//発射できたので処理を終了する
			return;
		}
	}
	//もし見つからなかったら
	//オブジェクト作成（このオブジェクトは非活性状態になったらこのクラス（C_BulletManager）のUpdate()で削除される）
	m_enemyList.push_back(C_EnemyBullet(&m_enemyTex));
	m_enemyList[m_enemyList.size() - 1].Shot(pos, move);
}

C_BulletManager::C_BulletManager()
{
	//画像の読み込み
	m_playerTex.Load("Texture/Bullet3.png");
	m_enemyTex.Load("Texture/Bullet.png");

	//オブジェクトプールサイズの弾を作成しておく
	for (int i = 0; i < m_playerPoolSize; i++)
		m_playerList.push_back(C_PlayerBullet(&m_playerTex));

	for (int i = 0; i < m_enemyPoolSize; i++)
		m_enemyList.push_back(C_EnemyBullet(&m_enemyTex));
}
