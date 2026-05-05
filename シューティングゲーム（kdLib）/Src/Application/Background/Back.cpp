#include"Back.h"
#include"../main.h"
#include"../Light/Light.h"
#include"../TextureCache/TextureCache.h"
#include"../Tools/RandEx/RandEx.h"
#include"../Fireworks/FireworksManager.h"

void LightParticle::Update(float deltaTime)
{
	//透明度が０かつ生存時間が０なら処理を行わない
	if (m_color.A() < 0 && m_life < 0)return;

	m_pos += m_move * deltaTime;

	m_life -= deltaTime;

	m_degree += m_deltaDegree * deltaTime;

	if (m_life <= 0)
	{
		m_color.A(m_color.A() - 0.1f * deltaTime);
	}
	else
	{
		float alpha = m_alphaMin + sinf(DirectX::XMConvertToRadians(m_degree)) * (m_alphaMax - m_alphaMin);
		m_color.A(alpha);
	}
}

void LightParticle::Draw()
{
	Light::Instance().Draw(m_pos, m_radius, m_color);
}

void Back::Init()
{
	m_fireworks = std::make_shared<FireworksManager>();

	//ズームアウトからスタート
	m_bZoomInFlg = false;
	m_allScale = m_zoomOutBackScale; 
	m_bZoomingFlg = false;

	m_farBgWidth = (float)TextureCache::Instance().Get("Texture/Back.png").get()->GetInfo().Width;

	//フェンスの横幅
	m_fanceWidth = m_fanceRenderWidth * m_allScale;

	m_lanternDistance = m_fanceWidth * 5;

	//最背面の背景画像のための初期化
	{
		for (int i = 0; i < 2; i++)
		{
			m_farBg.push_back(FarBackground());
		}
		//一枚目は中央からスタート
		m_farBg.front().m_pos = { 0,0 };
		m_farBg.front().m_scale = { 1,1 };
		//二枚目は一枚目の右隣からスタート
		m_farBg.back().m_pos = { m_farBg.front().m_pos.x + m_farBgWidth,0 };
		m_farBg.back().m_scale = { -1,1 };
		for (auto& farBg : m_farBg)
		{
			farBg.CreateMat(m_allScale);
		}
	}

	//フェンスの初期化
	{
		for (int i = 0; i < 20; i++)
		{
			m_fance.push_back(BackObject());
		}

		int i = 0;
		for (auto& fance : m_fance)
		{
			fance.m_pos = { -m_farBgWidth / 2.0f + m_fanceWidth * i,-300 * m_allScale };
			fance.m_scale = m_fanceRenderScale * m_allScale;
			fance.m_color = { 0.8f,0.8f,0.8f,0.5f };
			i++;
		}
	}
	//ランタンの初期化
	{

		for (int i = 0; i < 5; i++)
		{
			m_lantern.push_back(BackObject());
		}

		int i = 0;
		for (auto& lantern : m_lantern)
		{
			lantern.m_pos = { m_fance.front().m_pos.x + m_fanceWidth * 4 * i,-300 * m_allScale };
			lantern.m_scale = m_lanternRenderScale * m_allScale;
			lantern.m_color = { 0.8f,0.8f,0.8f,0.5f };
			i++;
		}
	}

	for (int i = 0; i < 30; i++)
	{
		Respawn(&m_lightParticleList[i]);
	}
}

//更新
void Back::Update(float deltaTime)
{
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		StartZoomIn();
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		StartZoomOut();
	}


	if (m_bZoomingFlg)
	{
		UpdateZooming(deltaTime);
	}
	else
	{
		UpdateNormal(deltaTime);
	}
	m_fireworks->Update(deltaTime);
}

