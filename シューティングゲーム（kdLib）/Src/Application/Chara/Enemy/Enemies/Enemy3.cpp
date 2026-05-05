#include "Enemy3.h"
#include"../../../Bullet/BulletManager.h"

void Enemy3::Init()
{
	m_pos = {};						//座標
	m_move = {};					//移動量
	m_moveSpeed = 120;
	m_scale = 0.8f;					//画像の拡縮
	m_radius = { 64*m_scale,64 * m_scale };			//半径
	
	m_animCnt = 0;					//現在のアニメーション
	m_animCntMax = 4;				//アニメーションのコマ数
	m_animSpeed = 5;				//アニメーションのスピード
	
	//色
	m_normalColor = { 0,1,0,1 };
	m_hitColor = { 1,1,0,1 };	
	m_color = m_normalColor;

	m_bActive = true;				//活性状態

	m_hpMax = 30;					//最大HP
	m_hp = m_hpMax;					//HPを初期化

	m_timer = 0;					//出現している時間

	m_shotWait = 1;					//敵が弾を撃つクールタイム
	m_shotWaitTimer = m_shotWait;	//敵が弾を撃つクールタイムを初期化

	m_bDead_ScreenOut = false;

	m_timer = 0;
}

void Enemy3::Dead()
{
	m_bActive = false;
}

void Enemy3::Spawn()
{
	//行列作成
	Math::Matrix scale = Math::Matrix::CreateScale(m_scale, m_scale, 0);
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scale * trans;
}

void Enemy3::OnHit()
{
	m_bHitFlg = true;
	m_hitEffectTimer = m_hitEffectTime;
	m_color = m_hitColor;
}

void Enemy3::Action(float deltaTime)
{
	//色を変更する
	if (m_bHitFlg)
	{
		m_hitEffectTimer -= deltaTime;
		if (m_hitEffectTimer <= 0)
		{
			m_hitEffectTimer = 0;
			m_bHitFlg = false;
			m_color = m_normalColor;
		}
	}

	//弾を発射する
	if (m_shotWaitTimer <= 0)
	{
		float radian = atan2f(s_pPlayerPos->y - m_pos.y, s_pPlayerPos->x - m_pos.x);
		Math::Vector2 move;
		move.x = cosf(radian) * m_shotSpeed;
		move.y = sinf(radian) * m_shotSpeed;

		s_pBulletManager->Shot({ m_pos.x - m_radius.x, m_pos.y }, m_move);

		//クールタイム
		m_shotWaitTimer = m_shotWait;
	}
}