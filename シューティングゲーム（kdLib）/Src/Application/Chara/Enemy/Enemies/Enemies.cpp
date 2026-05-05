#include"Enemies.h"
#include"../../../Tools/RandEx/RandEx.h"
#include"../../../Bullet/BulletManager.h"

//敵の行動パターン通りに動く

void Enemy1::Action(float deltaTime)
{
	if (m_shotCoolTimer <= 0)
	{
		//X方向にまっすぐ発射する
		Math::Vector2 shotPos;
		shotPos.x = m_pos.x - m_radius.x;
		shotPos.y = 0;
		s_pBulletManager->Shoot(shotPos, { m_bulletSpeed,0 });

		//クールタイム
		m_shotCoolTimer = m_shotCoolTime + randRange(0, m_shotCoolTimeNoiseMax);
	}
	
}

void Enemy2::Action(float deltaTime)
{
	//プレイヤー方向に弾を複数（m_shotNum分）発射
	if (m_shotCoolTimer <= 0)
	{
		if (m_shotNum <= 0)return;
		
		float centerRadian, startRadian, offsetRadian;
		Math::Vector2 vec, shotPos, shotMove;

		//弾を撃つ中心角、最小角度、弾ごとの角度の差
		centerRadian = atan2f(s_pPlayerPos->y - m_pos.y, s_pPlayerPos->x - m_pos.x);
		startRadian = centerRadian - m_shotArcRadian / 2.0f;
		offsetRadian = m_shotArcRadian / m_shotNum;

		//弾の撃つ場所を求める
		vec.x = cosf(centerRadian);
		vec.y = sinf(centerRadian);
		shotPos = m_pos + vec * m_radius;

		for (int i = 0; i < m_shotNum; i++)
		{
			//弾のベクトル
			vec.x = cosf(startRadian + offsetRadian * i);
			vec.y = sinf(startRadian + offsetRadian * i);
			shotMove = vec * m_bulletSpeed;
			
			//弾を発射する
			s_pBulletManager->Shoot(shotPos, shotMove);
		}

		//クールタイム
		m_shotCoolTimer = m_shotCoolTime + randRange(0, m_shotCoolTimeNoiseMax);
	}
}

void Enemy2::InitOriginal(float shotNum, float shotArcRadian)
{
	m_shotNum = shotNum;
	m_shotArcRadian = DirectX::XMConvertToRadians(shotArcRadian);
}
