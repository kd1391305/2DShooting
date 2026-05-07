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
	m_type = Type::Circle;

	m_texRadius = 200;
	m_tex = std::make_shared<KdTexture>();
	m_tex->CreateRenderTarget(m_texRadius * 2, m_texRadius * 2);

	//花火の火の部分を作成
	//円
	while (m_circleList.size() < 5)
	{
		m_circleList.emplace_back();
	}

	//活性状態数
	m_circleActiveNum = 0;
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
		m_move.y -= m_shotGravity * deltaTime;

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
			m_circleActiveNum = 0;
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
	for (int i = m_circleActiveNum - 1; i >= 0; i--)
	{
		//弾けているときは減速処理を行う
		if (m_bExploded)
		{
			m_circleList[i].m_move *= 1.0f - (0.98 * deltaTime);

			//重力
			m_circleList[i].m_move.y -= m_particleGravity * deltaTime;
		}
		//寿命が尽きたら非活性状態にする
		if (!m_circleList[i].Update(deltaTime))
		{
			m_circleActiveNum--;
			if (m_circleActiveNum <= 0)
			{
				m_bActive = false;
				break;
			}
			//寿命が尽きたものは
			// 配列の活性状態のまとまりの最後尾と入れ替える
			std::swap(m_circleList[i], m_circleList[m_circleActiveNum]);
		}
	}
}

//描画
void Fireworks1::Draw(float backScale)
{
	//描画先の初期化
	m_tex->ClearRenderTarget(Math::Color{ 0,0,0,0});
	m_tex->SetRenderTarget();

	//行列をリセット
	SHADER.m_spriteShader.ClearMatrix();
	for (int i = m_circleActiveNum - 1; i >= 0; i--)
	{
		for (int j = 0; j < 2; j++)SHADER.m_spriteShader.DrawCircle(m_circleList[i].m_pos.x, m_circleList[i].m_pos.y, 6, &m_circleList[i].m_color, true);
	}

	//描画先をBuckBufferに戻す
	D3D.SetBackBuffer();

	//行列作成
	Math::Matrix scale, rotation, trans, mat;
	if (!m_bExploded)
	{
		scale = Math::Matrix::CreateScale(m_beforeScale * backScale);
	}
	else
	{
		scale = Math::Matrix::CreateScale(m_afterScale * backScale);
	}
	trans = Math::Matrix::CreateTranslation(m_pos.x * backScale, m_pos.y * backScale, 0);

	//360°を10個に割り、回転させて描画
	for (int i = 0; i < 10; i++)
	{
		rotation = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(360 / 10 * i));
		SHADER.m_spriteShader.SetMatrix(scale * rotation * trans);
		SHADER.m_spriteShader.DrawTex_Src(m_tex);
	}
}

