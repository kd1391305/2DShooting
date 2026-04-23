#include"FireworksManager.h"

//更新
void FireworksManager::Update(float deltaTime)
{
	for (int i= m_fireworksList.size()-1;i>=0;i--)
	{
		if (m_fireworksList[i].IsActive())
		{
			m_fireworksList[i].Update(deltaTime);
		}

		else if (m_fireworksList.size() > m_poolSize)
		{
			m_fireworksList.erase(m_fireworksList.begin() + i);
		}
	}
}

//描画
void FireworksManager::Draw()
{
	//加算合成で描画
	D3D.SetBlendState(BlendMode::Add);

	for (auto& f : m_fireworksList)
	{
		if (f.IsActive()) 
		{
			KdTexture tex;
			tex.CreateRenderTarget(1280, 720);
			tex.ClearRenderTarget({ 0,0,0,0 });
			tex.SetRenderTarget();
			f.Draw(&m_tex);
			D3D.SetBackBuffer();
			Math::Matrix scale = Math::Matrix::CreateScale(1.0f,1.0f, 0);
			Math::Matrix trans = Math::Matrix::CreateTranslation(0, 0, 0);
			Math::Matrix mat = scale * trans;
			SHADER.m_spriteShader.SetMatrix(mat);
			SHADER.m_spriteShader.DrawTex_Src(&tex);
		}
	}

	//元に戻す
	D3D.SetBlendState(BlendMode::Alpha);
}

//花火を打ち上げる
void FireworksManager::Shot(Math::Vector2 pos,Math::Vector2 move)
{
	//使用していない花火をオブジェクトプール内から探す
	if (m_fireworksList.size() == m_poolSize)
	{
		for (auto& f : m_fireworksList)
		{
			if (!f.IsActive())
			{
				//花火を打ち上げる
				f.Shot(pos,move);
				//処理を終わる
				return;
			}
		}
	}
	//新しく花火オブジェクトを作成
	m_fireworksList.emplace_back();
	m_fireworksList.back().Shot(pos, move);
}

//初期化（１回しか呼ばれない）
void FireworksManager::Init()
{
	//オブジェクトプールの作成
	for (int i = 0; i < m_poolSize; i++)
	{
		m_fireworksList.emplace_back();
	}

	//画像の読み込み
	m_tex.Load("Texture/Bullet3.png");
}

//解放（１回しか呼ばれない）
void FireworksManager::Release()
{
	m_fireworksList.clear();
}
