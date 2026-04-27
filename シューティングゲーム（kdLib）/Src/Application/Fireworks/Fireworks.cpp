#include"Fireworks.h"
#include"../Tools/RandEx/RandEx.h"
#include"../Tools/Collision/Collision.h"
#include"../main.h"
#include"../TextureCache/TextureCache.h"

//===================================================
//花火１（円のみで描画）
//===================================================

//初期化
void Fireworks1::Init()
{
	if (!m_tex)
	{
		m_texRadius = 150;
		m_tex = std::make_shared<KdTexture>();
		m_tex->CreateRenderTarget(m_texRadius * 2, m_texRadius * 2);
	}
	m_texRadius = 100;
	m_tex->CreateRenderTarget(m_texRadius * 2, m_texRadius * 2);

	//花火の火の部分を作成
	//円
	while (m_circleList.size() < 5)
	{
		m_circleList.emplace_back();
	}

	//花火の当たり判定
	m_radius = 10;
}

//更新
void Fireworks1::Update(float deltaTime)
{
	//弾けているときは動かない
	if (!m_bExploded)
	{
		//座標更新
		m_pos += m_move * deltaTime;

		//ターゲット位置に行ったら
		if (m_bTarget)
		{
			if (IsCollision(m_pos, m_radius, m_targetPos, m_radius))
			{
				//花火を弾けさせる
				Explode();
			}
		}
	}

	//画面外にいったら
	/*if(IsScreenOut())
	{
		m_circleList.clear();
		m_bActive = false;
		m_bExploded = false;
		return;
	}*/

	//花火の火の更新
	for (int i = m_circleList.size() - 1; i >= 0; i--)
	{
		//弾けているときは減速処理を行う
		if (m_bExploded)
		{
			m_circleList[i].m_move *= 1.0f - (0.98 * deltaTime);
		}
		//寿命が尽きたら削除
		if (!m_circleList[i].Update(deltaTime))
		{
			//配列を入れ替えてpop_backで消去する（配列の途中を削除すると、詰めていく操作が行われるため）
			Particle temp = m_circleList[i];
			m_circleList[i] = m_circleList.back();
			m_circleList.back() = temp;
			//消去
			m_circleList.pop_back();
		}
	}


	//火が全てなくなったら非活性状態にする
	if (m_circleList.empty())
	{
		m_bActive = false;
	}
}

//描画
void Fireworks1::Draw()
{
	//描画先の初期化
	m_tex->ClearRenderTarget(Math::Color{ 0,0,0,0});
	m_tex->SetRenderTarget();

	//行列をリセット
	SHADER.m_spriteShader.ClearMatrix();
	for (auto& p : m_circleList)
	{
		SHADER.m_spriteShader.DrawCircle(p.m_pos.x, p.m_pos.y, 5, &p.m_color, true);
		SHADER.m_spriteShader.DrawCircle(p.m_pos.x, p.m_pos.y, 6, &p.m_color, true);
	}

	//描画先をBuckBufferに戻す
	D3D.SetBackBuffer();

	//行列作成
	Math::Matrix scale, rotation, trans, mat;
	if (!m_bExploded)
	{
		scale = Math::Matrix::CreateScale(m_beforeScale.x, m_beforeScale.y, 0);
	}
	else
	{
		scale = Math::Matrix::CreateScale(m_afterScale.x, m_afterScale.y, 0);
	}
	trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);

	//360°を10個に割り、回転させて描画
	for (int i = 0; i < 10; i++)
	{
		rotation = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(360 / 10 * i));
		SHADER.m_spriteShader.SetMatrix(scale * rotation * trans);
		SHADER.m_spriteShader.DrawTex_Src(m_tex.get());
	}
}