//花火を撃つ
void Fireworks1::Shoot(Math::Vector2& startPos, Math::Vector2& startMove, float beforeScale, float afterScale, Math::Color& color)
{
	m_pos = startPos;
	m_move = startMove;
	m_beforeScale = beforeScale;
	m_afterScale = afterScale;
	m_color = color;

	//花火を活性状態にする
	m_bActive = true;
	m_bExploded = false;
	m_circleActiveNum = m_circleList.size();

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
	//活性状態にする
	m_bActive = true;
	m_circleActiveNum = m_circleList.size();

	//爆発したフラグを立てる
	m_bExploded = true;

	Math::Vector2 texPos;
	texPos = { SCREEN_LEFT + m_texRadius,SCREEN_TOP - m_texRadius };

	Math::Vector2 vec;
	float radian;
	//円（火）をランダムに発射
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

		radian = DirectX::XMConvertToRadians(randRange(0, 360));
		vec.x = cosf(radian);
		vec.y = sinf(radian);
		p.m_move = vec * randRange(0, m_explodeSpeedMax);

		//寿命
		p.m_life = randRange(2.0f,3.5f);		//0.45秒～0.6秒
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
	m_type = Type::Circle_Line;

	m_texRadius = 200;
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

	//活性状態数
	m_circleActiveNum = 0;
	m_lineActiveNum = 0;

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
		m_move.y -= m_shotGravity * deltaTime;

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
			m_circleActiveNum = 0;
			m_lineActiveNum = 0;
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
	for (int i = m_circleActiveNum - 1; i >= 0; i--)
	{
		//弾けているときは減速処理を行う
		if (m_bExploded)
		{
			m_circleList[i].m_move *= 1.0f - (0.98 * deltaTime);

			//重力
			m_circleList[i].m_move.y -= m_particleGravity * deltaTime;
		}
		//寿命が尽きたら非活性状態にする
		if (!m_circleList[i].Update(deltaTime))
		{
			m_circleActiveNum--;
			if (m_circleActiveNum <= 0)break;
				
			//寿命が尽きたものは
			// 配列の活性状態のまとまりの最後尾と入れ替える
			std::swap(m_circleList[i], m_circleList[m_circleActiveNum]);
		}
	}
	//花火の火の更新
	//棒形
	if (m_bExploded)
	{
		for (int i = m_lineActiveNum - 1; i >= 0; i--)
		{
			//弾けているときは減速処理を行う
			if (m_bExploded)
			{
				m_lineList[i].m_move *= 1.0f - (0.98 * deltaTime);

				//重力
				m_lineList[i].m_move.y -= m_particleGravity * deltaTime;
			}
			//寿命が尽きたら非活性状態にする
			if (!m_lineList[i].Update(deltaTime))
			{
				m_lineActiveNum--;
				if (m_lineActiveNum <= 0)break;
				
				//寿命が尽きたものは
				// 配列の活性状態のまとまりの最後尾と入れ替える
				std::swap(m_lineList[i], m_lineList[m_lineActiveNum]);
			}
		}
	}

	//火が全てなくなったら非活性状態にする
	if (m_lineActiveNum <= 0 && m_circleActiveNum <= 0)
	{
		m_bActive = false;
	}
}

//描画
void Fireworks2::Draw(float backScale)
{
	//描画先の初期化
	m_tex->ClearRenderTarget(Math::Color{ 0,0,0,0 });
	m_tex->SetRenderTarget();

	//行列をリセット
	SHADER.m_spriteShader.ClearMatrix();

	//円形の描画
	for (int i = m_circleActiveNum - 1; i >= 0; i--)
	{
		SHADER.m_spriteShader.DrawCircle(m_circleList[i].m_pos.x, m_circleList[i].m_pos.y, 5, &m_circleList[i].m_color, true);
		SHADER.m_spriteShader.DrawCircle(m_circleList[i].m_pos.x, m_circleList[i].m_pos.y, 6, &m_circleList[i].m_color, true);
	}

	//棒形の描画
	if (m_bExploded)
	{
		for (int i = m_lineActiveNum - 1; i >= 0; i--)
		{
			SHADER.m_spriteShader.SetMatrix(m_lineList[i].m_mat);
			SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/Bullet3.png"), m_lineList[i].m_color);
		}
	}

	//描画先をBuckBufferに戻す
	D3D.SetBackBuffer();

	//行列作成
	Math::Matrix scale, rotation, trans, mat;
	if (!m_bExploded)
	{
		scale = Math::Matrix::CreateScale(m_beforeScale*backScale);
	}
	else
	{
		scale = Math::Matrix::CreateScale(m_afterScale*backScale);
	}
	trans = Math::Matrix::CreateTranslation(m_pos.x * backScale, m_pos.y * backScale, 0);

	//360°を10個に割り、回転させて描画
	for (int i = 0; i < 10; i++)
	{
		rotation = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(360 / 10 * i));
		SHADER.m_spriteShader.SetMatrix(scale * rotation * trans);
		SHADER.m_spriteShader.DrawTex_Src(m_tex);

	}
}

