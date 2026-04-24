#include"Enemy1.h"
#include"../../main.h"
#include"../../Tools/RandEx/RandEx.h"

//初期化
void Enemy1::Init()
{
	m_pos = {};						//座標
	m_move = {};					//移動量
	m_radius = { 32,32 };			//半径
	m_scale = 0.5f;					//画像の拡縮

	m_color = { 1,0.3f,0.3f,0.1f };	//色

	m_bActive = true;				//活性状態

	m_hpMax = 10;					//最大HP
	m_hp = m_hpMax;					//HPを初期化

	m_timer = 0;					//出現している時間

	m_shotWait = 1;					//敵が弾を撃つクールタイム
	m_shotWaitTimer = m_shotWait;	//敵が弾を撃つクールタイムを初期化
}

//倒れた時の処理
void Enemy1::Dead()
{
	m_bActive = false;
}

//出現させるときの処理
void Enemy1::Spawn()
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
void Enemy1::ActionPutturn()
{
	

}
