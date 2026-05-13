#include"PlayerBullet.h"
#include"../../main.h"
#include"../../TextureCache/TextureCache.h"

void PlayerBullet::Update(float deltaTime)
{
	m_anim += deltaTime * 10;
	if (m_anim >= 4)m_anim = 0;

	//座標更新
	m_pos += m_move * deltaTime;

	//画面外に出たら非活性状態にする
	if (m_pos.x < SCREEN_LEFT || m_pos.x > SCREEN_RIGHT ||
		m_pos.y > SCREEN_TOP || m_pos.y < SCREEN_BOTTOM)
	{
		m_bActive = false;
		return;
	}

	//行列作成
	Math::Matrix scaleMat = Math::Matrix::CreateScale(-1 - m_power / 7.0f, 1 + m_power / 7.0f, 0);
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scaleMat * transMat;
}

void PlayerBullet::Pierce()
{
	m_pierceNum++;
	m_power -= 2;
	if (m_power < 0)
	{
		m_power = 0;
		m_bActive = false;
	}
}

void PlayerBullet::Draw()
{
	Math::Rectangle rect;
	if (m_power == 10)	rect = { (11 + (int)m_anim) * 32, 5 * 32, 32, 32 };
	else				rect = { (11 + (int)m_anim) * 32, 0, 32, 32 };
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(TextureCache::Instance().Get("Texture/Bullet/Bullet.png").get(), 0, 0, &rect, &Math::Color{ 1,1,1,1 });

}