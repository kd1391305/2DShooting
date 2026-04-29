#include"Enemy2.h"
#include"../../../main.h"
#include"../../../Tools/RandEx/RandEx.h"
#include"../../../Bullet/BulletManager.h"

//初期化
void Enemy2::Init()
{
	m_pos = {};						//座標
	m_move = {};					//移動量
	m_moveSpeed = 150;
	m_radius = { 32,32 };			//半径
	m_scale = 0.5f;					//画像の拡縮

	m_animCnt = 0;					//現在のアニメーション
	m_animCntMax = 4;				//アニメーションのコマ数
	m_animSpeed = 5;				//アニメーションのスピード

	m_color = { 1,1.0f,0.0f,1.0f };	//色

	m_bActive = true;				//活性状態

	m_hpMax = 10;					//最大HP
	m_hp = m_hpMax;					//HPを初期化

	m_timer = 0;					//出現している時間

	m_shotWait = 1;					//敵が弾を撃つクールタイム
	m_shotWaitTimer = m_shotWait;	//敵が弾を撃つクールタイムを初期化

	m_bDead_ScreenOut = false;
}

//倒れた時の処理
void Enemy2::Dead()
{
	m_bActive = false;
}

//出現させるときの処理
void Enemy2::Spawn()
{
	//行列作成
	Math::Matrix scale = Math::Matrix::CreateScale(m_scale, m_scale, 0);
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scale * trans;
}

void Enemy2::InitSpawn(Math::Vector2 startPos, Math::Vector2 endPos, Math::Vector2 centerMoveSpeed, float radian, float deltaRadian, float gapCenter)
{
	m_centerPos = startPos;
	m_endPos = endPos;
	m_centerMoveSpeed = centerMoveSpeed;
	m_radian = radian;
	m_deltaRadian = deltaRadian;

	m_pos = m_centerPos;
	m_gapCenter = gapCenter;

	//中心との差をもとに初期位置を決める
	m_pos.x += cosf(m_radian) * m_gapCenter;
	m_pos.y += sinf(m_radian) * m_gapCenter;
}
//敵の行動パターン通りに動く
void Enemy2::Action(float deltaTime)
{
	m_radian += m_deltaRadian * deltaTime;

	m_centerPos += m_centerMoveSpeed * deltaTime;

	//次の移動量を求める
	Math::Vector2 nextPos;
	nextPos.x = m_centerPos.x + cosf(m_radian) * m_gapCenter;
	nextPos.y = m_centerPos.y + sinf(m_radian) * m_gapCenter;

	m_pos = nextPos;

	m_move = {};
	//終点地点を超えたとき
	if (m_centerPos.x <= m_endPos.x)
	{
		//中心の移動量をなくす
		m_centerMoveSpeed = {0,0};
	}

	if (m_shotWaitTimer <= 0)
	{
		//プレイヤーに向けて発射する
		Math::Vector2 move;
		float radian = atan2f(s_pPlayerPos->y - m_pos.y, s_pPlayerPos->x - m_pos.x);
		move.x = cosf(radian) * m_shotSpeed;
		move.y = sinf(radian) * m_shotSpeed;
		s_pBulletManager->Shot(m_pos, move);
		
		//クールタイム
		m_shotWaitTimer = m_shotWait;
	}
}
