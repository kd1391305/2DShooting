#include "Boss.h"
#include"../../../TextureCache/TextureCache.h"
#include"../../../Bullet/BulletManager.h"
#include"../../../Scene/GameScene/GameScene.h"
#include"../../Player/Player.h"
#include"../../../UI/UI.h"
#include"../../../Background/Back.h"
#include"../../../Fireworks/FireworksManager.h"
#include"../../../Tools/RandEx/RandEx.h"
#include"../Enemies/Enemies.h"
#include"../EnemyManager.h"
#include"AttackArea/AttackAreaManager.h"
#include"AttackArea/AttackArea.h"
#include"../../../main.h"
#include"../../../SoundCache/SoundCache.h"

Boss::Boss(Game*game):
	m_pGame(game)
{
}

void Boss::InitOriginal()
{
	m_shotCnt = 0;

	m_circleShotCoolTimer = 7.0f;
	m_circleShotCnt = 0;
	m_fallEnemyCoolTimer = 5.0f;
	m_fallEnemyCnt = 0;
	m_bHitChargeMax = false;
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
	if (m_hitEffectTimer <= 0 || !m_bHitChargeMax)
	{
		SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get(path), m_color);
	}
	else
	{
		D3D.SetBlendState(BlendMode::Add);
		for (int i = 0; i < 2; i++)SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get(path), { 1,1,1,1 });
		D3D.SetBlendState(BlendMode::Alpha);
	}
}

void Boss::Action(float deltaTime)
{
	if (m_hitEffectTimer <= 0)
	{
		m_bHitChargeMax = false;
	}

	//ボスの座標補正
	if (m_pos.x < m_endPosX)
	{
		m_move.x = 0;
		m_pos.x = m_endPosX;
	}

	
	m_circleShotCoolTimer -= deltaTime;
	m_fallEnemyCoolTimer -= deltaTime;

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

	if (m_pGame->GetBack()->GetFireworks()->GetList().size() >= 30)return;
	for (int i = 0; i < 3; i++)
	{
		//花火を打ち上げる
		float radian = DirectX::XMConvertToRadians(randRange(0, 360));
		Math::Vector2 vec;
		vec.x = cosf(radian) * m_radius.x;
		vec.y = sinf(radian) * m_radius.y;

		Math::Color color = { randRange(0,1),randRange(0,1),randRange(0,1),randRange(0.5,1.0f) };
		FireworksManager::Type type = FireworksManager::GetRandomType_Quick();
		for (int j = 0; j < 2; j++)
		{
			float scale = randRange(0.3f, 0.5f);
			m_pGame->GetBack()->GetFireworks()->Explode(type, m_pos + vec, scale, color, 0.01f);
		}
	}
	m_bActive = false;
}

void Boss::ExplodeFireworks(float power)
{
	m_bHitChargeMax = true;

	for (int i = 0; i < power / 2.0f; i++)
	{
		float radian = DirectX::XMConvertToRadians(randRange(0, 360));
		Math::Vector2 vec;
		vec.x = cosf(radian) * m_radius.x;
		vec.y = sinf(radian) * m_radius.y;

		Math::Color color = { randRange(0,1),randRange(0,1),randRange(0,1),randRange(0.5,1.0f) };
		FireworksManager::Type type = FireworksManager::GetRandomType_Quick();
		for (int j = 0; j < 1; j++)
		{
			m_pGame->GetBack()->GetFireworks()->Explode(type, m_pos + vec, 0.5f, color, 0.01f);
		}
	}
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

	//敵を落とす
	if (m_fallEnemyCoolTimer <= 0)
	{
		//落とす敵は五体
		//プレイヤーを中心に半径200メートル以内に落とす
		//落とす範囲
		float areaRadius = 200;
		float offsetY = 300;		//300上の位置から落とす(もし画面内スタートだったら、画面外からスタートさせる)
		Math::Vector2 pos;
		Math::Vector2 enemyRadius = { 30,30 };
		std::shared_ptr<Enemy10>enemy;
		std::shared_ptr<AttackArea>area;
		Math::Color enemyColor = { 1,1,1,1 };
		float startDeg = 135;		//左上向きに発射
		Math::Vector2 spawnPos = { m_pos.x - m_radius.x / 2.0f,m_pos.y + m_radius.y / 2.0f };

		//攻撃範囲の発生
		pos = { randRange(s_pPlayerPos->x - areaRadius, s_pPlayerPos->x + areaRadius), randRange(s_pPlayerPos->y - areaRadius, s_pPlayerPos->y + areaRadius) };
		area = m_pGame->GetAttackAreaManager()->Emit(pos, enemyRadius * 2);

		//敵を落とす
		enemy = std::make_shared<Enemy10>();
		enemy->Init();
		enemy->InitOriginal(area, m_pGame->GetBack()->GetFireworks());
		enemy->Spawn(spawnPos, enemyRadius, 400, startDeg, enemyColor, enemyColor, 9999, NULL, NULL);
		m_pGame->GetEnemyManager()->GetEnemyList().push_back(enemy);

		std::shared_ptr<KdSoundInstance> se = SoundCache::Instance().Get("Sound/SE/Throw.wav",SoundCache::SoundState::NotPlaying);
		se->SetVolume(0.1f);
		se->Play(false);

		//クールタイム
		if (m_fallEnemyCnt > 3)
		{
			m_fallEnemyCoolTimer = randRange(5, 8);
			m_fallEnemyCnt = 0;
		}
		else
		{
			m_fallEnemyCoolTimer = 0.3f;
			m_fallEnemyCnt++;
		}
	}
	
	//if (m_circleShotCoolTimer <= 0)
	//{
	//	//弾を30発出す
	//	int bulletNum = 20;
	//	float bulletSpeed = 200;
	//	Math::Vector2 vec;
	//	float radian;
	//	for (int i = 0; i < bulletNum; i++)
	//	{
	//		radian = DirectX::XMConvertToRadians(360 / bulletNum * i);
	//		vec.x = cosf(radian) * bulletSpeed;
	//		vec.y = sinf(radian) * bulletSpeed;
	//		m_pGame->GetBulletManager()->Shoot(m_pos, vec);
	//	}
	//	//circleShotを３回撃ったら長めのクールタイム
	//	if (m_circleShotCnt >= 3)
	//	{
	//		m_circleShotCoolTimer = 10;
	//		m_circleShotCnt = 0;
	//	}
	//	//普通のクールタイム
	//	else
	//	{
	//		m_circleShotCoolTimer = 0.3f;
	//	}

	//	//circleShotを連続で撃った回数をカウント
	//	m_circleShotCnt++;
	//}
}

