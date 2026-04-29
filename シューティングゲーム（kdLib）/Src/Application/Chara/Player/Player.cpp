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
#include"../../TextureCache/TextureCache.h"
#include"../../Light/Light.h"
#include"../../Animtion/ChargeAnim/ChargeAnim.h"

//初期化
void Player::Init()
{
	m_pos = { -300,50 };
	m_move = { 0,0 };
	m_moveSpeed = 300;
	m_hpMax = 100;
	m_hp = m_hpMax;
	m_scale = 0.5f;
	m_color = { 1,1,1,1 };

	//無敵時間の初期化
	m_bInvincible = false;
	m_invincibleTime = 0;
	m_sumDeltaTime = 0;
	

	m_shotWait = 0.4f;
	m_chargeShotWait = 0.7f;		//チャージショットのクールタイム
	//次の弾を撃つまでに一秒経ってから（シーン切り替え後すぐに弾を撃つのは少しおかしいから）
	m_shotWaitTimer = 1.0f;

	//アニメーションの初期化
	m_animCnt = 0;
	m_animSpeed = 4.0f;
	m_animCntMax = 10;

	//プレイヤーの明るさを変化させる用
	m_deg = 0;
	m_deltaDeg = 20;
	m_alphaMax = 0.2f;

	m_gapPos_chargeBullet=50;		//チャージ弾との距離

	//プレイヤーの大きさを求める（半径）
	KdTexture* tex = TextureCache::Instance().Get("Texture/Player/Player0.png").get();
	m_radius.x = tex->GetInfo().Width / 2.0f * m_scale;
	m_radius.y = tex->GetInfo().Height / 2.0f * m_scale;

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
		if (m_animCnt >= m_animCntMax)
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

	//チャージアニメーションの更新
	if (m_chargeAnim)
		m_chargeAnim->Update(deltaTime);

	m_deg += m_deltaDeg * deltaTime;
	if (m_deg >= 180 || m_bInvincible)m_deg = 0;

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
		if (m_chargeAnim)
			m_chargeAnim->Draw();

		SHADER.m_spriteShader.SetMatrix(m_mat);
		char path[100];
		sprintf_s(path, sizeof(path), "Texture/Player/Player%d.png", (int)m_animCnt);
		SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get(path), m_color);
		if (!m_bInvincible)
		{
			float alpha = 0.2f + fabs(sinf(DirectX::XMConvertToRadians(m_deg) * m_alphaMax));
			Light::Instance().Draw(m_pos, m_radius, Math::Color{ 1.0f,1.0f,1.0f,alpha });
		}
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

	//移動方向を決める
	{
		Math::Vector2 vec;
		//キー判定
		if (KEY.IsHeld(VK_LEFT) || KEY.IsHeld('A'))
		{
			vec.x = -1;
		}
		else if (KEY.IsHeld(VK_RIGHT) || KEY.IsHeld('D'))
		{
			vec.x = 1;
		}
		else
		{
			m_move.x = 0;
		}

		if (KEY.IsHeld(VK_UP) || KEY.IsHeld('W'))
		{
			vec.y = 1;
		}
		else if (KEY.IsHeld(VK_DOWN) || KEY.IsHeld('S'))
		{
			vec.y = -1;
		}
		else
		{
			m_move.y = 0;
		}

		//移動量を決める
		vec.Normalize();
		m_move = vec * m_moveSpeed;
	}
	//チャージ中は移動量0.7f
	if (m_chargeBullet)
	{
		m_move *= 0.7f;
	}

	//弾
	m_shotWaitTimer -= deltaTime;
	if (m_shotWaitTimer < 0)m_shotWaitTimer = 0;
	if (m_shotWaitTimer == 0)
	{
		//クリックしていないとき
		if (!KEY.IsHeld(VK_LBUTTON))
		{
			if (!m_chargeBullet)
			{
				//速射弾を発射
				Math::Vector2 startPos = { m_pos.x + m_radius.x, m_pos.y };
				Math::Vector2 targetPos = { m_pos.x + SCREEN_WIDTH,m_pos.y };
				Math::Vector2 beforeScale = { 0.7f,0.7f };
				Math::Vector2 afterScale = { 0.4f,0.4f };
				Math::Color color = { randRange(0.0f,0.6f),randRange(0.0f,0.6f),randRange(0.0f,0.6f),randRange(0.3f,0.4f) };
				m_pFireworksManager->Shot((FireworksManager::Type)1, startPos, targetPos, m_bulletSpeed, beforeScale, afterScale, color);
				
				//クールタイム
				m_shotWaitTimer = m_shotWait;				
			}
			else
			{
				//チャージしたものを発射
				m_chargeBullet->SetSpeed(m_chargeSpeedMax);
				float power = (m_chargeTime / 2.0f) * 10;
				
				//パワーが５を超えたら
				if (power > m_chargePowerMax)
				{
					power = m_chargePowerMax;
				}
				m_chargeBullet->SetPower(power);
				m_chargeBullet->Shot();
				//クールタイム
				m_shotWaitTimer = m_chargeShotWait;				
				//プレイヤークラスではチャージした弾の情報は保有しない（花火管理クラスに譲渡）
				m_chargeBullet = nullptr;
				m_chargeAnim = nullptr;
				
			}
		}
		//クリックしたとき
		else
		{
			//チャージ中の弾があるなら
			if (m_chargeBullet)
			{
				m_chargeTime += deltaTime;

				m_chargeBullet->SetPos({ m_pos.x + m_gapPos_chargeBullet, m_pos.y });
				m_chargeBullet->SetTargetPos({ m_pos.x + 800, m_pos.y });

				if (m_chargeTime < m_chargeTimeMax)
				{
					m_chargeBullet->SetBeforeScale(m_chargeBullet->GetBeforeScale() * (1.0 + deltaTime));
					m_chargeBullet->SetAfterScale(m_chargeBullet->GetAfterScale() * (1.0 + deltaTime));
					//透明度が小さかったら、透明度を上げる
					Math::Color color = m_chargeBullet->GetColor();
					if (color.A() < 0.5f)
					{
						color.A(color.A() + deltaTime * 0.1f);
					}
					m_chargeBullet->SetColor(color);
				}

				//チャージアニメーションの中心座標をプレイヤーの動きに追従させる
				m_chargeAnim->SetPos({ m_pos.x + m_gapPos_chargeBullet, m_pos.y });
			}
			//なかったら新しく作る
			else
			{
				m_chargeBullet = std::make_shared<Fireworks3>();
				m_chargeBullet->Init();

				Math::Vector2 startPos = { m_pos.x + m_radius.x, m_pos.y };
				Math::Vector2 targetPos = { m_pos.x + 800,m_pos.y };
				float speed = 400;
				Math::Vector2 beforeScale = { 0.7f,0.7f };
				Math::Vector2 afterScale = { 0.4f,0.4f };
				Math::Color color = { randRange(0.0f,0.5f),randRange(0.0f,0.5f),randRange(0.0f,0.5f),randRange(0.3f,0.5f) };
				m_chargeBullet->StartCharge(startPos, targetPos, beforeScale, afterScale, color);
				m_chargeTime = 0;

				//花火管理クラスに情報を登録（チャージ中はプレイヤーが値を変更、管理クラスが描画、更新処理を行う）
				m_pFireworksManager->Wait(m_chargeBullet);

				m_chargeAnim = std::make_shared<ChargeAnim>();
				m_chargeAnim->Init();
				m_chargeAnim->Start({ m_pos.x + m_gapPos_chargeBullet, m_pos.y }, { 70,70 }, 200, 350);
			}
		}
	}
}

