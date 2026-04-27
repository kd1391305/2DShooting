#pragma once
#include "BaseEnemy.h"
#include"../../Font/DrawString.h"
#include"../../Tools/RandEx/RandEx.h"
#include"../../main.h"
#include"../../Bullet/BulletManager.h"
#include"../../TextureCache/TextureCache.h"

//静的変数の初期化
BulletManager* BaseEnemy::s_pBulletManager = nullptr;

void BaseEnemy::Init()
{
	m_pos = {};						//座標
	m_move = {};					//移動量
	m_radius = { 32,32 };			//半径	
	m_scale = 0.5f;					//画像の拡縮

	m_color = { 1,1,1,1 };			//色

	m_bActive = true;				//活性状態

	m_hpMax = 10;					//最大HP
	m_hp = m_hpMax;					//HPを初期化

	m_timer = 0;					//出現している時間

	m_shotWait = 3;					//敵が弾を撃つクールタイム
	m_shotWaitTimer = m_shotWait;	//敵が弾を撃つクールタイムを初期化
}

//更新
void BaseEnemy::Update(float deltaTime)
{
	m_timer += deltaTime;

	//クールタイムを減らす
	m_shotWaitTimer -= deltaTime;
	if (m_shotWaitTimer < 0)m_shotWaitTimer = 0;

	//敵の行動パターン通りに動く
	ActionPutturn();

	//座標更新
	m_pos += m_move * deltaTime;

	//画面外に出たら非活性にする
	float left = m_pos.x - m_radius.x;
	if (left < SCREEN_LEFT)		//一旦、左端のみ判定する
	{
		m_bActive = false;
		return;
	}

	//行列作成
	Math::Matrix scale = Math::Matrix::CreateScale(m_scale, m_scale, 0);
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scale * trans;
}

//描画
void BaseEnemy::Draw()
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/Enemy.png"), m_color);
}

//敵をスポーンさせる（画面右端からランダムで出現、敵はまっすぐに動く）
void BaseEnemy::Spawn()
{
	float x = SCREEN_RIGHT + m_radius.x;
	float y = randRange(
		SCREEN_BOTTOM + m_radius.y,	//最小値
		SCREEN_TOP - m_radius.y);	//最大値

	m_pos = { x,y };				//出現位置
	m_move = { -200,0 };			//移動量

	//行列作成
	Math::Matrix scale = Math::Matrix::CreateScale(m_scale, m_scale, 0);
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scale * trans;
}

//敵の行動パターン通りに動く
void BaseEnemy::ActionPutturn()
{
	//クールタイムが終わったら
	if (m_shotWaitTimer <= 0)
	{
		//確率で弾を発射
		if (HitGacha(0.01))		//1000分の１の確率で弾を発射
		{
			s_pBulletManager->Shot(m_pos, m_move * 2);
			m_shotWait = 3;			//3秒間のクールタイムを設ける
		}
	}
}