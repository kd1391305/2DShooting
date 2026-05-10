#pragma once
#include "BaseEnemy.h"
#include"../../../Font/DWriteCustom.h"
#include"../../../Tools/RandEx/RandEx.h"
#include"../../../main.h"
#include"../../../Bullet/BulletManager.h"
#include"../../../TextureCache/TextureCache.h"
#include"../../../Tools/Collision/Collision.h"

//静的変数の初期化
BulletManager* BaseEnemy::s_pBulletManager = nullptr;
Math::Vector2* BaseEnemy::s_pPlayerPos = nullptr;

void BaseEnemy::Init()
{
	m_radian = 0;					//敵画像の回転角度

	m_animCnt = 0;					//現在のアニメーション
	m_animCntMax = 4;				//アニメーションのコマ数
	m_animSpeed = 5;				//アニメーションのスピード

	m_bActive = true;				//活性状態
}

//更新
void BaseEnemy::Update(float deltaTime)
{
	m_timer += deltaTime;

	//アニメーションを進める
	m_animCnt += m_animSpeed * deltaTime;
	if (m_animCnt >= m_animCntMax)m_animCnt = 0;

	//クールタイムを減らす
	m_shotCoolTimer -= deltaTime;
	if (m_shotCoolTimer < 0)m_shotCoolTimer = 0;

	if (m_bHitFlg)
	{
		m_hitEffectTimer -= deltaTime;
		if (m_hitEffectTimer <= 0)
		{
			m_hitEffectTimer = 0;
			m_color = m_normalColor;
			m_bHitFlg = false;
		}
	}

	//敵の行動パターン通りに動く
	Action(deltaTime);

	//座標更新
	m_pos += m_move * deltaTime;
	
	//(スポーンから10秒経っていて、敵が画面外にいたら、敵を消す)
	//何らかの不具合で敵がうまく発生しなかった時のためにも必要
	if (m_timer >= 10)
	{
		float left = m_pos.x - m_radius.x;
		float right = m_pos.x + m_radius.x;
		float top = m_pos.y + m_radius.y;
		float bottom = m_pos.y - m_radius.y;
		if (right < SCREEN_LEFT || left > SCREEN_RIGHT || bottom > SCREEN_TOP || top < SCREEN_BOTTOM)		//一旦、左端のみ判定する
		{
			m_bActive = false;
			return;
		}
	}

	//行列作成
	Math::Matrix scale = Math::Matrix::CreateScale(m_scale.x, m_scale.y, 0);
	Math::Matrix rotation = Math::Matrix::CreateRotationZ(m_radian);
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scale *rotation *  trans;
}

//敵を出現させる
void BaseEnemy::Spawn(Math::Vector2& pos, Math::Vector2& radius, float moveSpeed, float moveDeg, Math::Color& normalColor, Math::Color& hitColor, float hp, float bulletSpeed, float shotCoolTime, const float shotCoolTimeNoiseMax,const float spawnShotCoolTime)
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
		KdTexture* tex = TextureCache::Instance().Get("Texture/Enemy/Enemy0.png").get();
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

	//画面内でスポーンするなら、画面外判定を行うタイマーを判定し始める時間から開始する
	{
		float left = m_pos.x - m_radius.x;
		float right = m_pos.x + m_radius.x;
		float top = m_pos.y + m_radius.y;
		float bottom = m_pos.y - m_radius.y;
		if (left >SCREEN_LEFT && right < SCREEN_RIGHT && top < SCREEN_TOP && bottom > SCREEN_BOTTOM )
		{
			m_timer = 10;
		}
	}
}

//描画
void BaseEnemy::Draw()
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	char path[50];
	sprintf_s(path, sizeof(path), "Texture/Enemy/Enemy%d.png", (int)m_animCnt);
	SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get(path), m_color);
}

void BaseEnemy::Damage(float damage)
{
	m_hp -= damage;
	if (m_hp <= 0)
	{
		m_hp = 0;
		Dead();			//倒れた時の処理を呼び出す
	}
	if (m_hp > m_hpMax)m_hp = m_hpMax;

	m_bHitFlg = true;
	m_hitEffectTimer = m_hitEffectTime;
	m_color = m_hitColor;
}