//花火を撃つ
void Fireworks1::Shot(Math::Vector2 startPos, Math::Vector2 targetPos, float speed, Math::Vector2 beforeScale, Math::Vector2 afterScale, Math::Color color, const bool bTarget)
{
	m_pos = startPos;
	m_targetPos = targetPos;
	m_speed = speed;
	m_beforeScale = beforeScale;
	m_afterScale = afterScale;
	m_color = color;
	m_bTarget = bTarget;

	//花火を活性状態にする
	m_bActive = true;

	//打ち上げる角度を求める
	float radian = atan2f(targetPos.y - startPos.y, targetPos.x - startPos.x);
	//角度を元に移動量を求める
	m_move.x = cosf(radian) * m_speed;
	m_move.y = sinf(radian) * m_speed;
	m_bExploded = false;

	//パーティクルを描画する先の中心座標を求める
	Math::Vector2 texPos;
	texPos = { SCREEN_LEFT + m_texRadius,SCREEN_TOP - m_texRadius };

	//円(火)の初期化
	for (auto& p : m_circleList)
	{
		//座標を中心から少しずらす
		p.m_pos = {
			texPos.x + randRange(-3.5f,3.5f),
			texPos.y + randRange(-3.5f,3.5f)
		};

		//移動量を少しだけ変化させる
		p.m_move = {
			randRange(-3.0f,3.0f),
			randRange(-3.0f,3.0f)
		};
		//色も少し変える
		float r = m_color.R() + randRange(-0.1f, 0.1f);
		float g = m_color.G() + randRange(-0.1f, 0.1f);
		float b = m_color.B() + randRange(-0.1f, 0.1f);
		float a = m_color.A() + randRange(-0.1f, 0.1f);
		p.m_color = { r,g,b,a };

		//寿命は途中（爆発するまでに）で尽きることの無いように
		p.m_life = 10;
	}
}

//花火を弾けさせる
void Fireworks1::Explode()
{
	//爆発したフラグを立てる
	m_bExploded = true;

	//円（火）をランダムに発射
	for (auto& p : m_circleList)
	{
		//移動量
		Math::Vector2 move = { randRange(0.0f,100.0f),randRange(0.0f,100.0f) };
		if (rand() % 2)
			move.x *= -1;
		if (rand() % 2)
			move.y *= -1;
		p.m_move = move;

		//寿命
		p.m_life = randRange(0.45f, 0.6f);		//0.45秒～0.6秒
	}
}

//解放
void Fireworks1::Release()
{
	if (!m_circleList.empty())m_circleList.clear();
}

//===================================================

//===================================================
//花火２（円と棒で描画）
//===================================================

//初期化
void Fireworks2::Init()
{
	if (!m_tex)
	{
		m_texRadius = 150;
		m_tex = std::make_shared<KdTexture>();
		m_tex->CreateRenderTarget(m_texRadius * 2, m_texRadius * 2);
	}

	//花火の火の部分を作成
	//円形
	while (m_circleList.size() < 5)
	{
		m_circleList.emplace_back();
	}

	//棒形
	while (m_lineList.size() < 30)
	{
		m_lineList.emplace_back();
	}

	m_bExploded = false;
	m_bActive = false;

	//花火の当たり判定
	m_radius = 10;
}

//更新
void Fireworks2::Update(float deltaTime)
{
	//弾けているときは動かない
	if (!m_bExploded)
	{
		//座標更新
		m_pos += m_move * deltaTime;

		//ターゲット位置に行ったら
		if (m_bTarget)
		{
			if (IsCollision(m_pos, m_radius, m_targetPos, m_radius))
			{
				//花火を弾けさせる
				Explode();
			}
		}
	}

	//画面外にいったら
	/*if(IsScreenOut())
	{
		m_circleList.clear();
		m_bActive = false;
		m_bExploded = false;
		return;
	}*/

	//花火の火の更新
	//円形
	for (auto& p : m_circleList)
	{
		//弾けているときは減速処理を行う
		if (m_bExploded)
		{
			p.m_move *= 1.0f - (0.98 * deltaTime);
		}
		//寿命が尽きたら削除
		if (!p.Update(deltaTime))
		{
			//配列を入れ替えてpop_backで消去する（配列の途中を削除すると、詰めていく操作が行われるため）
			Particle temp = p;
			p = m_circleList.back();
			m_circleList.back() = temp;
			//消去
			m_circleList.pop_back();
		}
	}

	//花火の火の更新
	//棒形
	if (m_bExploded)
	{
		for (auto& p : m_lineList)
		{
			//弾けているときは減速処理を行う
			if (m_bExploded)
			{
				p.m_move *= 1.0f - (0.95 * deltaTime);
			}
			//寿命が尽きたら削除
			if (!p.Update(deltaTime))
			{
				//配列を入れ替えてpop_backで消去する（配列の途中を削除すると、詰めていく操作が行われるため）
				Particle2 temp = p;
				p = m_lineList.back();
				m_lineList.back() = temp;
				//消去
				m_lineList.pop_back();
			}
		}
	}

	//火が全てなくなったら非活性状態にする
	if (m_circleList.empty())
	{
		m_bActive = false;
	}
}