//倒れた時のアニメーション用の値を初期化
void Player::InitDeadAnim()
{

	char path[100];
	sprintf_s(path, sizeof(path), "Texture/Player/Player%d.png", (int)m_animCnt);
	KdTexture* tex = TextureCache::Instance().Get(path).get();
	int height =tex->GetInfo().Height;
	int width = tex->GetInfo().Width;

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
	char path[100];
	sprintf_s(path, sizeof(path), "Texture/Player/Player%d.png", (int)m_animCnt);
	KdTexture* pTex = TextureCache::Instance().Get(path).get();
	int height = pTex->GetInfo().Height;
	int width = pTex->GetInfo().Width;
	tex.CreateRenderTarget(width, height);
	tex.ClearRenderTarget(Math::Color{ 0,0,0,0 });
	tex.SetRenderTarget();

	for (int h=0;h<m_bDraw.size();h++)
	{
		for (int w = 0; w < m_bDraw[h].size(); w++)
		{
			//ランダムで破壊する(描画させなくする)
			if (m_bDraw[h][w])
			{
				if (HitGacha(m_deadTimer / 5.0f))
				{
					m_bDraw[h][w] = false;
				}
			}

			//描画フラグが立っていたら
			if (m_bDraw[h][w])
			{
				SHADER.m_spriteShader.SetMatrix_Pos({ SCREEN_LEFT + 0.5f + w,SCREEN_TOP - 0.5f - h });
				SHADER.m_spriteShader.DrawTex(pTex, 0, 0, &Math::Rectangle{ w,h,1,1 }, &Math::Color{ 1,1,1,1 });
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