//花火を撃つ
void Fireworks2::Shoot(Math::Vector2& startPos, Math::Vector2& startMove, float beforeScale, float afterScale, Math::Color& color)
{
	m_pos = startPos; 
	m_move = startMove;
	m_beforeScale = beforeScale;
	m_afterScale = afterScale;
	m_color = color;

	//花火を活性状態にする
	m_bActive = true;
	m_bExploded = false;
	m_circleActiveNum = m_circleList.size();
	m_lineActiveNum = m_lineList.size();

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
	//花火を活性状態にする
	m_bActive = true;
	m_circleActiveNum = m_circleList.size();
	m_lineActiveNum = m_lineList.size();

	//爆発したフラグを立てる
	m_bExploded = true;

	Math::Vector2 vec;
	float radian;
	//円形（火）をランダムに発射
	for (auto& p : m_circleList)
	{
		//移動量
		radian = DirectX::XMConvertToRadians(randRange(0, 360));
		vec.x = cosf(radian);
		vec.y = sinf(radian);
		p.m_move = vec * randRange(0, m_explodeSpeedMax);

		//寿命
		p.m_life = randRange(2.0f,3.5f);		//0.45秒～0.6秒
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
		//移動量
		radian = DirectX::XMConvertToRadians(randRange(0, 360));
		vec.x = cosf(radian);
		vec.y = sinf(radian);
		p.m_move = vec * randRange(0, m_explodeSpeedMax);

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
		p.m_life = randRange(2.0f,3.5f);		//0.5秒～0.8秒
	}
}

//===================================================

//===================================================
//花火3（円だけで描画)
//===================================================

//初期化
void Fireworks3::Init()
{
	m_type = Type::NewCircle;

	m_texRadius = 200;
	m_tex = std::make_shared<KdTexture>();
	m_tex->CreateRenderTarget(m_texRadius * 2, m_texRadius * 2);

	//花火の火の部分を作成
	while (m_circleList.size() < 25)
	{
		m_circleList.emplace_back();
	}

	m_bExploded = false;
	m_bActive = false;

	//活性状態数
	m_circleActiveNum = 0;
}

//更新
void Fireworks3::Update(float deltaTime)
{
	//弾けているときは動かない
	if (!m_bExploded)
	{
		//座標更新
		m_pos += m_move * deltaTime;

		//重力
		m_move.y -= m_shotGravity * deltaTime;

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
			m_circleActiveNum = 0;
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
	for (int i = m_circleActiveNum - 1; i >= 0; i--)
	{
		//弾けているときは減速処理を行う
		if (m_bExploded)
		{
			m_circleList[i].m_move *= 1.0f - (0.98 * deltaTime);

			//重力
			m_circleList[i].m_move.y -= m_particleGravity * deltaTime;
		}
		//寿命が尽きたら非活性状態にする
		if (!m_circleList[i].Update(deltaTime, m_baseScale))
		{
			m_circleActiveNum--;
			if (m_circleActiveNum <= 0)
			{
				m_bActive = false;
				break;
			}

			//寿命が尽きたものは
			// 配列の活性状態のまとまりの最後尾と入れ替える
			std::swap(m_circleList[i], m_circleList[m_circleActiveNum]);
		}
	}
}

//描画
void Fireworks3::Draw(float backScale)
{
	//描画先の初期化
	m_tex->ClearRenderTarget(Math::Color{ 0,0,0,0 });
	m_tex->SetRenderTarget();

	//円の描画
	{
		std::shared_ptr<KdTexture>tex = TextureCache::Instance().Get("Texture/Fireworks/Particle.png");
		for (int i = m_circleActiveNum - 1; i >= 0; i--)
		{

			m_circleList[i].Draw(tex);
		}
	}

	//描画先をBuckBufferに戻す
	D3D.SetBackBuffer();

	//行列作成
	Math::Matrix scale,rotation, trans;
	if (!m_bExploded)
	{
		scale = Math::Matrix::CreateScale(m_beforeScale*backScale);
	}
	else
	{
		scale = Math::Matrix::CreateScale(m_afterScale*backScale);
	}
	trans = Math::Matrix::CreateTranslation(m_pos.x*backScale, m_pos.y*backScale, 0);

	//360°を8個に割り、回転させて描画
	for (int i = 0; i < 8; i++)
	{
		rotation = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(360 / 8 * i));
		SHADER.m_spriteShader.SetMatrix(scale * rotation * trans);
		SHADER.m_spriteShader.DrawTex_Src(m_tex);
		SHADER.m_spriteShader.DrawTex_Src(m_tex, Math::Color{ 0.8f,0.8f,0.8f,1 });
	}
}

