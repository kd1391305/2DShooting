#include"Fireworks.h"
#include"../Tools/RandEx/RandEx.h"
#include"../Tools/Collision/Collision.h"
#include"../main.h"

//リセットする（配列を作成しておく）
void Fireworks::Init()
{
	if (!m_tex)
	{
		m_texRadius = 300;
		m_tex = std::make_shared<KdTexture>();
		m_tex->CreateRenderTarget(m_texRadius * 2, m_texRadius * 2);
	}

	//花火の火の部分を作成
	//円
	while (m_circleList.size() < 5)
	{
		m_circleList.emplace_back();
	}

	//棒
	while (m_lineList.size() < 30)
	{
		m_lineList.emplace_back();
	}
}

//更新
void Fireworks::Update(float deltaTime)
{
	if (!m_bExploded)
	{
		//座標更新
		m_pos += m_move * deltaTime;
	}

	//画面外にいったら
	float left = m_pos.x - m_radius;
	float right = m_pos.x + m_radius;
	float top = m_pos.y + m_radius;
	float bottom = m_pos.y - m_radius;
	if (left < SCREEN_LEFT || right > SCREEN_RIGHT || top > SCREEN_TOP || bottom < SCREEN_BOTTOM)
	{
		m_circleList.clear();
		m_lineList.clear();
		m_bActive = false;
		m_bExploded = false;
		return;
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

	//弾けている（爆発している）ときだけ処理を行う
	if (m_bExploded)
	{
		for (int i = m_lineList.size() - 1; i >= 0; i--)
		{
			//弾けているときは減速処理を行う
			if (m_bExploded)
			{
				m_lineList[i].m_move *= 1.0f - (0.95 * deltaTime);
			}
			//寿命が尽きたら削除
			if (!m_lineList[i].Update(deltaTime))
			{
				//配列を入れ替えてpop_backで消去する（配列の途中を削除すると、詰めていく操作が行われるため）
				Particle2 temp = m_lineList[i];
				m_lineList[i] = m_lineList.back();
				m_lineList.back() = temp;
				//消去
				m_lineList.pop_back();
			}
		}
	}

	//火が全てなくなったら非活性状態にする
	if (m_circleList.empty() && m_lineList.empty())
	{
		m_bActive = false;
	}

	Math::Matrix scale = Math::Matrix::CreateScale(m_scale.x, m_scale.y, 0);
	Math::Matrix rota = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(0));
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scale * rota * trans;
}

//花火を打ち上げる
void Fireworks::Shot(Math::Vector2 startP,Math::Vector2 targetP)
{
	//花火の当たり判定
	m_radius = 10;

	//花火の拡縮
	m_scale.x = 1.0f;
	m_scale.y = 1.0f;

	//花火を活性状態にする
	m_bActive = true;

	//花火の中心（軸）の初期化
	//この値を元に、花火の火の部分を作成する
	m_pos = startP;

	//打ち上げる角度を求める
	float radian = atan2f(targetP.y - startP.y, targetP.x - startP.x);
	//角度を元に移動量を求める
	m_move.x = cosf(radian) * m_speed;
	m_move.y = sinf(radian) * m_speed;
	m_color = { randRange(0, 0.6f), randRange(0, 0.6f), randRange(0, 0.6f), randRange(0.4f, 0.6f) };
	m_bExploded = false;

	//花火の火の部分を作成
	Math::Vector2 texPos;			//パーティクルを描画する先の中心座標
	texPos = { SCREEN_LEFT + m_texRadius,SCREEN_TOP - m_texRadius };

	//円
	for (auto& p : m_circleList)
	{
		//座標を中心（軸）から少しずらす
		p.m_pos = {
			texPos.x + randRange(-3.5f,3.5f),
			texPos.y + randRange(-3.5f,3.5f)
		};
		//移動量を中心（軸）と比べて少しだけ変化させる
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
		p.m_life = 999;
	}
}

//花火を弾けさせる
void Fireworks::Explode()
{
	//円は中央に配置、寿命も短く
	//棒は外側に配置、寿命は少し長く

	m_bExploded = true;

	//円
	
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

	//棒
	for (auto& p : m_lineList)
	{
		//座標
		Math::Vector2 texPos;			//パーティクルを描画する先の中心座標
		texPos = { SCREEN_LEFT + m_texRadius,SCREEN_TOP - m_texRadius };
		p.m_pos = texPos;

		//移動量
		Math::Vector2 move = { randRange(0.0f,150.0f),randRange(0.0f,150.0f) };
		if (rand() % 2)
			move.x *= -1;
		if (rand() % 2)
			move.y *= -1;
		p.m_move = move;

		//角度
		p.m_radian = atan2f(p.m_move.y, p.m_move.x);
		
		//色
		float r = m_color.R() + randRange(-0.1f, 0.1f);
		float g = m_color.G() + randRange(-0.1f, 0.1f);
		float b = m_color.B() + randRange(-0.1f, 0.1f);
		float a = m_color.A() + randRange(-0.1f, 0.1f);
		p.m_color = { r,g,b,a };

		//寿命
		p.m_life = randRange(0.5f, 0.7f);				//0.5秒～0.7秒
	}
}

