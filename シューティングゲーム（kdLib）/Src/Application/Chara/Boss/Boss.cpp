#include "Boss.h"
#include"../../TextureCache/TextureCache.h"
#include"../../Bullet/BulletManager.h"

std::shared_ptr<BulletManager> Boss::m_pBulletManager = nullptr;

void Boss::Init()
{
	m_pos = {};				//座標
	m_move = {};				//移動量
	m_moveSpeed = 200;		//移動スピード
	m_radius = { 64,64 };			//半径（当たり判定、画面端判定）

	std::shared_ptr<KdTexture> tex = TextureCache::Instance().Get("Texture/Boss/Boss0.png");
	float width = tex->GetInfo().Width;

	m_scale = { m_radius.x * 2 / width };			//画像の拡縮
	
	m_color = { 1,1,1,1 };			//色
	
	m_hpMax = 1000;			//最大体力
	m_hp = m_hpMax;			//体力

	
	m_animCnt = 0;			//アニメーションカウンター
	m_animCntMax = 8;		//アニメーションの終わり（何コマか？）
	m_animSpeed = 5.0f;		//アニメーションスピード

	m_shotWaitTimer = 1;	//撃つまでのクールタイムを測る
	m_shotWait = 1;			//クールタイム
	m_shotCnt = 0;
	m_shotCntMax = 5;		//通常弾を５回まで連続で撃てる

	m_circleShotWait = 0.8f;
	m_circleShotWaitTimer = 5.0f;
	m_circleShotCnt = 0;
	m_circleShotCntMax = 3;	//３６０°に飛ばす弾を連続で３回撃てる
}

void Boss::Update(float deltaTime)
{
	//ショットのクールタイム
	m_shotWaitTimer -= deltaTime;
	if (m_shotWaitTimer < 0)m_shotWaitTimer = 0;

	m_circleShotWaitTimer -= deltaTime;
	if (m_circleShotWaitTimer < 0)m_shotWaitTimer = 0;


	//ボスの行動
	Action(deltaTime);

	//アニメーションの更新
	m_animCnt += m_animSpeed * deltaTime;
	if (m_animCnt >= m_animCntMax)m_animCnt = 0;

	m_pos += m_move * deltaTime;
	Math::Matrix scale = Math::Matrix::CreateScale(m_scale, m_scale, 0);
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scale * trans;
}

void Boss::Draw()
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	char path[50];
	sprintf_s(path, sizeof(path), "Texture/Boss/Boss%d.png", (int)m_animCnt);
	SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get(path), m_color);
}

void Boss::Action(float deltaTime)
{
	if (m_shotWaitTimer == 0)
	{
		m_pBulletManager->Shot({ m_pos.x - m_radius.x,m_pos.y }, { -100,0 });
		m_shotWaitTimer = m_shotWait;
		m_shotCnt++;
	}
	if (m_circleShotWaitTimer == 0)
	{
		m_pBulletManager->Shot({ m_pos.x - m_radius.x,m_pos.y }, { -100,0 });
		m_circleShotWaitTimer = m_circleShotWait;
		m_circleShotCnt++;
	}
}

void Boss::Spawn(Math::Vector2 pos,Math::Vector2 move)
{
	m_pos = pos;
	m_move = move;
}

void Boss::Dead()
{

}

void Boss::Release()
{
}
