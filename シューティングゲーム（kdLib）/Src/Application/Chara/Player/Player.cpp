#include"Player.h"
#include"../../Font/DrawString.h"
#include"../../Bullet/BulletManager.h"
#include"../../Key/KeyManager.h"
#include"../../Timer/Timer.h"
#include"../../Fireworks/FireworksManager.h"
#include"../../Mouse/Mouse.h"
#include"../../Tools/RandEx/RandEx.h"
#include"../../Scene/GameScene/GameScene.h"
#include"../../Fireworks/Fireworks.h"

//コンストラクタ
Player::Player()
{
	
}

void Player::Init(Game* g,FireworksManager* f)
{
	m_pGame = g;
	m_pFireworksManager = f;
	m_pos = { 0,0 };
	m_move = { 0,0 };
	m_hpMax = 100;
	m_hp = m_hpMax;
	m_scale = 0.5f;
	m_color = { 1,1,1,1 };
	m_bInvincible = false;
	m_invincibleTime = 0;
	m_sumDeltaTime = 0;
	m_animCnt = 0;
	m_animSpeed = 4.0f;

	for (int i = 0; i < 10; i++)
	{
		char filePath[100];
		sprintf_s(filePath, sizeof(filePath), "Texture/Player/Player%d.png", i);
		m_tex[i].Load(filePath);
	}
	//プレイヤーの大きさを求める（半径）
	m_radius.x = (m_tex[0].GetInfo().Width / 2.0f) * m_scale;
	m_radius.y = (m_tex[0].GetInfo().Height / 2.0f) * m_scale;

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
		else if (m_sumDeltaTime >= 0.15f)
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

	//アニメーションの更新
	if (m_hp > 0)
	{
		m_animCnt += deltaTime * m_animSpeed;
		if (m_animCnt >= 10)
		{
			m_animCnt = 0;
		}
	}
	//倒れたアニメーションの更新
	else
	{
		m_deadTimer += deltaTime;

		if (m_deadTimer >= 0.3f)
		{
			//ゲームオーバー状態にする
			m_pGame->GameOver();
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
	
	if (top > SCREEN_TOP)
	{
		m_pos.y = SCREEN_TOP - m_radius.y;
	}
	else if (bottom < SCREEN_BOTTOM)
	{
		m_pos.y = SCREEN_BOTTOM + m_radius.y;
	}


	////減速処理
	//m_move *= 1.0f - (4.0f * deltaTime);		//2.0の値はちょうどよい減速率にするため(毎フレーム0.98くらい？になりそう)

	//行列作成
	Math::Matrix scaleMat = Math::Matrix::CreateScale(m_scale, m_scale, 0);
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scaleMat* transMat;
}

//描画
void Player::Draw()
{
	//生きているとき
	if (m_hp > 0)
	{
		SHADER.m_spriteShader.SetMatrix(m_mat);
		SHADER.m_spriteShader.DrawTex_Src(&m_tex[(int)m_animCnt], m_color);
	}
	//倒れているとき
	else
	{
		DrawDeadAnim();
	}
}

//行動
void Player::Action(float deltaTime)
{
	if (m_hp <= 0)return;

	else if (KEY.IsHeld(VK_LEFT) || KEY.IsHeld('A'))
	{
		m_move.x -= 30.0f;
		if (m_move.x < s_speedMax)m_move.x = -s_speedMax;
	}
	else if (KEY.IsHeld(VK_RIGHT) || KEY.IsHeld('D'))
	{
		m_move.x += 30.0f;
		if (m_move.x > s_speedMax)m_move.x = s_speedMax;
	}
	else
	{
		m_move.x = 0;
	}

	if (KEY.IsHeld(VK_UP) || KEY.IsHeld('W'))
	{
		m_move.y += 30.0f;
		if (m_move.y > s_speedMax)m_move.y = s_speedMax;
	}
	else if (KEY.IsHeld(VK_DOWN) || KEY.IsHeld('S'))
	{
		m_move.y -= 30.0f;
		if (m_move.y < -s_speedMax)m_move.y = -s_speedMax;
	}
	else
	{
		m_move.y = 0;
	}


	//弾
	m_shotWait -= deltaTime;
	if (m_shotWait < 0)m_shotWait = 0;
	if (m_shotWait == 0)
	{
		//速射弾を発射
		if (!KEY.IsHeld(VK_LBUTTON))
		{
			if (!m_chargeBullet)
			{
				//速射
				Math::Vector2 startPos = { m_pos.x + m_radius.x, m_pos.y };
				Math::Vector2 targetPos = { m_pos.x + 800 ,m_pos.y };
				float speed = 400;
				Math::Vector2 beforeScale = { 0.7f,0.7f };
				Math::Vector2 afterScale = { 0.4f,0.4f };
				Math::Color color = { randRange(0.0f,0.6f),randRange(0.0f,0.6f),randRange(0.0f,0.6f),randRange(0.3f,0.4f) };
				m_pFireworksManager->Shot((FireworksManager::Type)1, startPos, targetPos, speed, beforeScale, afterScale, color);
				//撃つ待機時間を設ける
				m_shotWait = 0.3f;				//0.3秒
			}
			else
			{
				//チャージしたものを発射
				Math::Vector2 startPos = m_chargeBullet->GetPos();
				Math::Vector2 targetPos = m_chargeBullet->GetTargetPos();
				float speed = m_chargeBullet->GetSpeed();
				Math::Vector2 beforeScale = m_chargeBullet->GetBeforeScale();
				Math::Vector2 afterScale = m_chargeBullet->GetAfterScale();
				Math::Color color = m_chargeBullet->GetColor();

				m_pFireworksManager->Shot((FireworksManager::Type)1, startPos, targetPos, speed, beforeScale, afterScale, color);
				m_shotWait = 0.3f;			//0.3秒

				//プレイヤークラスではチャージした弾の情報は保有しない（花火管理クラスに譲渡）
				m_chargeBullet = nullptr;
			}
		}
		else
		{
			//チャージ中の弾があるなら
			if (m_chargeBullet)
			{
				m_chargeTime += deltaTime;
				m_chargeBullet->SetPos({ m_pos.x + m_radius.x * m_chargeBullet->GetBeforeScale().x, m_pos.y });
				m_chargeBullet->SetBeforeScale(m_chargeBullet->GetBeforeScale() * (1.0 + deltaTime));
				m_chargeBullet->SetAfterScale(m_chargeBullet->GetAfterScale() * (1.0 + deltaTime));
				Math::Color color = m_chargeBullet->GetColor();
				if (m_chargeTime < 1.0f)
				{
					if (color.A() < 0.5f)
					{
						color.A(color.A() + deltaTime * 0.1f);
					}
				}
				else
				{
					color *= (0.2f - deltaTime);
					color.A(0.3f);
				}
				m_chargeBullet->SetColor({ color.R(),color.G(),color.B(),color.A() });
			}
			//なかったら新しく作る
			else
			{
				m_chargeBullet = std::make_shared<Fireworks3>();
				m_chargeBullet->Init();

				Math::Vector2 startPos = { m_pos.x + m_radius.x, m_pos.y };
				Math::Vector2 targetPos = { 9999,m_pos.y };
				float speed = 400;
				Math::Vector2 beforeScale = { 0.7f,0.7f };
				Math::Vector2 afterScale = { 0.4f,0.4f };
				Math::Color color = { randRange(0.0f,0.5f),randRange(0.0f,0.5f),randRange(0.0f,0.5f),randRange(0.3f,0.5f) };
				m_pFireworksManager->Shot(m_chargeBullet);
				m_chargeTime = 0;
			}
		}
	}
}

//倒れた時のアニメーション用の値を初期化
void Player::InitDeadAnim()
{
	int height = m_tex[(int)m_animCnt].GetInfo().Height;
	int width = m_tex[(int)m_animCnt].GetInfo().Width;

	for (int h = 0; h < height; h++)
	{
		m_bDraw.emplace_back();
		for (int w = 0; w < width;w++)
		{
			m_bDraw[h].emplace_back(true);
		}
	}

	

	m_deadTimer = 0;
}

//倒れた時のアニメーション
void Player::DrawDeadAnim()
{
	KdTexture tex;
	tex.CreateRenderTarget(m_tex[(int)m_animCnt].GetInfo().Width, m_tex[(int)m_animCnt].GetInfo().Height);
	tex.ClearRenderTarget(Math::Color{ 0,0,0,0 });
	tex.SetRenderTarget();

	for (int h=0;h<m_bDraw.size();h++)
	{
		for (int w = 0; w < m_bDraw[h].size(); w++)
		{
			//ランダムで破壊する(描画させなくする)
			if (m_bDraw[h][w])
			{
				if (HitGacha(m_deadTimer / 20.0f))
				{
					m_bDraw[h][w] = false;
				}
			}

			//描画フラグが立っていたら
			if (m_bDraw[h][w])
			{
				SHADER.m_spriteShader.SetMatrix_Pos({ SCREEN_LEFT + 0.5f + w,SCREEN_TOP - 0.5f - h });
				SHADER.m_spriteShader.DrawTex(&m_tex[(int)m_animCnt], 0, 0, &Math::Rectangle{ w,h,1,1 }, &Math::Color{ 1,1,1,1 });
			}
		}
	}
	D3D.SetBackBuffer();

	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex_Src(&tex);

	

}

//倒れた時の処理
void Player::Dead()
{
	//移動量をなくす
	m_move = { 0,0 };
	Timer::Instance().Slow(10.0f, 0.3f);
	InitDeadAnim();
}
