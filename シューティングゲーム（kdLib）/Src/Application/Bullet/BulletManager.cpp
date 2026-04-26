#include"BulletManager.h"

//コンストラクタ
BulletManager::BulletManager()
{
	//画像の読み込み
	m_enemyTex.Load("Texture/Bullet.png");

	//オブジェクトプールサイズの弾を作成しておく
	for (int i = 0; i < m_enemyPoolSize; i++)
		m_enemyList.push_back(EnemyBullet(&m_enemyTex));
}

//更新
void BulletManager::Update(float deltaTime)
{
	//敵の弾
	for (auto itr = m_enemyList.begin(); itr != m_enemyList.end();)
	{
		if (itr->IsActive())itr->Update(deltaTime);
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
void BulletManager::Draw()
{
	//敵の弾
	for (auto& b : m_enemyList)
	{
		if (b.IsActive())
		{
			b.Draw();
		}
	}
}

void BulletManager::Shot(Math::Vector2 pos, Math::Vector2 move)
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
	//オブジェクト作成（このオブジェクトは非活性状態になったらこのクラス（BulletManager）のUpdate()で削除される）
	m_enemyList.push_back(EnemyBullet(&m_enemyTex));
	m_enemyList.back().Shot(pos, move);
}


