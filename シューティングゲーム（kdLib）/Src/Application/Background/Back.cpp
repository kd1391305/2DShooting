#include"Back.h"
#include"../main.h"
#include"../Light/Light.h"
#include"../TextureCache/TextureCache.h"

void Back::Init()
{
	//データの初期化
	for (int i = 0; i < s_drawNum; i++)
	{
		m_pos[i] = { 0 + (float)TextureCache::Instance().Get("Texture/Back.png").get()->GetInfo().Width * i,0 };									//一つ目は中心スタート、二つ目はその右隣からスタート
		Math::Matrix scaleMat = Math::Matrix::CreateScale(1 - (i % 2) * 2, 1, 0);		//偶数はそのまま、奇数は反転させて描画する
		Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos[i].x, m_pos[i].y, 0);
		m_mat[i] = scaleMat * transMat;
	}

	//背景オブジェクトの初期化
	for (int i = 0; i < 10; i++)
	{
		m_lantern[i].m_pos = { 0,-300 };
		m_lantern[i].m_scale = 0.2f;
		m_lantern[i].m_color = { 0.8f,0.8f,0.8f,0.5f };
	}
	for (int i = 0; i < 10; i++)
	{
		m_fance[i].m_pos = {147.5f * i,-300};
		m_fance[i].m_scale = 0.2f;
		m_fance[i].m_color = { 0.8f,0.8f,0.8f,0.5f };
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
		l.Update(scrollX);
	}


	for (auto& f : m_fance)
	{
		f.Update(scrollX);
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
		SHADER.m_spriteShader.DrawTex_Src(tex, Math::Color{ 1.0f,1.0f,1.0f,0.8f });
	}

	std::shared_ptr<KdTexture>fanceTex = TextureCache::Instance().Get("Texture/Fance.png");
	std::shared_ptr<KdTexture>lanternTex = TextureCache::Instance().Get("Texture/Lantern.png");
	for (auto& f : m_fance)
	{
		f.Draw(fanceTex);
	}


	for (auto& l : m_lantern)
	{
		l.Draw(lanternTex);
		Light::Instance().Draw(l.m_pos, { 30, 30 },Math::Color{0.9f,0.9f,0.9f,0.01f});
	}
}


