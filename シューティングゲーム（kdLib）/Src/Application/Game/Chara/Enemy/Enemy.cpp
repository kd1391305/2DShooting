#pragma once
#include "Enemy.h"
#include"../../../ToolKit/Font/DrawString.h"
#include"../../../ToolKit/RandEx.h"
#include"../../../main.h"
#include"../../Bullet/BulletManager.h"

//静的変数の初期化
BulletManager* Enemy::s_pBulletManager = nullptr;

//コンストラクタ
Enemy::Enemy()
{
	m_pos = {};			//座標
	m_move = {};		//移動量
	m_radius = {32,32};		//半径	
	m_bActive = false;	//活性状態
	m_shotWait = 60;	//撃つまでのクールタイム
	//行列作成
	m_mat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
}

//更新
void Enemy::Update(float deltaTime)
{
	//クールタイムを減らす
	m_shotWait -= deltaTime;
	if (m_shotWait < 0)m_shotWait = 0;

	//クールタイムが終わったら
	if (m_shotWait <= 0)
	{
		//確率で弾を発射
		if (HitGacha(s_shotProbability))
		{
			s_pBulletManager->Shot(m_pos, m_move * 2);
			m_shotWait = 3;			//3秒間のクールタイムを設ける
		}
	}

	//座標更新
	m_pos += m_move * deltaTime;

	//画面外に出たら非活性にする
	float left = m_pos.x - m_radius.x;
	if (left < SCREEN_LEFT)		//一旦、左端のみ判定する
	{
		m_bActive = false;
		return;
	}

	m_mat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
}

//描画
void Enemy::Draw(KdTexture* tex)
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex_Src(tex);
}

//敵をスポーンさせる
void Enemy::Spawn(Math::Vector2 pos, Math::Vector2 move)
{
	m_pos = pos;
	m_move = move;
	m_radius = { 32,32 };			//半径	
	m_bActive = true;
	m_mat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_shotWait = 3;					//敵が弾を撃つクールタイム
}