//花火を打ち上げる
void Fireworks3::Shoot(Math::Vector2& startPos, Math::Vector2&startMove, float beforeScale, float afterScale, Math::Color& color)
{
	m_pos = startPos;
	m_move = startMove;
	m_beforeScale = beforeScale;
	m_afterScale = afterScale;
	m_color = color;

	//花火を活性状態にする
	m_bActive = true;
	m_bExploded = false;
	m_circleActiveNum = m_circleList.size();

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

		p.m_mat = Math::Matrix::CreateScale(m_baseScale) * Math::Matrix::CreateTranslation(p.m_pos.x, p.m_pos.y, 0);

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

//花火を弾けさせる
void Fireworks3::Explode()
{
	//花火を活性状態にする
	m_bActive = true;
	m_circleActiveNum = m_circleList.size();

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
		p.m_move = vec * randRange(0, m_explodeSpeedMax);

		//拡縮を少し変更
		p.m_scale = 1.0 + randRange(-0.3f, 0.3f);

		//色も少し変える
		float r = m_color.R() + randRange(-0.1f, 0.1f);
		float g = m_color.G() + randRange(-0.1f, 0.1f);
		float b = m_color.B() + randRange(-0.1f, 0.1f);
		float a = m_color.A() + randRange(-0.1f, 0.1f);
		p.m_color = { r,g,b,a };

		p.m_life = randRange(2.0f,3.5f);
	}
	
}

//===================================================

//===================================================
//花火4（花弁画像だけで描画）
//===================================================

//初期化
void Fireworks4::Init()
{
	m_type = Type::Petal;

	m_texRadius = 200;
	m_tex = std::make_shared<KdTexture>();
	m_tex->CreateRenderTarget(m_texRadius * 2, m_texRadius * 2);

	while (m_petalList.size() < 6)
	{
		m_petalList.emplace_back();
	}

	m_bActive = true;
	m_bExploded = false;

	//活性状態数
	m_petalActiveNum = 0;
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
		m_move.y -= m_shotGravity * deltaTime;

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
			m_petalActiveNum = 0;
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
	//花火の火の更新
	for (int i = m_petalActiveNum - 1; i >= 0; i--)
	{
		//弾けているときは減速処理を行う
		if (m_bExploded)
		{
			m_petalList[i].m_move *= 1.0f - (0.98 * deltaTime);

			//重力
			m_petalList[i].m_move.y -= m_particleGravity * deltaTime;
		}
		//寿命が尽きたら非活性状態にする
		if (!m_petalList[i].Update(deltaTime))
		{
			m_petalActiveNum--;
			if (m_petalActiveNum <= 0)
			{
				m_bActive = false;
				break;
			}

			//寿命が尽きたものは
			// 配列中の活性状態のまとまりの最後尾と入れ替える
			std::swap(m_petalList[i], m_petalList[m_petalActiveNum]);
		}
	}
}

