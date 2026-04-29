#include"Enemy1.h"
#include"../../../main.h"
#include"../../../Tools/RandEx/RandEx.h"
#include"../../../Bullet/BulletManager.h"


//初期化
void Enemy1::Init()
{
	m_pos = {};						//座標
	m_move = {};					//移動量
	m_moveSpeed = 200;
	m_radius = { 32,32 };			//半径
	m_scale = 0.5f;					//画像の拡縮

	m_animCnt = 0;					//現在のアニメーション
	m_animCntMax = 4;				//アニメーションのコマ数
	m_animSpeed = 5;				//アニメーションのスピード

	m_color = { 1,0.5f,0.5f,0.8f };	//色

	m_bActive = true;				//活性状態

	m_hpMax = 10;					//最大HP
	m_hp = m_hpMax;					//HPを初期化

	m_shotWait = 5;					//敵が弾を撃つクールタイム
	m_shotWaitTimer = m_shotWait;	//敵が弾を撃つクールタイムを初期化

	m_bDead_ScreenOut = false;
}

//倒れた時の処理
void Enemy1::Dead()
{
	m_bActive = false;
}


//出現させるときの処理
void Enemy1::Spawn()
{
	m_move = { -m_moveSpeed,0 };	//移動量

	//行列作成
	Math::Matrix scale = Math::Matrix::CreateScale(m_scale, m_scale, 0);
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scale * trans;
}

//敵の行動パターン通りに動く
void Enemy1::Action(float deltaTime)
{
	//クールタイムが終わったら
	if (m_shotWaitTimer <= 0)
	{
		Math::Vector2 move;
		//プレイヤーに向けて発射する
		float radian = atan2f(s_pPlayerPos->y - m_pos.y, s_pPlayerPos->x - m_pos.x);
		move.x = cosf(radian) * m_shotSpeed;
		move.y = sinf(radian) * m_shotSpeed;

		s_pBulletManager->Shot(m_pos,move);
		m_shotWaitTimer = m_shotWait;			//3秒間のクールタイムを設ける
	}
}