//描画
void Back::Draw()
{

	//最背面の背景画像を描画
	std::shared_ptr<KdTexture> tex =  TextureCache::Instance().Get("Texture/Back.png");
	for (auto&farBg:m_farBg)
	{
		SHADER.m_spriteShader.SetMatrix(farBg.m_mat);
		D3D.SetBlendState(BlendMode::Add);
		SHADER.m_spriteShader.DrawTex_Src(tex, Math::Color{1.0f,1.0f,1.0f,1.0f});
		D3D.SetBlendState(BlendMode::Alpha);
		SHADER.m_spriteShader.DrawTex_Src(tex, Math::Color{ 0.6f,0.6f,0.6f,0.8f });
	}

	//フェンスの描画
	std::shared_ptr<KdTexture>fanceTex = TextureCache::Instance().Get("Texture/Fance.png");
	for (auto& f : m_fance)
	{
		f.Draw(fanceTex);
	}

	//ランタンの描画
	std::shared_ptr<KdTexture>lanternTex = TextureCache::Instance().Get("Texture/Lantern.png");
	for (auto& l : m_lantern)
	{
		l.Draw(lanternTex);
		Light::Instance().Draw(l.m_pos * m_allScale, { 25, 25 },Math::Color{0.9f,0.9f,0.9f,0.2f});
	}

	//蛍的なものの描画
	for (auto& l : m_lightParticleList)
	{
		l.Draw();
	}

	//花火の描画
	m_fireworks->Draw(m_allScale);
}

//ズームインする（ゲーム画面に遷移時に呼び出す）
void Back::StartZoomIn()
{
	m_bZoomInFlg = true;
	m_bZoomingFlg = true;
}

//ズームアウトする（タイトル画面に遷移時に呼び出す）
void Back::StartZoomOut()
{
	m_bZoomInFlg = false;
	m_bZoomingFlg = true;
}

void Back::Respawn(LightParticle* light)
{
	light->m_pos = { randRange(SCREEN_LEFT,SCREEN_RIGHT),randRange(SCREEN_BOTTOM,SCREEN_TOP) };
	light->m_move = { randRange(-5.0f,5.0f),randRange(-5.0f,5.0f) };
	float r = randRange(1.5, 3);
	light->m_radius = { r,r };
	light->m_degree = randRange(0, 180);
	light->m_deltaDegree = randRange(0.1f, 2.0f);
	light->m_alphaMin = randRange(0.1f, 0.15f);
	light->m_alphaMax = randRange(0.2f, 0.6f);
	light->m_color = { 0.7f,0.7f,0.7f,randRange(light->m_alphaMin,light->m_alphaMax) };
	light->m_life = randRange(5, 20);
}

void Back::UpdateZooming(float deltaTime)
{
	//ズーム中の拡縮を変更
	if (m_bZoomInFlg)
	{
		m_allScale += m_zoomSpeed * deltaTime;
		if (m_allScale >= m_zoomInBackScale)
		{
			m_allScale = m_zoomInBackScale;
			m_bZoomingFlg = false;
		}
	}
	else
	{
		m_allScale -= m_zoomSpeed * deltaTime;
		if (m_allScale <= m_zoomOutBackScale)
		{
			m_allScale = m_zoomOutBackScale;
			m_bZoomingFlg = false;
		}
	}

	float scrollX;
	{
		//アンダーフローが発生するため、小数点以下第一位以上をスクロール量とする
		//発生すると背景画像が少しずれ、バックバッファが表示される
		int tempScrollX = -m_scrollSpeed * deltaTime * 10;
		scrollX = tempScrollX / 10.0f;
	}

	//背景画像の更新
	{
		//背景画像がアンダーフロー時にバックバッファが表示されないようにしている。
		// スクロール量を直接足すのはリストの最初だけ。
		//　次からは最初の値を元に足す。
		m_farBg.front().m_pos.x += scrollX;
		m_farBg.back().m_pos.x = m_farBg.front().m_pos.x + m_farBgWidth;

		//画面端に到達したら、画面右端にセットする
		if (m_farBg.front().m_pos.x <= -m_farBgWidth)
		{
			m_farBg.front().m_pos.x = m_farBg.back().m_pos.x + m_farBgWidth;
			//背景画像二枚のうち、左にあるほうをリストの最初に、右にあるものをリストの最後にする
			m_farBg.splice(m_farBg.end(), m_farBg, m_farBg.begin());
		}
		for (auto& farBg : m_farBg)
		{
			farBg.CreateMat(m_allScale);
		}
	}

	//背景オブジェクトの更新
	//提灯
	for (auto& l : m_lantern)
	{
		l.Update(scrollX,m_allScale);
	}
	//画面外に到達したとき
	// m_fanceWidthには本来の画像より少し小さい値を入れている。
	// そのため、画面端判定に使用すると、画像が画面内で消えてしまう。
	// なので 画面端判定はゆとりを持たせている
	if (m_lantern.front().m_pos.x < SCREEN_LEFT/m_zoomOutBackScale - m_fanceWidth)
	{
		m_lantern.front().m_pos.x = m_lantern.back().m_pos.x + m_lanternDistance;
		m_lantern.splice(m_lantern.end(), m_lantern, m_lantern.begin());
	}

	//フェンス
	for (auto& f : m_fance)
	{
		f.Update(scrollX,m_allScale);
	}
	//画面外に到達したとき
	// m_fanceWidthには本来の画像より少し小さい値を入れている。
	// そのため、画面端判定に使用すると、画像が画面内で消えてしまう。
	// なので 画面端判定はゆとりを持たせている
	if (m_fance.front().m_pos.x < SCREEN_LEFT / m_zoomOutBackScale - m_fanceWidth)
	{
		//フェンスを右端に配置
		//リストの最後に配置する
		m_fance.front().m_pos.x = m_fance.back().m_pos.x + m_fanceWidth;
		m_fance.splice(m_fance.end(), m_fance, m_fance.begin());
	}


	//画面中の明かり（蛍みたいなパーティクル）
	for (auto& l : m_lightParticleList)
	{
		l.Update(deltaTime);
		if (l.m_color.A() < 0 && l.m_life < 0)
		{
			Respawn(&l);
		}
	}
}

