#include"Fireworks.h"
#include"../Tools/RandEx/RandEx.h"
#include"../Tools/Collision/Collision.h"
#include"../main.h"
#include"../TextureCache/TextureCache.h"
#include"../Light/Light.h"

//===================================================
//花火１（円のみで描画）
//===================================================

//初期化
void Fireworks1::Init()
{
	m_texRadius = 150;
	m_tex = std::make_shared<KdTexture>();
	m_tex->CreateRenderTarget(m_texRadius * 2, m_texRadius * 2);

	//花火の火の部分を作成
	//円
	while (m_circleList.size() < 5)
	{
		m_circleList.emplace_back();
	}
}

//更新
void Fireworks1::Update(float deltaTime)
{
	//弾けているときは動かない
	if (!m_bExploded)
	{
		//座標更新
		m_pos += m_move * deltaTime;

		//重力
		m_move.y -= m_gravity * deltaTime;

		//十分に減速したら
		if (m_move.y < 20)
		{
			//花火を弾けさせる
			Explode();
		}
	}

	if (m_bDead_ScreenOut)
	{
		//画面外にいったら
		if (IsScreenOut())
		{
			m_circleList.clear();
			m_bActive = false;
			m_bExploded = true;
			return;
		}
	}
	if (m_bDead_ScreenOut)
	{
		//画面外にいったら
		if (IsScreenOut())
		{
			m_circleList.clear();
			m_bActive = false;
			m_bExploded = true;
			return;
		}
	}
	else
	{
		if (IsCollision_Box(m_pos, {}, {}, { SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f }))
		{
			m_bDead_ScreenOut = true;
		}
	}

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
		scale = Math::Matrix::CreateScale(m_beforeScale);
	}
	else
	{
		scale = Math::Matrix::CreateScale(m_afterScale);
	}
	trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);

	//360°を10個に割り、回転させて描画
	for (int i = 0; i < 10; i++)
	{
		rotation = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(360 / 10 * i));
		SHADER.m_spriteShader.SetMatrix(scale * rotation * trans);
		SHADER.m_spriteShader.DrawTex_Src(m_tex);
		SHADER.m_spriteShader.DrawTex_Src(m_tex, Math::Color{ 0.8f,0.8f,0.8f,1 });
	}
}

//花火を撃つ
void Fireworks1::Shot(Math::Vector2& startPos, Math::Vector2& startMove, float beforeScale, float afterScale, Math::Color& color)
{
	m_pos = startPos;
	m_move = startMove;
	m_beforeScale = beforeScale;
	m_afterScale = afterScale;
	m_color = color;

	//花火を活性状態にする
	m_bActive = true;

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

		//移動量を0
		p.m_move = {};
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
	m_texRadius = 150;
	m_tex = std::make_shared<KdTexture>();
	m_tex->CreateRenderTarget(m_texRadius * 2, m_texRadius * 2);

	//花火の火の部分を作成
	//円形
	while (m_circleList.size() < 5)
	{
		m_circleList.emplace_back();
	}

	//棒形
	while (m_lineList.size() < 5)
	{
		m_lineList.emplace_back();
	}

	m_bExploded = false;
	m_bActive = false;

	m_lineBaseScale = { 0.1f,0.1f };
}

//更新
void Fireworks2::Update(float deltaTime)
{
	//弾けているときは動かない
	if (!m_bExploded)
	{
		//座標更新
		m_pos += m_move * deltaTime;

		//重力
		m_move.y -= m_gravity * deltaTime;

		//十分に減速したら
		if (m_move.y < 20)
		{
			//花火を弾けさせる
			Explode();
		}
	}

	if (m_bDead_ScreenOut)
	{
		//画面外にいったら
		if (IsScreenOut())
		{
			m_circleList.clear();
			m_lineList.clear();
			m_bActive = false;
			m_bExploded = true;
			return;
		}
	}
	else
	{
		if (IsCollision_Box(m_pos, {}, {}, { SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f }))
		{
			m_bDead_ScreenOut = true;
		}
	}

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
		scale = Math::Matrix::CreateScale(m_beforeScale);
	}
	else
	{
		scale = Math::Matrix::CreateScale(m_afterScale);
	}
	trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);

	//360°を10個に割り、回転させて描画
	for (int i = 0; i < 10; i++)
	{
		rotation = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(360 / 10 * i));
		SHADER.m_spriteShader.SetMatrix(scale * rotation * trans);
		SHADER.m_spriteShader.DrawTex_Src(m_tex);
		SHADER.m_spriteShader.DrawTex_Src(m_tex, Math::Color{ 0.8f,0.8f,0.8f,1 });

	}
}

//花火を撃つ
void Fireworks2::Shot(Math::Vector2& startPos, Math::Vector2& startMove, float beforeScale, float afterScale, Math::Color& color)
{
	m_pos = startPos; 
	m_move = startMove;
	m_beforeScale = beforeScale;
	m_afterScale = afterScale;
	m_color = color;

	//花火を活性状態にする
	m_bActive = true;

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

		//移動量を0
		p.m_move = {};
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

		p.m_scale.x = m_lineBaseScale.x + randRange(-0.05f, 0.05f);
		p.m_scale.y = m_lineBaseScale.y + randRange(-0.05f, 0.05f);

		//寿命
		p.m_life = randRange(0.5f, 0.8f);		//0.5秒～0.8秒
	}
}

