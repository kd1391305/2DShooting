#include "Boss.h"
#include"../../../TextureCache/TextureCache.h"
#include"../../../Bullet/BulletManager.h"
#include"../../../Scene/GameScene/GameScene.h"
#include"../../Player/Player.h"
#include"../../../UI/UI.h"

Boss::Boss(Game*game):
	m_pGame(game)
{
}

void Boss::InitOriginal()
{
	m_shotCnt = 0;

	m_circleShotCoolTimer = 7.0f;
	m_circleShotCnt = 0;
}

//スポーンするオーバライド関数は敵の大きさのm_scaleを求める処理のみ変更
void Boss::Spawn(Math::Vector2& pos, Math::Vector2& radius, float moveSpeed, float moveDeg, Math::Color& normalColor, Math::Color& hitColor, float hp, float bulletSpeed, float shotCoolTime, const float shotCoolTimeNoiseMax, const float spawnShotCoolTime)
{
	//座標
	m_pos = pos;

	//移動量
	m_moveSpeed = moveSpeed;
	{
		float radian = DirectX::XMConvertToRadians(moveDeg);
		m_move.x = cosf(radian) * moveSpeed;
		m_move.y = sinf(radian) * moveSpeed;
	}

	//敵の大きさ
	m_radius = radius;
	{
		KdTexture* tex = TextureCache::Instance().Get("Texture/Enemy/Boss/Boss0.png").get();
		float texRadiusX = tex->GetRadius().x;
		float texRadiusY = tex->GetRadius().y;
		m_scale.x = radius.x / texRadiusX;
		m_scale.y = radius.y / texRadiusY;
	}

	//敵の向き
	if (m_move.x > 0)m_scale.x *= -1;

	//行列作成
	Math::Matrix scaleMat, rotationMat, transMat;
	scaleMat = Math::Matrix::CreateScale(m_scale.x, m_scale.y, 0);
	rotationMat = Math::Matrix::CreateRotationZ(m_radian);
	transMat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scaleMat * rotationMat * transMat;

	//色
	m_color = normalColor;
	m_normalColor = normalColor;
	m_hitColor = hitColor;

	//HP
	m_hpMax = hp;
	m_hp = hp;

	//弾
	m_bulletSpeed = bulletSpeed;
	m_shotCoolTime = shotCoolTime;
	m_shotCoolTimeNoiseMax = shotCoolTimeNoiseMax;				//クールタイムのずれる時間
	m_shotCoolTimer = shotCoolTime + spawnShotCoolTime;			//スポーン時の弾発射のクールタイム
	m_pGame->GetUI()->CreateBossGauge(&m_hpMax, &m_hp);
}

void Boss::Draw()
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	char path[50];
	sprintf_s(path, sizeof(path), "Texture/Enemy/Boss/Boss%d.png", (int)m_animCnt);
	SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get(path), m_color);
}

void Boss::Action(float deltaTime)
{
	//ボスの座標補正
	if (m_pos.x < m_endPosX)
	{
		m_move.x = 0;
		m_pos.x = m_endPosX;
	}

	m_circleShotCoolTimer -= deltaTime;

	float hpRatio = m_hp / m_hpMax;
	if (hpRatio >= 0.75)
	{
		Action1();
	}
	else if (hpRatio >= 0.5)
	{
		Action2();
	}
	else if (hpRatio >= 0.25)
	{
		Action3();
	}
	else if (hpRatio > 0)
	{
		Action4();
	}
}

void Boss::Dead()
{
	//ボスが倒れたらゲームクリア
	m_pGame->GameClear();
}

void Boss::Action1()
{
	//通常弾はプレイヤーに向けて発射
	if (m_shotCoolTimer <= 0)
	{
		const Math::Vector2 playerPos = m_pGame->GetPlayer()->GetPos();
		Math::Vector2 pos = { m_pos.x - m_radius.x, m_pos.y };
		Math::Vector2 move;
		float radian = atan2f(playerPos.y - pos.y, playerPos.x - pos.x);
		move.x = cosf(radian) * m_bulletSpeed;
		move.y = sinf(radian) * m_bulletSpeed;
		m_pGame->GetBulletManager()->Shoot(pos, move);
		m_shotCnt++;
		if (m_shotCnt >= 3)
		{
			m_shotCoolTimer = m_shotCoolTime * 3;
			m_shotCnt = 0;
		}
		else
		{
			m_shotCoolTimer = m_shotCoolTime;
		}
	}

	
	if (m_circleShotCoolTimer <= 0)
	{
		//弾を30発出す
		int bulletNum = 20;
		float bulletSpeed = 200;
		Math::Vector2 vec;
		float radian;
		for (int i = 0; i < bulletNum; i++)
		{
			radian = DirectX::XMConvertToRadians(360 / bulletNum * i);
			vec.x = cosf(radian) * bulletSpeed;
			vec.y = sinf(radian) * bulletSpeed;
			m_pGame->GetBulletManager()->Shoot(m_pos, vec);
		}
		//circleShotを３回撃ったら長めのクールタイム
		if (m_circleShotCnt >= 3)
		{
			m_circleShotCoolTimer = 10;
			m_circleShotCnt = 0;
		}
		//普通のクールタイム
		else
		{
			m_circleShotCoolTimer = 0.3f;
		}

		//circleShotを連続で撃った回数をカウント
		m_circleShotCnt++;
	}
}