void Boss::Action2()
{
	//通常弾はプレイヤーに向けて発射
	if (m_shotCoolTimer <= 0)
	{
		std::shared_ptr<Enemy9>enemy;
		enemy = std::make_shared<Enemy9>(m_pGame->GetEnemyManager().get());
		enemy->Init();

		float explodeCnt = 2;
		float increaseShotNum = 2;
		float shotNum = 4;
		float shotArcRadian = DirectX::XMConvertToRadians(150);
		Math::Vector2 shotPos = { m_pos.x - m_radius.x,m_pos.y };
		enemy->InitOriginal(explodeCnt, increaseShotNum, shotNum, shotArcRadian);
		enemy->Spawn(shotPos, Math::Vector2{ 32,32 }, 300, 135, Math::Color{ 1,1,1,1 }, Math::Color{ 1,0.8,0.8,1 }, 999, 220, 0.5f, 0, 0);
		m_pGame->GetEnemyManager()->GetEnemyList().push_back(enemy);

		m_shotCoolTimer = 5;
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
		int bulletNum = 30;
		float bulletSpeed = 250;
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
		if (m_circleShotCnt >= 5)
		{
			m_circleShotCoolTimer = 3.5;
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
		if (m_shotCnt >= 6)
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
		int bulletNum = 25;
		float bulletSpeed = 280;
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
		if (m_circleShotCnt >= 10)
		{
			m_circleShotCoolTimer = 2;
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

	//敵を落とす
	if (m_fallEnemyCoolTimer <= 0)
	{
		//落とす敵は10~15体
		//プレイヤーに落とす
		//落とす範囲
		float areaRadius = 200;
		float offsetY = 300;						//300上の位置から落とす(もし画面内スタートだったら、画面外からスタートさせる)
		Math::Vector2 enemyRadius = { 30,30 };
		std::shared_ptr<Enemy10>enemy;
		std::shared_ptr<AttackArea>area;
		Math::Color enemyColor = { 1,1,1,1 };
		Math::Vector2 spawnPos = { m_pos.x - m_radius.x / 2.0f,m_pos.y + m_radius.y / 2.0f };
		float startDeg = 135;		//左上向きに発射

		//攻撃範囲の発生
		area = m_pGame->GetAttackAreaManager()->Emit(*s_pPlayerPos, enemyRadius * 2);

		//敵を落とす
		enemy = std::make_shared<Enemy10>();
		enemy->Init();
		enemy->InitOriginal(area, m_pGame->GetBack()->GetFireworks());
		enemy->Spawn(spawnPos, enemyRadius, 500, startDeg, enemyColor, enemyColor, 9999, NULL, NULL);
		m_pGame->GetEnemyManager()->GetEnemyList().push_back(enemy);

		//クールタイム
		if (m_fallEnemyCnt > 3)
		{
			m_fallEnemyCoolTimer = randRange(5, 8);
			m_fallEnemyCnt = 0;
		}
		else
		{
			m_fallEnemyCoolTimer = 0.3f;
			m_fallEnemyCnt++;
		}
	}
}