//===================================================

//===================================================
//花火3（円だけで描画)
//===================================================

void Fireworks3::Init()
{
	m_texRadius = 200;
	m_tex = std::make_shared<KdTexture>();
	m_tex->CreateRenderTarget(m_texRadius * 2, m_texRadius * 2);

	//花火の火の部分を作成
	while (m_circleList.size() < 250)
	{
		m_circleList.emplace_back();
	}

	m_bExploded = false;
	m_bActive = false;
}

void Fireworks3::Update(float deltaTime)
{
	//弾けているときは動かない
	if (!m_bExploded)
	{
		//座標更新
		m_pos += m_move * deltaTime;

		//重力
		m_move.y -= m_gravity * deltaTime;

		//十分に減速したら
		if (m_move.y < 20)
		{
			//花火を弾けさせる
			Explode();
		}
	}

	if (m_bDead_ScreenOut)
	{
		//画面外にいったら
		if (IsScreenOut())
		{
			m_circleList.clear();
			m_bActive = false;
			m_bExploded = true;
			return;
		}
	}
	else
	{
		if (IsCollision_Box(m_pos, {}, {}, { SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f }))
		{
			m_bDead_ScreenOut = true;
		}
	}

	//花火の火の更新
	//円形
	for (auto& p : m_circleList)
	{
		//弾けているときは減速処理を行う
		if (m_bExploded)
		{
			//空気抵抗的な減速
			p.m_move *= 1.0f - (0.95 * deltaTime);

			//重力
			p.m_move.y -= m_gravity * deltaTime;
		}
		//寿命が尽きて、透明度も０になったら削除
		if (!p.Update(deltaTime, m_baseScale))
		{
			//配列を入れ替えてpop_backで消去する（配列の途中を削除すると、詰めていく操作が行われるため）
			Particle3 temp = p;
			p = m_circleList.back();
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

void Fireworks3::Draw()
{
	//描画先の初期化
	m_tex->ClearRenderTarget(Math::Color{ 0,0,0,0 });
	m_tex->SetRenderTarget();

	//円の描画
	{
		std::shared_ptr<KdTexture>tex = TextureCache::Instance().Get("Texture/Fireworks/Particle2.png");
		for (auto& p : m_circleList)
		{
			for (int i = 0; i < 3; ++i)
			{
				p.Draw(tex);
			}
		}
	}


	//描画先をBuckBufferに戻す
	D3D.SetBackBuffer();

	//行列作成
	Math::Matrix scale, trans;
	if (!m_bExploded)
	{
		scale = Math::Matrix::CreateScale(m_beforeScale);
	}
	else
	{
		scale = Math::Matrix::CreateScale(m_afterScale);
	}
	trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);

	SHADER.m_spriteShader.SetMatrix(scale * trans);
	SHADER.m_spriteShader.DrawTex_Src(m_tex);
	SHADER.m_spriteShader.DrawTex_Src(m_tex, Math::Color{ 0.8f,0.8f,0.8f,1 });
}

void Fireworks3::Shot(Math::Vector2& startPos, Math::Vector2&startMove, float beforeScale, float afterScale, Math::Color& color)
{
	m_pos = startPos;
	m_move = startMove;
	m_beforeScale = beforeScale;
	m_afterScale = afterScale;
	m_color = color;

	//花火を活性状態にする
	m_bActive = true;

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

		//移動量を0
		p.m_move = {};

		//拡縮を少し変更
		p.m_scale = randRange(0.7f, 1.3f);

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

void Fireworks3::Explode()
{
	m_bExploded = true;

	Math::Vector2 texPos;
	Math::Vector2 vec;
	float radian;
	texPos = { SCREEN_LEFT + m_texRadius,SCREEN_TOP - m_texRadius };

	for (auto& p : m_circleList)
	{
		//座標を中心から少しずらす
		p.m_pos = {
			texPos.x + randRange(-3.5f,3.5f),
			texPos.y + randRange(-3.5f,3.5f)
		};

		//移動量を
		
		radian = DirectX::XMConvertToRadians(randRange(0, 360));
		vec.x = cosf(radian);
		vec.y = sinf(radian);
		vec.Normalize();
		p.m_move = vec * randRange(0, m_explodeSpeedMax);

		//拡縮を少し変更
		p.m_scale = 1.0 + randRange(-0.3f, 0.3f);

		//色も少し変える
		float r = m_color.R() + randRange(-0.1f, 0.1f);
		float g = m_color.G() + randRange(-0.1f, 0.1f);
		float b = m_color.B() + randRange(-0.1f, 0.1f);
		float a = m_color.A() + randRange(-0.1f, 0.1f);
		p.m_color = { r,g,b,a };

		p.m_life = randRange(0.8f, 1.2f);
	}
}

//===================================================

//===================================================
//花火4（花弁画像だけで描画）
//===================================================

//初期化
void Fireworks4::Init()
{
	m_texRadius = 150;
	m_tex = std::make_shared<KdTexture>();
	m_tex->CreateRenderTarget(m_texRadius * 2, m_texRadius * 2);

	while (m_petal.size() < 6)
	{
		m_petal.emplace_back();
	}

	m_bActive = true;
	m_bExploded = false;
}

//更新
void Fireworks4::Update(float deltaTime)
{
	//弾けているときは動かない
	if (!m_bExploded)
	{
		//座標更新
		m_pos += m_move * deltaTime;

		//重力
		m_move.y -= m_gravity * deltaTime;

		//十分に減速したら
		if (m_move.y < 20)
		{
			//花火を弾けさせる
			Explode();
		}
	}


	if (m_bDead_ScreenOut)
	{
		//画面外にいったら
		if (IsScreenOut())
		{
			m_petal.clear();
			m_bActive = false;
			m_bExploded = true;
			return;
		}
	}
	else
	{
		if (IsCollision_Box(m_pos, {}, {}, { SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f }))
		{
			m_bDead_ScreenOut = true;
		}
	}

	//花火の火の更新
	//円形
	for (auto& p : m_petal)
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
			p = m_petal.back();
			m_petal.back() = temp;
			//消去
			m_petal.pop_back();
		}
	}

	//火が全てなくなったら非活性状態にする
	if (m_petal.empty())
	{
		m_bActive = false;
	}
}

//描画
void Fireworks4::Draw()
{
	//描画先の初期化
	m_tex->ClearRenderTarget(Math::Color{0,0,0,0});
	m_tex->SetRenderTarget();

	//花弁の描画
	for (auto& p : m_petal)
	{
		for (int i = 0; i < 8; i++)
		{
			Math::Matrix scale = Math::Matrix::CreateScale(0.2f, 0.2f, 0);
			Math::Matrix trans = Math::Matrix::CreateTranslation(p.m_pos.x, p.m_pos.y, 0);
			SHADER.m_spriteShader.SetMatrix(scale * trans);
			SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/Petal.png"), p.m_color);
		}
	}

	//描画先をBuckBufferに戻す
	D3D.SetBackBuffer();

	//行列作成
	Math::Matrix scale, rotation, trans;
	if (!m_bExploded)
	{
		scale = Math::Matrix::CreateScale(m_beforeScale);
	}
	else
	{
		scale = Math::Matrix::CreateScale(m_afterScale);
	}
	trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);

	//360°を8個に割り、回転させて描画
	for (int i = 0; i < 8; i++)
	{
		rotation = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(360 / 8 * i));
		SHADER.m_spriteShader.SetMatrix(scale * rotation * trans);
		SHADER.m_spriteShader.DrawTex_Src(m_tex);
		SHADER.m_spriteShader.DrawTex_Src(m_tex, Math::Color{ 0.8f,0.8f,0.8f,1 });

	}
}