void Back::UpdateNormal(float deltaTime)
{
	float scrollX;
	{
		//アンダーフローが発生するため、小数点以下第一位以上をスクロール量とする
		//発生すると背景画像が少しずれ、バックバッファが表示される
		int tempScrollX = -m_scrollSpeed * deltaTime * m_allScale * 10;
		scrollX = tempScrollX / 10.0f;
	}

	//背景画像の更新
	{
		//背景画像がアンダーフロー時にバックバッファが表示されないようにしている。
		// スクロール量を直接足すのはリストの最初だけ。
		//　次からは最初の値を元に足す。
		m_farBg.front().m_pos.x += scrollX;
		m_farBg.back().m_pos.x = m_farBg.front().m_pos.x + m_farBgWidth ;

		//画面端に到達したら、画面右端にセットする
		if (m_farBg.front().m_pos.x <= -m_farBgWidth)
		{
			m_farBg.front().m_pos.x = m_farBg.back().m_pos.x + m_farBgWidth;
			//背景画像二枚のうち、左にあるほうをリストの最初に、右にあるものをリストの最後にする
			m_farBg.splice(m_farBg.end(), m_farBg, m_farBg.begin());
		}
		for (auto& farBg : m_farBg)
		{
			farBg.CreateMat(m_allScale);
		}
	}

	//背景オブジェクトの更新
	//提灯
	for (auto& l : m_lantern)
	{
		l.Update(scrollX,m_allScale);
	}
	//画面外に到達したとき
	// m_fanceWidthには本来の画像より少し小さい値を入れている。
	// そのため、画面端判定に使用すると、画像が画面内で消えてしまう。
	// なので 画面端判定はゆとりを持たせている
	if (m_lantern.front().m_pos.x < SCREEN_LEFT / m_zoomOutBackScale - m_fanceWidth)
	{
		m_lantern.front().m_pos.x = m_lantern.back().m_pos.x + m_lanternDistance;
		m_lantern.splice(m_lantern.end(), m_lantern, m_lantern.begin());
	}

	//フェンス
	for (auto& f : m_fance)
	{
		f.Update(scrollX,m_allScale);
	}
	//画面外に到達したとき
	// m_fanceWidthには本来の画像より少し小さい値を入れている。
	// そのため、画面端判定に使用すると、画像が画面内で消えてしまう。
	// なので 画面端判定はゆとりを持たせている
	if (m_fance.front().m_pos.x < SCREEN_LEFT / m_zoomOutBackScale - m_fanceWidth)
	{
		//フェンスを右端に配置
		//リストの最後に配置する
		m_fance.front().m_pos.x = m_fance.back().m_pos.x + m_fanceWidth;
		m_fance.splice(m_fance.end(), m_fance, m_fance.begin());
	}


	//画面中の明かり（蛍みたいなパーティクル）
	for (auto& l : m_lightParticleList)
	{
		l.Update(deltaTime);
		if (l.m_color.A() < 0 && l.m_life < 0)
		{
			Respawn(&l);
		}
	}
}


