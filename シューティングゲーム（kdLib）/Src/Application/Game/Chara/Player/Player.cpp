#include"Player.h"
#include"../../../Toolkit/Font/DrawString.h"
#include"../../Bullet/BulletManager.h"
#include"../../../Toolkit/KeyManager.h"
#include"../../../Timer/Timer.h"
#include"../../../Fireworks/FireworksManager.h"
#include"../../../Mouse/Mouse.h"

//コンストラクタ
Player::Player()
{
	
}

void Player::Init(FireworksManager* set)
{
	m_pFireworksManager = set;
	m_pos = { 0,0 };
	m_move = { 0,0 };
	m_hpMax = 100;
	m_hp = m_hpMax;
	m_scale = 0.5f;
	m_color = { 1,1,1,1 };
	m_bInvincible = false;
	m_invincibleTime = 0;
	m_sumDeltaTime = 0;
	m_tex.Load("Texture/Player.png");

	//プレイヤーの大きさを求める（半径）
	m_radius.x = (m_tex.GetInfo().Width / 2.0f) * m_scale;
	m_radius.y = (m_tex.GetInfo().Height / 2.0f) * m_scale;

	//行列作成
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_scale, m_scale, 0);
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scaleMat * transMat;
}


//更新
void Player::Update(float deltaTime)
{
	//無敵時間の更新
	if (m_bInvincible)
	{
		//経過した時間を足す
		m_sumDeltaTime += deltaTime;

		//無敵時間から経過した時間分、減らす
		m_invincibleTime -= deltaTime;

		//無敵時間が終わったら
		if (m_invincibleTime < 0)
		{
			m_bInvincible = false;
			m_invincibleTime = 0;
			m_sumDeltaTime = 0;
			m_color = { 1,1,1,1 };
		}
		//無敵時間中は、
		//0.1秒経過ごとに透明度を切り替える
		else if (m_sumDeltaTime >= 0.1f)
		{
			//0.1秒分、引く
			m_sumDeltaTime -= 0.1f;	

			//色を切り替える
			if (m_color.A() == 1.0f)
				m_color.A(0.1f);
			else
				m_color.A(1.0f);
		}
	}

	//座標更新
	m_pos += m_move * deltaTime;


	//画面端判定
	float left = m_pos.x - m_radius.x;
	float right = m_pos.x + m_radius.x;
	float top = m_pos.y + m_radius.y;
	float bottom = m_pos.y - m_radius.y;
	if (left < SCREEN_LEFT)
	{
		m_pos.x = SCREEN_LEFT + m_radius.x;
	}
	else if (right > SCREEN_RIGHT)
	{
		m_pos.x = SCREEN_RIGHT - m_radius.x;
	}
	else if (top > SCREEN_TOP)
	{
		m_pos.y = SCREEN_TOP - m_radius.x;
	}
	else if (bottom < SCREEN_BOTTOM)
	{
		m_pos.y = SCREEN_BOTTOM + m_radius.x;
	}


	//減速処理
	m_move *= 1.0f - (2.0f * deltaTime);		//2.0の値はちょうどよい減速率にするため(毎フレーム0.98くらい？になりそう)

	//行列作成
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_scale, m_scale, 0);
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scaleMat* transMat;
}

//描画
void Player::Draw()
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex_Src(&m_tex, m_color);
	float fontSize = 8;
	float interval = 8;				//プレイヤーと文字との間隔
	DWriteCustom::Instance().Draw("Player", { m_pos.x- m_radius.x,m_pos.y + m_radius.y + fontSize + interval },fontSize);
}

//行動
void Player::Action(float deltaTime)
{
	if (KEY.IsHeld(VK_LEFT) || KEY.IsHeld('A'))
	{
		m_move.x -= 30.0f;
		if (m_move.x < s_speedMax)m_move.x = -s_speedMax;
	}
	if (KEY.IsHeld(VK_RIGHT) || KEY.IsHeld('D'))
	{
		m_move.x += 30.0f;
		if (m_move.x > s_speedMax)m_move.x = s_speedMax;
	}
	if (KEY.IsHeld(VK_UP) || KEY.IsHeld('W'))
	{
		m_move.y += 30.0f;
		if (m_move.y > s_speedMax)m_move.y = s_speedMax;
	}
	if (KEY.IsHeld(VK_DOWN) || KEY.IsHeld('S'))
	{
		m_move.y -= 30.0f;
		if (m_move.y < -s_speedMax)m_move.y = -s_speedMax;
	}

	//弾
	m_shotWait -= deltaTime;
	if (m_shotWait < 0)m_shotWait = 0;
	if (m_shotWait == 0)
	{
		if (KEY.IsHeld(VK_LBUTTON))
		{
			//弾発射
			m_pFireworksManager->Shot(m_pos, MOUSE.GetPosf());
			//撃つ待機時間を設ける
			m_shotWait = 0.1f;				//0.3秒
		}
	}
}

//倒れた時の処理
void Player::Dead()
{
	//移動量をなくす
	m_move = { 0,0 };
	Timer::Instance().Slow(3.0f, 0.3f);
}