//描画
void Fireworks::Draw(KdTexture* tex)
{
	m_tex->ClearRenderTarget(Math::Color{ 0,0,0,0 });
	m_tex->SetRenderTarget();
	SHADER.m_spriteShader.ClearMatrix();
	for (auto& p : m_circleList)
	{
		SHADER.m_spriteShader.DrawCircle(p.m_pos.x, p.m_pos.y, 5, &p.m_color, true);
		SHADER.m_spriteShader.DrawCircle(p.m_pos.x, p.m_pos.y, 6, &p.m_color, true);
	}
	if (m_bExploded)
	{
		for (auto& p : m_lineList)
		{
			SHADER.m_spriteShader.SetMatrix(p.m_mat);
			SHADER.m_spriteShader.DrawTex_Src(tex, p.m_color);
		}
	}
	D3D.SetBackBuffer();

	Math::Matrix scale, rotation, trans, mat;
	scale = Math::Matrix::CreateScale(m_scale.x, m_scale.y, 0);
	trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	for (int i = 0; i < 10; i++)
	{
		rotation = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(360 / 10 * i));
		SHADER.m_spriteShader.SetMatrix(scale * rotation * trans);
		SHADER.m_spriteShader.DrawTex_Src(m_tex.get());
	}
}

//===================================================
//二つ目の花火パーティクル
//===================================================
void Fireworks2::Update(float deltaTime)
{
	if (!m_bExploded)
	{
		//座標更新
		m_pos += m_move * deltaTime;

		//ここだけ変更
	// ========================================================
	// targetPosに近づいたら花火を弾けさせる
		if (IsCollision(m_pos, m_radius, m_targetPos, m_radius))
		{
			Explode();
			m_bExploded = true;
		}
		//========================================================
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

	//弾けている（爆発している）ときだけ処理を行う
	if (m_bExploded)
	{
		for (int i = m_lineList.size() - 1; i >= 0; i--)
		{
			//弾けているときは減速処理を行う
			if (m_bExploded)
			{
				m_lineList[i].m_move *= 1.0f - (0.95 * deltaTime);
			}
			//寿命が尽きたら削除
			if (!m_lineList[i].Update(deltaTime))
			{
				//配列を入れ替えてpop_backで消去する（配列の途中を削除すると、詰めていく操作が行われるため）
				Particle2 temp = m_lineList[i];
				m_lineList[i] = m_lineList.back();
				m_lineList.back() = temp;
				//消去
				m_lineList.pop_back();
			}
		}
	}

	//火が全てなくなったら非活性状態にする
	if (m_circleList.empty() && m_lineList.empty())
	{
		m_bActive = false;
	}

	Math::Matrix scale = Math::Matrix::CreateScale(m_scale.x, m_scale.y, 0);
	Math::Matrix rota = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(0));
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scale * rota * trans;
}

void Fireworks2::Shot(Math::Vector2 startP, Math::Vector2 targetP)
{
	//ここだけ違う=============
	m_targetPos = targetP; 
	
	//========================= 
	//花火の当たり判定
	m_radius = 10;

	//花火の拡縮
	m_scale.x = 1.0f;
	m_scale.y = 1.0f;

	//花火を活性状態にする
	m_bActive = true;

	//花火の中心（軸）の初期化
	//この値を元に、花火の火の部分を作成する
	m_pos = startP;

	//打ち上げる角度を求める
	float radian = atan2f(targetP.y - startP.y, targetP.x - startP.x);
	//角度を元に移動量を求める
	m_move.x = cosf(radian) * m_speed;
	m_move.y = sinf(radian) * m_speed;
	m_color = { randRange(0.0f, 0.6f), randRange(0.0f, 0.6f), randRange(0.0f, 0.6f), randRange(0.4f, 0.6f)};
	m_bExploded = false;

	//花火の火の部分を作成
	//円
	for (auto& p : m_circleList)
	{
		//座標
		Math::Vector2 texPos;			//パーティクルを描画する先の中心座標
		texPos = { SCREEN_LEFT + m_texRadius,SCREEN_TOP - m_texRadius };
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
		p.m_life = 999;
	}

	//棒
	/*for (auto& p : m_lineList)
	{
		p.m_pos = {
			m_pos.x + randRange(-3.5f,3.5f),
			m_pos.y + randRange(-3.5f,3.5f)
		};
		p.m_move = {
			m_move.x + randRange(-3.0f,3.0f),
			m_move.y + randRange(-3.0f,3.0f)
		};
		float r = m_color.R() + randRange(-0.1f, 0.1f);
		float g = m_color.G() + randRange(-0.1f, 0.1f);
		float b = m_color.B() + randRange(-0.1f, 0.1f);
		float a = m_color.A() + randRange(-0.1f, 0.1f);
		p.m_color = { r,g,b,a };
		p.m_life = 999;

	}*/
}

//===================================================