//描画
void Fireworks4::Draw(float backScale)
{
	//描画先の初期化
	m_tex->ClearRenderTarget(Math::Color{0,0,0,0});
	m_tex->SetRenderTarget();

	//花弁の描画
	for (auto& p : m_petalList)
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
		scale = Math::Matrix::CreateScale(m_beforeScale * backScale);
	}
	else
	{
		scale = Math::Matrix::CreateScale(m_afterScale * backScale);
	}
	trans = Math::Matrix::CreateTranslation(m_pos.x * backScale, m_pos.y * backScale, 0);

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
void Fireworks4::Shoot(Math::Vector2& startPos, Math::Vector2& startMove, float beforeScale, float afterScale, Math::Color& color)
{
	m_pos = startPos; 
	m_move = startMove;
	m_beforeScale = beforeScale;
	m_afterScale = afterScale;
	m_color = color;

	//花火を活性状態にする
	m_bActive = true;
	m_bExploded = false;
	m_petalActiveNum = m_petalList.size();

	//パーティクルを描画する先の中心座標を求める
	Math::Vector2 texPos;
	texPos = { SCREEN_LEFT + m_texRadius,SCREEN_TOP - m_texRadius };

	//円(火)の初期化
	for (auto& p : m_petalList)
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
	m_bActive = true;
	m_bExploded = true;
	m_petalActiveNum = m_petalList.size();

	Math::Vector2 texPos;
	texPos = { SCREEN_LEFT + m_texRadius,SCREEN_TOP - m_texRadius };

	Math::Vector2 vec;
	float radian;
	//円形（火）をランダムに発射
	for (auto& p : m_petalList)
	{
		//座標
		p.m_pos = texPos;

		radian = DirectX::XMConvertToRadians(randRange(0, 360));
		vec.x = cosf(radian);
		vec.y = sinf(radian);
		p.m_move = vec * randRange(0, m_explodeSpeedMax);

		//色も少し変える
		float r = m_color.R() + randRange(-0.1f, 0.1f);
		float g = m_color.G() + randRange(-0.1f, 0.1f);
		float b = m_color.B() + randRange(-0.1f, 0.1f);
		float a = m_color.A() + randRange(-0.1f, 0.1f);
		p.m_color = { r,g,b,a };

		//寿命
		p.m_life = randRange(2.0f,3.5f);		//0.45秒～0.6秒
	}
	

}

//解放
void Fireworks4::Release()
{
	m_petalList.clear();
}


//===================================================
//花火5（トレイル（軌跡）付き）
//===================================================
void Fireworks5::Init()
{
	m_type = Type::Trail;

	m_texRadius = 200;
	m_tex = std::make_shared<KdTexture>();
	m_tex->CreateRenderTarget(m_texRadius * 2, m_texRadius * 2);

	while (m_trailList.size() < 50)
	{
		m_trailList.emplace_back();
	}

	m_bActive = true;
	m_bExploded = false;

	//活性状態数
	m_trailActiveNum = 0;
}

void Fireworks5::Update(float deltaTime)
{
	//弾けているときは動かない
	if (!m_bExploded)
	{
		//座標更新
		m_pos += m_move * deltaTime;

		//重力
		m_move.y -= m_shotGravity * deltaTime;

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
			m_trailActiveNum = 0;
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
	//花火の火の更新
	for (int i = m_trailActiveNum - 1; i >= 0; i--)
	{
		//弾けているときは減速処理を行う
		if (m_bExploded)
		{
			m_trailList[i].m_move *= 1.0f - (0.98 * deltaTime);

			//重力
			m_trailList[i].m_move.y -= m_particleGravity * deltaTime;
		}
		//寿命が尽きたら非活性状態にする
		if (!m_trailList[i].Update(deltaTime))
		{
			m_trailActiveNum--;
			if (m_trailActiveNum <= 0)
			{
				m_bActive = false;
				break;
			}

			//寿命が尽きたものは
			// 配列の活性状態のまとまりの最後尾と入れ替える
			std::swap(m_trailList[i], m_trailList[m_trailActiveNum]);
		}
	}
}

void Fireworks5::Draw(float backScale)
{
	//描画先の初期化
	m_tex->ClearRenderTarget(Math::Color{ 0,0,0,0 });
	m_tex->SetRenderTarget();

	//軌跡の描画
	KdTexture* tex = TextureCache::Instance().Get("Texture/Fireworks/Particle3.png").get();
	for (auto& trail : m_trailList)
	{
		trail.Draw(tex);
	}

	//描画先をBuckBufferに戻す
	D3D.SetBackBuffer();

	//行列作成
	Math::Matrix scale, rotation, trans;
	if (!m_bExploded)
	{
		scale = Math::Matrix::CreateScale(m_beforeScale * backScale);
	}
	else
	{
		scale = Math::Matrix::CreateScale(m_afterScale * backScale);
	}
	trans = Math::Matrix::CreateTranslation(m_pos.x * backScale, m_pos.y * backScale, 0);

	//360°を8個に割り、回転させて描画
	for (int i = 0; i <7; i++)
	{
		rotation = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(360 / 7 * i));
		SHADER.m_spriteShader.SetMatrix(scale * rotation * trans);
		SHADER.m_spriteShader.DrawTex_Src(m_tex);
		SHADER.m_spriteShader.DrawTex_Src(m_tex, Math::Color{ 0.8f,0.8f,0.8f,1 });

	}
}