//花火を撃つ
void Fireworks4::Shot(Math::Vector2& startPos, Math::Vector2& startMove, float beforeScale, float afterScale, Math::Color& color)
{
	m_pos = startPos; 
	m_move = startMove;
	m_beforeScale = beforeScale;
	m_afterScale = afterScale;
	m_color = color;

	//花火を活性状態にする
	m_bActive = true;

	//パーティクルを描画する先の中心座標を求める
	Math::Vector2 texPos;
	texPos = { SCREEN_LEFT + m_texRadius,SCREEN_TOP - m_texRadius };

	//円(火)の初期化
	for (auto& p : m_petal)
	{
		//座標を中心から少しずらす
		p.m_pos = {
			texPos.x + randRange(-3.5f,3.5f),
			texPos.y + randRange(-3.5f,3.5f)
		};

		//移動量を0
		p.m_move = {};
		
		//色も少し変える
		float r = m_color.R() + randRange(-0.1f, 0.1f);
		float g = m_color.G() + randRange(-0.1f, 0.1f);
		float b = m_color.B() + randRange(-0.1f, 0.1f);
		float a = m_color.A() + randRange(-0.1f, 0.1f);
		if (a < 0.5f)a = 0.5f;
		p.m_color = { r,g,b,a };

		//寿命は途中（爆発するまでに）で尽きることの無いように
		p.m_life = 10;
	}
}

//花火を弾けさせる
void Fireworks4::Explode()
{
	//爆発したフラグを立てる
	m_bExploded = true;

	//円形（火）をランダムに発射
	for (auto& p : m_petal)
	{
		//移動量
		Math::Vector2 move = { randRange(0.0f,100.0f),randRange(0.0f,100.0f) };
		if (rand() % 2)
			move.x *= -1;
		if (rand() % 2)
			move.y *= -1;
		p.m_move = move;

		//寿命
		p.m_life = randRange(0.6f, 0.8f);		//0.45秒～0.6秒
	}
}

//解放
void Fireworks4::Release()
{
	m_petal.clear();
}