void Boss::Action2()
{
	//通常弾はプレイヤーに向けて発射
	if (m_shotCoolTimer <= 0)
	{
		const Math::Vector2 playerPos = m_pGame->GetPlayer()->GetPos();
		Math::Vector2 pos = { m_pos.x - m_radius.x, m_pos.y };
		Math::Vector2 move;
		float radian = atan2f(playerPos.y - pos.y, playerPos.x - pos.x);
		move.x = cosf(radian) * m_bulletSpeed;
		move.y = sinf(radian) * m_bulletSpeed;
		m_pGame->GetBulletManager()->Shoot(pos, move);
		m_shotCnt++;
		if (m_shotCnt >= 5)
		{
			m_shotCoolTimer = m_shotCoolTime * 3;
			m_shotCnt = 0;
		}
		else
		{
			m_shotCoolTimer = m_shotCoolTime;
		}
	}


	if (m_circleShotCoolTimer <= 0)
	{
		//弾を30発出す
		int bulletNum = 20;
		float bulletSpeed = 200;
		Math::Vector2 vec;
		//一発目はプレイヤー目掛けて撃つ
		float startRadian = atan2(s_pPlayerPos->y - m_pos.y,s_pPlayerPos->x-m_pos.x);
		float radian;
		for (int i = 0; i < bulletNum; i++)
		{
			radian = startRadian + DirectX::XMConvertToRadians(360 / bulletNum * i);
			vec.x = cosf(radian) * bulletSpeed;
			vec.y = sinf(radian) * bulletSpeed;
			m_pGame->GetBulletManager()->Shoot(m_pos, vec);
		}
		//circleShotを3回撃ったら長めのクールタイム
		if (m_circleShotCnt >= 3)
		{
			m_circleShotCoolTimer = 10;
			m_circleShotCnt = 0;
		}
		//普通のクールタイム
		else
		{
			m_circleShotCoolTimer = 0.3f;
		}

		//circleShotを連続で撃った回数をカウント
		m_circleShotCnt++;
	}
}

void Boss::Action3()
{
	//通常弾はプレイヤーに向けて発射
	if (m_shotCoolTimer <= 0)
	{
		const Math::Vector2 playerPos = m_pGame->GetPlayer()->GetPos();
		Math::Vector2 pos = { m_pos.x - m_radius.x, m_pos.y };
		Math::Vector2 move;
		float radian = atan2f(playerPos.y - pos.y, playerPos.x - pos.x);
		move.x = cosf(radian) * m_bulletSpeed;
		move.y = sinf(radian) * m_bulletSpeed;
		m_pGame->GetBulletManager()->Shoot(pos, move);
		m_shotCnt++;
		if (m_shotCnt >= 5)
		{
			m_shotCoolTimer = m_shotCoolTime * 3;
			m_shotCnt = 0;
		}
		else
		{
			m_shotCoolTimer = m_shotCoolTime;
		}
	}


	if (m_circleShotCoolTimer <= 0)
	{
		//弾を30発出す
		int bulletNum = 20;
		float bulletSpeed = 200;
		Math::Vector2 vec;
		//一発目はプレイヤー目掛けて撃つ
		float startRadian = atan2(s_pPlayerPos->y - m_pos.y, s_pPlayerPos->x - m_pos.x);
		float radian;
		for (int i = 0; i < bulletNum; i++)
		{
			radian = startRadian + DirectX::XMConvertToRadians(360 / bulletNum * i);
			vec.x = cosf(radian) * bulletSpeed;
			vec.y = sinf(radian) * bulletSpeed;
			m_pGame->GetBulletManager()->Shoot(m_pos, vec);
		}
		//circleShotを3回撃ったら長めのクールタイム
		if (m_circleShotCnt >= 3)
		{
			m_circleShotCoolTimer = 10;
			m_circleShotCnt = 0;
		}
		//普通のクールタイム
		else
		{
			m_circleShotCoolTimer = 0.3f;
		}

		//circleShotを連続で撃った回数をカウント
		m_circleShotCnt++;
	}
}

void Boss::Action4()
{
	//通常弾はプレイヤーに向けて発射
	if (m_shotCoolTimer <= 0)
	{
		const Math::Vector2 playerPos = m_pGame->GetPlayer()->GetPos();
		Math::Vector2 pos = { m_pos.x - m_radius.x, m_pos.y };
		Math::Vector2 move;
		float radian = atan2f(playerPos.y - pos.y, playerPos.x - pos.x);
		move.x = cosf(radian) * m_bulletSpeed;
		move.y = sinf(radian) * m_bulletSpeed;
		m_pGame->GetBulletManager()->Shoot(pos, move);
		m_shotCnt++;
		if (m_shotCnt >= 5)
		{
			m_shotCoolTimer = m_shotCoolTime * 3;
			m_shotCnt = 0;
		}
		else
		{
			m_shotCoolTimer = m_shotCoolTime;
		}
	}


	if (m_circleShotCoolTimer <= 0)
	{
		//弾を30発出す
		int bulletNum = 20;
		float bulletSpeed = 200;
		Math::Vector2 vec;
		//一発目はプレイヤー目掛けて撃つ
		float startRadian = atan2(s_pPlayerPos->y - m_pos.y, s_pPlayerPos->x - m_pos.x);
		float radian;
		for (int i = 0; i < bulletNum; i++)
		{
			radian = startRadian + DirectX::XMConvertToRadians(360 / bulletNum * i);
			vec.x = cosf(radian) * bulletSpeed;
			vec.y = sinf(radian) * bulletSpeed;
			m_pGame->GetBulletManager()->Shoot(m_pos, vec);
		}
		//circleShotを3回撃ったら長めのクールタイム
		if (m_circleShotCnt >= 3)
		{
			m_circleShotCoolTimer = 10;
			m_circleShotCnt = 0;
		}
		//普通のクールタイム
		else
		{
			m_circleShotCoolTimer = 0.3f;
		}

		//circleShotを連続で撃った回数をカウント
		m_circleShotCnt++;
	}
}

