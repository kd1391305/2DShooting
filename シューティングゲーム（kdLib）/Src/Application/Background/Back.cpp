#include"Back.h"
#include"../main.h"
#include"../Light/Light.h"
#include"../TextureCache/TextureCache.h"
#include"../Tools/RandEx/RandEx.h"

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
		float alpha = m_alphaMin + sinf(DirectX::XMConvertToRadians(m_degree) * (m_alphaMax - m_alphaMin));
		m_color.A(alpha);
	}
}

void LightParticle::Draw()
{
	Light::Instance().Draw(m_pos, m_radius, m_color);
}



void Back::Init()
{
	//フェンスの横幅
	m_fanceWidth = 110.5f;

	//データの初期化
	for (int i = 0; i < s_drawNum; i++)
	{
		m_pos[i] = { 0 + (float)TextureCache::Instance().Get("Texture/Back.png").get()->GetInfo().Width * i,0 };									//一つ目は中心スタート、二つ目はその右隣からスタート
		Math::Matrix scaleMat = Math::Matrix::CreateScale(1 - (i % 2) * 2, 1, 0);		//偶数はそのまま、奇数は反転させて描画する
		Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos[i].x, m_pos[i].y, 0);
		m_mat[i] = scaleMat * transMat;
	}

	//背景オブジェクトの初期化
	for (int i = 0; i < 3; i++)
	{
		m_lantern[i].m_pos = { SCREEN_LEFT + m_fanceWidth * 4 * i,-300 };
		m_lantern[i].m_scale = 0.2f;
		m_lantern[i].m_color = { 0.8f,0.8f,0.8f,0.5f };
	}
	for (int i = 0; i < 13; i++)
	{
		m_fance[i].m_pos = { SCREEN_LEFT + m_fanceWidth * i,-300 };
		m_fance[i].m_scale = 1.0f;
		m_fance[i].m_color = { 0.8f,0.8f,0.8f,0.5f };
	}

	for (int i = 0; i < 30; i++)
	{
		Respawn(&m_lightParticleList[i]);
	}
}

//更新
void Back::Update(float deltaTime)
{
	float scrollX = -40.0f * deltaTime;

	for (int i = 0; i < s_drawNum; i++)
	{
		//左にスクロールする
		m_pos[i].x += scrollX;

		//画面端に到達したら、画面右端にセットする
		float radiusX = (float)TextureCache::Instance().Get("Texture/Back.png").get()->GetInfo().Width / 2.0f;			//画像の半径
		float right = m_pos[i].x + radiusX;						//画像の右端
		if (right < SCREEN_LEFT)m_pos[i].x += radiusX * 2 * 2;

		//行列作成
		Math::Matrix scaleMat, transMat;
		scaleMat = Math::Matrix::CreateScale(1 - (i % 2) * 2, 1, 0);					//偶数なら左右反転、奇数ならそのまま
		transMat = Math::Matrix::CreateTranslation(m_pos[i].x, m_pos[i].y, 0);
		m_mat[i] = scaleMat * transMat;
	}
	for (auto& l : m_lantern)
	{
		if (l.m_pos.x < SCREEN_LEFT - m_fanceWidth / 2.0f)
		{
			l.m_pos.x = SCREEN_LEFT + m_fanceWidth * 12;
		}
		l.Update(scrollX);
	}


	for (auto& f : m_fance)
	{
		if (f.m_pos.x < SCREEN_LEFT - m_fanceWidth / 2.0f)
		{
			f.m_pos.x = SCREEN_LEFT + m_fanceWidth * 12;
		}
		f.Update(scrollX);
	}

	for (auto& l : m_lightParticleList)
	{
		l.Update(deltaTime);
		if (l.m_color.A() < 0 && l.m_life < 0)
		{
			Respawn(&l);
		}
	}

}

//描画
void Back::Draw()
{
	std::shared_ptr<KdTexture> tex =  TextureCache::Instance().Get("Texture/Back.png");
	for (int i = 0; i < 2; i++)
	{
		SHADER.m_spriteShader.SetMatrix(m_mat[i]);
		D3D.SetBlendState(BlendMode::Add);
		SHADER.m_spriteShader.DrawTex_Src(tex, Math::Color{1.0f,1.0f,1.0f,1.0f});
		D3D.SetBlendState(BlendMode::Alpha);
		SHADER.m_spriteShader.DrawTex_Src(tex, Math::Color{ 0.6f,0.6f,0.6f,0.8f });
	}

	std::shared_ptr<KdTexture>fanceTex = TextureCache::Instance().Get("Texture/Fance.png");
	for (auto& f : m_fance)
	{
		f.Draw(fanceTex);
	}

	std::shared_ptr<KdTexture>lanternTex = TextureCache::Instance().Get("Texture/Lantern.png");
	for (auto& l : m_lantern)
	{
		l.Draw(lanternTex);
		Light::Instance().Draw(l.m_pos, { 25, 25 },Math::Color{0.9f,0.9f,0.9f,0.2f});
	}

	for (auto& l : m_lightParticleList)
	{
		l.Draw();
	}
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


