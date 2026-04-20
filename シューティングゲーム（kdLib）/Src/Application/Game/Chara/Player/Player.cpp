#include"Player.h"
#include"../../../Toolkit/Font/DrawString.h"
#include"../../Bullet/BulletManager.h"
#include"../../../Toolkit/KeyManager.h"
#include"../../../Save/Save.h"

//コンストラクタ
C_Player::C_Player()
{
	m_pos = {0,0};
	m_move = { 0,0 };
	m_hpMax = 100;
	m_hp = m_hpMax;
	m_scale = 0.5f;
	m_color = { 1,1,1,1 };
	m_bInvincible = false;
	m_invincibleTime = 0;
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
void C_Player::Update()
{
	//無敵時間の更新
	if (m_bInvincible)
	{
		m_invincibleTime--;
		if (m_invincibleTime < 0)
		{
			m_bInvincible = false;
			m_invincibleTime = 0;
			m_color = { 1,1,1,1 };
		}
		//4フレームに一回、透明度を変更する
		else if (m_invincibleTime % 4 == 0)
		{
			if (m_color.A() == 1.0f)		m_color.A(0.1f);
			else									m_color.A(1.0f);
		}
	}
	//座標更新
	m_pos += m_move;

	//減速処理
	m_move *= 0.8f;

	//行列作成
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_scale, m_scale, 0);
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scaleMat* transMat;

}

//描画
void C_Player::Draw()
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex_Src(&m_tex, m_color);
	float fontSize = 8;
	float interval = 8;		//プレイヤーと文字との間隔
	DRAW_STRING.Draw("Player", { m_pos.x- m_radius.x,m_pos.y + m_radius.y + fontSize + interval },fontSize);
}

//行動
void C_Player::Action()
{
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_move.x -= 1.0f;
		if (m_move.x < s_speedMax)m_move.x = -s_speedMax;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_move.x += 1.0f;
		if (m_move.x > s_speedMax)m_move.x = s_speedMax;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_move.y += 1.0f;
		if (m_move.y > s_speedMax)m_move.y = s_speedMax;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_move.y -= 1.0f;
		if (m_move.y < -s_speedMax)m_move.y = -s_speedMax;
	}

	//弾
	m_shotWait--;
	if (m_shotWait < 0)m_shotWait = 0;
	if (m_shotWait == 0)
	{
		if (KEY.IsHeld(VK_SPACE))
		{
			//弾発射
			BULLET_MANAGER.ShotOfPlayer(m_pos, { 15,0 });
			//撃つ待機時間を設ける
			m_shotWait = 10;
		}
	}
}
