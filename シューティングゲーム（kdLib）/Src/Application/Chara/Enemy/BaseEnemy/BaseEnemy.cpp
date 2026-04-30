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
	m_pos = {};						//座標
	m_move = {};					//移動量
	m_moveSpeed = 50;
	m_radius = { 32,32 };			//半径	
	m_scale = 0.5f;					//画像の拡縮

	m_animCnt = 0;					//現在のアニメーション
	m_animCntMax = 4;				//アニメーションのコマ数
	m_animSpeed = 5;				//アニメーションのスピード

	m_color = { 1,1,1,1 };			//色

	m_bActive = true;				//活性状態

	m_hpMax = 10;					//最大HP
	m_hp = m_hpMax;					//HPを初期化

	m_timer = 0;					//出現している時間

	m_shotWait = 3;					//敵が弾を撃つクールタイム
	m_shotWaitTimer = m_shotWait;	//敵が弾を撃つクールタイムを初期化

	m_bDead_ScreenOut = false;		//画面外に出たら敵を倒すフラグ
}

//更新
void BaseEnemy::Update(float deltaTime)
{
	//アニメーションを進める
	m_animCnt += m_animSpeed * deltaTime;
	if (m_animCnt >= m_animCntMax)m_animCnt = 0;

	//クールタイムを減らす
	m_shotWaitTimer -= deltaTime;
	if (m_shotWaitTimer < 0)m_shotWaitTimer = 0;

	//敵の行動パターン通りに動く
	Action(deltaTime);

	//座標更新
	m_pos += m_move * deltaTime;

	//スクリーン内に入った敵は「スクリーン外に出ると倒れるフラグ」を立てる
	if (!m_bDead_ScreenOut)
	{
		if(IsCollision_Box(m_pos, m_radius, { 0,0 }, { SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f }))
		{
			m_bDead_ScreenOut = true;
		}
	}
	//画面外に出たら非活性にする
	//(スクリーンアウトすることで敵が倒れるフラグが経っていたら)
	else
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
	Math::Matrix scale = Math::Matrix::CreateScale(m_scale, m_scale, 0);
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scale * trans;
}

//描画
void BaseEnemy::Draw()
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	char path[50];
	sprintf_s(path, sizeof(path), "Texture/Enemy/Enemy%d.png", (int)m_animCnt);
	SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get(path), m_color);
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
void BaseEnemy::Action(float deltaTime)
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