//描画
void Fireworks2::Draw()
{
	//描画先の初期化
	m_tex->ClearRenderTarget(Math::Color{ 0,0,0,0 });
	m_tex->SetRenderTarget();

	//行列をリセット
	SHADER.m_spriteShader.ClearMatrix();

	//円形の描画
	for (auto& p : m_circleList)
	{
		SHADER.m_spriteShader.DrawCircle(p.m_pos.x, p.m_pos.y, 5, &p.m_color, true);
		SHADER.m_spriteShader.DrawCircle(p.m_pos.x, p.m_pos.y, 6, &p.m_color, true);
	}

	//棒形の描画
	if (m_bExploded)
	{
		for (auto& p : m_lineList)
		{
			SHADER.m_spriteShader.SetMatrix(p.m_mat);
			SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/Bullet3.png"), p.m_color);
		}
	}

	//描画先をBuckBufferに戻す
	D3D.SetBackBuffer();

	//行列作成
	Math::Matrix scale, rotation, trans, mat;
	if (!m_bExploded)
	{
		scale = Math::Matrix::CreateScale(m_beforeScale.x, m_beforeScale.y, 0);
	}
	else
	{
		scale = Math::Matrix::CreateScale(m_afterScale.x, m_afterScale.y, 0);
	}
	trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);

	//360°を10個に割り、回転させて描画
	for (int i = 0; i < 10; i++)
	{
		rotation = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(360 / 10 * i));
		SHADER.m_spriteShader.SetMatrix(scale * rotation * trans);
		SHADER.m_spriteShader.DrawTex_Src(m_tex.get());
	}
}

//花火を撃つ
void Fireworks2::Shot(Math::Vector2 startPos, Math::Vector2 targetPos, float speed, Math::Vector2 beforeScale, Math::Vector2 afterScale, Math::Color color, const bool bTarget)
{
	m_pos = startPos;
	m_targetPos = targetPos;
	m_speed = speed;
	m_beforeScale = beforeScale;
	m_afterScale = afterScale;
	m_color = color;
	m_bTarget = bTarget;

	//花火を活性状態にする
	m_bActive = true;

	//打ち上げる角度を求める
	float radian = atan2f(targetPos.y - startPos.y, targetPos.x - startPos.x);
	//角度を元に移動量を求める
	m_move.x = cosf(radian) * m_speed;
	m_move.y = sinf(radian) * m_speed;
	m_bExploded = false;

	//================================================
	//花火の火の部分を作成
	//================================================
	
	//パーティクルを描画する先の中心座標を求める
	Math::Vector2 texPos;			
	texPos = { SCREEN_LEFT + m_texRadius,SCREEN_TOP - m_texRadius };

	//円(火)の初期化
	for (auto& p : m_circleList)
	{
		//座標を中心から少しずらす
		p.m_pos = {
			texPos.x + randRange(-3.5f,3.5f),
			texPos.y + randRange(-3.5f,3.5f)
		};

		//移動量を少しだけ変化させる
		p.m_move = {
			randRange(-3.0f,3.0f),
			randRange(-3.0f,3.0f)
		};
		//色も少し変える
		float r = m_color.R() + randRange(-0.1f, 0.1f);
		float g = m_color.G() + randRange(-0.1f, 0.1f);
		float b = m_color.B() + randRange(-0.1f, 0.1f);
		float a = m_color.A() + randRange(-0.1f, 0.1f);
		p.m_color = { r,g,b,a };

		//寿命は途中（爆発するまでに）で尽きることの無いように
		p.m_life = 10;
	}
	//================================================
}

