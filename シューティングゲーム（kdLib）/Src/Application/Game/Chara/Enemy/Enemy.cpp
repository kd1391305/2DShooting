#pragma once
#include "Enemy.h"
#include"../../../ToolKit/Font/DrawString.h"
#include"../../../ToolKit/RandEx.h"
#include"../../../main.h"
#include"../../Bullet/BulletManager.h"

C_Enemy::C_Enemy()
{
	m_pos = { SCREEN_RIGHT,0 };			//座標
	m_move = { -5,0 };							//移動量
	m_radius = { 32,32 };						//半径	
	m_bActive = false;								//活性状態
	m_shotWait = 60;								//撃つまでのクールタイム

	//行列作成
	m_mat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
}

void C_Enemy::Update()
{
	//クールタイムを減らす
	m_shotWait--;
	if (m_shotWait < 0)m_shotWait = 0;

	//クールタイムが終わったら
	if (m_shotWait <= 0)
	{
		//確率で弾を発射
		if (HitGacha(s_shotProbability))
		{
			BULLET_MANAGER.ShotOfEnemy(m_pos, m_move * 2);
			m_shotWait = 60;			//クールタイムを設ける
		}
	}
	//座標更新
	m_pos += m_move;

	//画面外に出たら非活性にする
	if (m_pos.x < SCREEN_LEFT)		//一旦、左端のみ判定する
	{
		m_bActive = false;
		return;
	}

	m_mat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
}
void C_Enemy::Draw(KdTexture* tex)
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex_Src(tex);
}

void C_Enemy::Spawn(Math::Vector2 pos, Math::Vector2 move)
{
	m_pos = pos;
	m_move = move;
	m_bActive = true;
	m_mat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_shotWait = 20;
}
