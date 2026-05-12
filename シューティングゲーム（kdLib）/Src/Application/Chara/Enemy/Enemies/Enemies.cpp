#include"Enemies.h"
#include"../../../Tools/RandEx/RandEx.h"
#include"../../../Bullet/BulletManager.h"
#include"../EnemyManager.h"
#include"../../../main.h"
#include"../Boss/AttackArea/AttackArea.h"
#include"../../../Fireworks/FireworksManager.h"

//敵の行動パターン通りに動く

void Enemy1::Action(float deltaTime)
{
	if (m_shotCoolTimer <= 0)
	{
		//X方向にまっすぐ発射する
		Math::Vector2 shotPos;
		shotPos.x = m_pos.x - m_radius.x;
		shotPos.y = m_pos.y;
		s_pBulletManager->Shoot(shotPos, { -m_bulletSpeed,0 });

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
		if (m_shotNum > 1)	offsetRadian = m_shotArcRadian / (float)(m_shotNum - 1);
		else				offsetRadian = 0;
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
	m_shotArcRadian =shotArcRadian;
}

void Enemy3::Action(float deltaTime)
{
	//弾ける地点を超えたら、弾ける
	if (m_pos.x <= m_explodePosX)
	{
		m_pos.x = m_explodePosX;

		//敵を出現させる

		//スポーンに必要な共通の変数の初期化
		EnemyManager::SpawnData spawnData;
		spawnData.pos = m_pos;						//出現座標
		spawnData.radius = { 25,25 };				//出現する大きさ
		spawnData.moveSpeed = m_explodeEnemySpeed;	//移動速度
		//spawnData.moveDeg = 180;					//移動する方向
		spawnData.normalColor = m_normalColor;		//通常時の色
		spawnData.hitColor = m_hitColor;			//当たった時の色
		spawnData.hp = 999;							//HP
		spawnData.bulletSpeed = 0;					//弾の速度
		spawnData.shotCoolTime =999;				//弾を撃つクールタイム
		spawnData.shotCoolTimeNoiseMax = 0.0f;		//弾を撃つクールタイムのランダム値
		spawnData.spawnShotCoolTime = 999;			//（shotCoolTime　+ spawnShotCoolTime = スポーンしてから初めて弾を撃つまでの時間）

		//角度
		float startDeg, offsetDeg;
		startDeg = DirectX::XMConvertToDegrees(atan2f(s_pPlayerPos->y - m_pos.y, s_pPlayerPos->x - m_pos.x));
		offsetDeg = 360.0f / m_explodeEnemyNum;	//間隔

		std::shared_ptr<Enemy1>enemy;
		for (int i = 0; i < m_explodeEnemyNum; i++)
		{
			spawnData.moveDeg = startDeg + offsetDeg * i;
			enemy = std::make_shared<Enemy1>();
			enemy->Init();
			enemy->Spawn(spawnData.pos, spawnData.radius, spawnData.moveSpeed, spawnData.moveDeg, spawnData.normalColor, spawnData.hitColor, spawnData.hp, spawnData.bulletSpeed, spawnData.shotCoolTime, spawnData.shotCoolTimeNoiseMax, spawnData.spawnShotCoolTime);
			m_pEnemyManager->GetEnemyList().push_back(enemy);
		}

		//弾けたらこの敵は非活性状態に
		m_bActive = false;
	}
}

void Enemy3::InitOriginal(float explodePosX, int explodeEnemyNum,float explodeEnemySpeed)
{
	m_explodePosX = explodePosX;
	m_explodeEnemyNum = explodeEnemyNum;
	m_explodeEnemySpeed = explodeEnemySpeed;
}

void Enemy4::Action(float deltaTime)
{
	m_life -= deltaTime;
	if (m_life <= 0)
	{
		m_bActive = false;
		return;
	}

	if (m_pos.x <= m_explodePosX)
	{
		m_pos.x = m_explodePosX;
		m_move = { 0,0 };
		m_bCanShoot = true;
	}

	//弾ける地点を超えたら、弾ける
	if (m_shotCoolTimer <= 0)
	{
		//敵を出現させる

		//スポーンに必要な共通の変数の初期化
		EnemyManager::SpawnData spawnData;
		spawnData.pos = m_pos;						//出現座標
		spawnData.radius = { 25,25 };				//出現する大きさ
		spawnData.moveSpeed = m_explodeEnemySpeed;	//移動速度
		//spawnData.moveDeg = 180;					//移動する方向
		spawnData.normalColor = m_normalColor;		//通常時の色
		spawnData.hitColor = m_hitColor;			//当たった時の色
		spawnData.hp = 999;							//HP
		spawnData.bulletSpeed = 0;					//弾の速度
		spawnData.shotCoolTime = 999;				//弾を撃つクールタイム
		spawnData.shotCoolTimeNoiseMax = 0.0f;		//弾を撃つクールタイムのランダム値
		spawnData.spawnShotCoolTime = 999;			//（shotCoolTime　+ spawnShotCoolTime = スポーンしてから初めて弾を撃つまでの時間）

		//角度
		float startDeg, offsetDeg;
		startDeg = DirectX::XMConvertToDegrees(atan2f(s_pPlayerPos->y - m_pos.y, s_pPlayerPos->x - m_pos.x));
		offsetDeg = 360.0f / m_explodeEnemyNum;	//間隔

		std::shared_ptr<Enemy1>enemy;
		for (int i = 0; i < m_explodeEnemyNum; i++)
		{
			spawnData.moveDeg = startDeg + offsetDeg * i;
			enemy = std::make_shared<Enemy1>();
			enemy->Init();
			enemy->Spawn(spawnData.pos, spawnData.radius, spawnData.moveSpeed, spawnData.moveDeg, spawnData.normalColor, spawnData.hitColor, spawnData.hp, spawnData.bulletSpeed, spawnData.shotCoolTime, spawnData.shotCoolTimeNoiseMax, spawnData.spawnShotCoolTime);
			m_pEnemyManager->GetEnemyList().push_back(enemy);
		}

		//クールタイム
		m_shotCoolTimer = m_shotCoolTime;
	}
}

void Enemy4::InitOriginal(float explodePosX, int explodeEnemyNum, float explodeEnemySpeed, float life,const bool bCanShoot)
{
	m_explodePosX = explodePosX;
	m_explodeEnemyNum = explodeEnemyNum;
	m_explodeEnemySpeed = explodeEnemySpeed;
	m_life = life;
	m_bCanShoot = bCanShoot;
}

void Enemy5::Action(float deltaTime)
{
	m_radian += m_deltaRadian * deltaTime;

	//プレイヤー方向に弾を複数（m_shotNum分）発射
	if (m_shotCoolTimer <= 0)
	{
		if (m_shotNum <= 0)return;

		float centerRadian, startRadian, offsetRadian;
		Math::Vector2 vec, shotPos, shotMove;

		//弾を撃つ中心角、最小角度、弾ごとの角度の差
		centerRadian = m_radian;
		startRadian = centerRadian - m_shotArcRadian / 2.0f;
		if (m_shotNum > 1)	offsetRadian = m_shotArcRadian / (float)(m_shotNum - 1);
		else				offsetRadian = 0;

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


void Enemy5::InitOriginal(float deltaRadian, float shotNum, float shotArcRadian)
{
	m_deltaRadian = deltaRadian;
	m_shotNum = shotNum;
	m_shotArcRadian = shotArcRadian;
}

void Enemy6::Action(float deltaTime)
{
	if (m_reflectNum > 0)
	{
		bool bReflect = false;

		//現在の座標
		float left = m_pos.x - m_radius.x;
		float right = m_pos.x + m_radius.x;
		float top = m_pos.y + m_radius.y;
		float bottom = m_pos.y - m_radius.y;

		//未来座標で画面外に出たかを判定
		float nextLeft = left + m_move.x *deltaTime;
		float nextRight = right + m_move.x * deltaTime;
		float nextTop = top+ m_move.y * deltaTime;
		float nextBottom = bottom + m_move.y * deltaTime;

		if (left > SCREEN_LEFT)
		{
			if (nextLeft < SCREEN_LEFT)		
			{
				m_move.x *= -1;
				m_pos.x = SCREEN_LEFT + m_radius.x - m_move.x * deltaTime;
				bReflect = true;
			}
		}
		if (right < SCREEN_RIGHT)
		{
			if (nextRight > SCREEN_RIGHT)
			{
				m_move.x *= -1;
				m_pos.x = SCREEN_RIGHT - m_radius.x - m_move.x * deltaTime;
				bReflect = true;
			}
		}
		if (top < SCREEN_TOP)
		{
			if (nextTop > SCREEN_TOP)
			{
				m_move.y *= -1;
				m_pos.y = SCREEN_TOP - m_radius.y - m_move.y * deltaTime;
				bReflect = true;
			}
		}
		if (bottom > SCREEN_BOTTOM)
		{
			if (nextBottom < SCREEN_BOTTOM)
			{
				m_move.y *= -1;
				m_pos.y = SCREEN_BOTTOM + m_radius.y - m_move.y * deltaTime;
				bReflect = true;
			}
		}
		if (bReflect)
		{
			m_reflectNum--;
		}
	}
}

void Enemy6::InitOriginal(int reflectNum)
{
	m_reflectNum = reflectNum;
}

void Enemy7::Action(float deltaTime)
{
	//弾を発射
	if (m_shotCoolTimer <= 0)
	{
		Math::Vector2 vec = *s_pPlayerPos - m_pos;
		vec.Normalize();
		s_pBulletManager->Shoot(Math::Vector2{ m_pos + vec * m_radius }, Math::Vector2{ vec * m_bulletSpeed });

		//クールタイム
		m_shotCoolTimer = m_shotCoolTime;
	}

	if (m_nextPos.empty())return;

	//指定した座標に近づいたら
	if (Math::Vector2(m_nextPos.front() - m_pos).Length() < m_moveSpeed * deltaTime)
	{
		m_nextPos.pop();
		if (m_nextPos.empty())return;

		//次の座標へ移動する
		Math::Vector2 vec = m_nextPos.front() - m_pos;
		vec.Normalize();
		m_move = vec * m_moveSpeed;
	}
}

void Enemy7::PushNextPos(Math::Vector2& pos)
{
	m_nextPos.push(pos);
}

void Enemy7::PostInit()
{
	if (m_nextPos.empty())return;
	
	//行き先を決定する
	Math::Vector2 vec = m_nextPos.front() - m_pos;
	vec.Normalize();
	m_move = vec * m_moveSpeed;
}

void Enemy9::Action(float deltaTime)
{
	if (m_explodeCnt > 0)
	{
		//プレイヤー方向に敵を複数（m_shotNum分）発射
		if (m_shotCoolTimer <= 0)
		{
			if (m_shotNum <= 0)return;

			float centerRadian, startRadian, offsetRadian;

			//敵を撃つ中心角、最小角度、弾ごとの角度の差
			centerRadian = atan2f(s_pPlayerPos->y - m_pos.y, s_pPlayerPos->x - m_pos.x);
			startRadian = centerRadian - m_shotArcRadian / 2.0f;
			if (m_shotNum > 1)	offsetRadian = m_shotArcRadian / (float)(m_shotNum - 1);
			else				offsetRadian = 0;

			float radian;

			std::shared_ptr<Enemy9>enemy;
			for (int i = 0; i < m_shotNum; i++)
			{
				radian = startRadian + offsetRadian * i;

				//敵を発射する
				enemy = std::make_shared<Enemy9>(m_pEnemyManager);
				enemy->Init();
				enemy->InitOriginal(m_explodeCnt - 1, m_increaseShotNum, m_shotNum + m_increaseShotNum, m_shotArcRadian);
				enemy->Spawn(m_pos, m_radius, m_moveSpeed * 1.1f, DirectX::XMConvertToDegrees(radian), m_normalColor, m_hitColor, m_hp, m_bulletSpeed, m_shotCoolTime);
				m_pEnemyManager->GetEnemyList().push_back(enemy);
			}

			m_bActive = false;
		}
	}
}

void Enemy9::InitOriginal(int explodeCnt, int increaseShotNum, int shotNum, float shotArcRadian)
{
	m_explodeCnt = explodeCnt;
	m_increaseShotNum = increaseShotNum;
	m_shotNum = shotNum;
	m_shotArcRadian = shotArcRadian;
}

void Enemy10::Action(float deltaTime)
{
	m_timer = 0;
	
	if (m_move.y < -400)m_move.y = -400;
	
	if (m_pos.y - m_radius.y >= SCREEN_TOP)
	{
		if (m_pos.x < m_pAttackArea->GetPos().x)
		{
			m_pos.x = m_pAttackArea->GetPos().x;
			m_move.x = 0;
			m_move.y = -10;
		}
		m_pos.y = SCREEN_TOP + m_radius.y;
	}
	
	//重力にしたがって落ちていく
	if (m_move.y <= 0)
	{
		m_move.y -= 300 * deltaTime;
		if (m_pAttackArea)
		{
			if (m_pos.y + m_move.y * deltaTime < m_pAttackArea->GetPos().y)
			{
				m_pAttackArea->SetHitFlg(true, 0.5f);	//フラグと当たり判定の時間
				m_bActive = false;

				//花火を発生させる
				Math::Vector2 pos;
				Math::Color color;
				float scale;
				float seVolume = 0.05f;
				color = { randRange(0,1),randRange(0,1),randRange(0,1),randRange(0.7,1.0f) };
				for (int i = 0; i < m_fireworksNum; i++)
				{
					pos = m_pos;
					pos += {randRange(-50, 50), randRange(-50, 50)};
					scale = randRange(0.3f, 0.6f);
					m_pFireworksManager->Explode(FireworksManager::GetRandomType_Quick(), pos, scale, color, seVolume);
				}
			}
		}
	}
}