void Fireworks5::Shoot(Math::Vector2& startPos, Math::Vector2& startMove, float beforeScale, float afterScale, Math::Color& color)
{
	m_pos = startPos;
	m_move = startMove;
	m_beforeScale = beforeScale;
	m_afterScale = afterScale;
	m_color = color;

	//花火を活性状態にする
	m_bActive = true;
	m_bExploded = false;
	m_trailActiveNum = m_trailList.size();

	//パーティクルを描画する先の中心座標を求める
	Math::Vector2 texPos;
	texPos = { SCREEN_LEFT + m_texRadius,SCREEN_TOP - m_texRadius };

	//円(火)の初期化
	for (auto& trail : m_trailList)
	{
		trail.m_trail.m_pos.clear();

		//座標を中心から少しずらす
		trail.m_pos = {
			texPos.x + randRange(-3.5f,3.5f),
			texPos.y + randRange(-3.5f,3.5f)
		};

		//移動量を0
		trail.m_move = {};

		//色も少し変える
		float r = m_color.R() + randRange(-0.1f, 0.1f);
		float g = m_color.G() + randRange(-0.1f, 0.1f);
		float b = m_color.B() + randRange(-0.1f, 0.1f);
		float a = m_color.A() + randRange(-0.1f, 0.1f);
		if (a < 0.5f)a = 0.5f;
		trail.m_color = { r,g,b,a };

		//寿命は途中（爆発するまでに）で尽きることの無いように
		trail.m_life = 10;
	}
}

void Fireworks5::Explode()
{
	//爆発したフラグを立てる
	m_bActive = true;
	m_bExploded = true;
	m_trailActiveNum = m_trailList.size();

	Math::Vector2 texPos;
	texPos = { SCREEN_LEFT + m_texRadius,SCREEN_TOP - m_texRadius };

	Math::Vector2 vec;
	float radian;
	//円形（火）をランダムに発射
	for (auto& trail : m_trailList)
	{
		trail.m_trail.m_pos.clear();

		//座標
		trail.m_pos = texPos;

		radian = DirectX::XMConvertToRadians(randRange(0, 360));
		vec.x = cosf(radian);
		vec.y = sinf(radian);
		trail.m_move = vec * randRange(0, m_explodeSpeedMax);
		trail.m_trail.m_radian = radian;

		//色も少し変える
		float r = m_color.R() + randRange(-0.1f, 0.1f);
		float g = m_color.G() + randRange(-0.1f, 0.1f);
		float b = m_color.B() + randRange(-0.1f, 0.1f);
		float a = m_color.A() + randRange(-0.1f, 0.1f);
		trail.m_color = { r,g,b,a };

		//寿命
		trail.m_life = randRange(2.0f, 3.5f);		//0.45秒～0.6秒
	}
	
}

void Fireworks5::Release()
{
	m_trailList.clear();
}