//花火を弾けさせる
void Fireworks2::Explode()
{
	//爆発したフラグを立てる
	m_bExploded = true;

	//円形（火）をランダムに発射
	for (auto& p : m_circleList)
	{
		//移動量
		Math::Vector2 move = { randRange(0.0f,100.0f),randRange(0.0f,100.0f) };
		if (rand() % 2)
			move.x *= -1;
		if (rand() % 2)
			move.y *= -1;
		p.m_move = move;

		//寿命
		p.m_life = randRange(0.45f, 0.6f);		//0.45秒～0.6秒
	}

	//棒形（火）をランダムに発射
	for (auto& p : m_lineList)
	{
		//パーティクルを描画する先の中心座標を求める
		Math::Vector2 texPos;
		texPos = { SCREEN_LEFT + m_texRadius,SCREEN_TOP - m_texRadius };

		//座標
		p.m_pos = texPos;

		//移動量
		Math::Vector2 move = { randRange(0.0f,150.0f),randRange(0.0f,150.0f) };
		if (rand() % 2)
			move.x *= -1;
		if (rand() % 2)
			move.y *= -1;
		p.m_move = move;

		p.m_radian = atan2(p.m_move.y, p.m_move.x);

		//色も少し変える
		float r = m_color.R() + randRange(-0.1f, 0.1f);
		float g = m_color.G() + randRange(-0.1f, 0.1f);
		float b = m_color.B() + randRange(-0.1f, 0.1f);
		float a = m_color.A() + randRange(-0.1f, 0.1f);
		p.m_color = { r,g,b,a };

		//寿命
		p.m_life = randRange(0.5f, 0.8f);		//0.5秒～0.8秒
	}
}

//解放
void Fireworks2::Release()
{
	if (!m_circleList.empty())m_circleList.clear();
	if (!m_lineList.empty())m_lineList.clear();
}

//===================================================

//初期化
void Fireworks3::Init()
{
	while (m_fireworksList.size() < m_power)
	{
		if (rand() % 2)
		{
			m_fireworksList.push_back(std::make_shared<Fireworks1>());
			m_fireworksList.back()->Init();
		}
		else
		{
			m_fireworksList.push_back(std::make_shared<Fireworks2>());
			m_fireworksList.back()->Init();
		}
	}

	
}

//更新
void Fireworks3::Update(float deltaTime)
{
	if (!m_bExplodedFlg)
	{
		m_pos += m_move * deltaTime;
	}
	for (int i = m_fireworksList.size() - 1; i >= 0; i--)
	{
		m_fireworksList[i]->Update(deltaTime);
		if (!m_fireworksList[i]->IsActive())
		{
			m_fireworksList.erase(m_fireworksList.begin() + i);
		}
	}

	if (m_fireworksList.empty())
	{
		m_bActive = false;
	}
}

//描画
void Fireworks3::Draw()
{
	for (auto& f : m_fireworksList)
	{
		if(f->IsActive())
		{
			f->Draw();
		}
	}
}

//チャージを始める
void Fireworks3::StartCharge(Math::Vector2 startPos, Math::Vector2 targetPos, Math::Vector2 beforeScale, Math::Vector2 afterScale, Math::Color color)
{
	m_pos = startPos;
	m_targetPos = targetPos;
	m_beforeScale = beforeScale;
	m_afterScale = afterScale;
	m_color = color;
	m_power = 1;

	m_speed = 0;

	m_bExplodedFlg = false;
	m_bChargeFlg = true;
	m_bActive = true;

	//花火を作成する
	Create();
	m_fireworksList.back()->Shot(m_pos, m_targetPos, m_speed, m_beforeScale, m_afterScale, m_color);
}

//花火を撃つ
void Fireworks3::Shot()
{
	//当たり判定
	m_radius = 20;

	//チャージフラグをオフに
	m_bChargeFlg = false;

	m_move.x = m_speed;
	m_move.y = 0;

	//一つ目の花火は決められたもの
	m_fireworksList[0]->Shot(m_pos, m_targetPos, m_speed, m_beforeScale, m_afterScale, m_color);

	//二つ目以降の花火は色サイズ変更
	for (int i = 1; i < m_fireworksList.size(); i++)
	{

	}
}

//花火を弾けさせる
void Fireworks3::Explode()
{
	for (auto& p : m_fireworksList)
	{
		p->Explode();
	}

	m_bExplodedFlg = true;
}

//貫通する
void Fireworks3::Pierce(Math::Vector2 pos)
{
	//パワーを減らす
	m_power--;
	if ((int)m_power <= 0)
	{
		m_power = 0;
		Explode();
	}
}

//花火を作成する
void Fireworks3::Create()
{
	if (rand() % 2)
	{
		m_fireworksList.push_back(std::make_shared<Fireworks1>());
		m_fireworksList.back()->Init();

	}
	else
	{
		m_fireworksList.push_back(std::make_shared<Fireworks2>());
		m_fireworksList.back()->Init();
	}
}

//解放
void Fireworks3::Release()
{
	m_fireworksList.clear();
}

