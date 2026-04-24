#include"FireworksManager.h"
#include"../Font/DrawString.h"

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

	for (int i= m_fireworksList2.size()-1;i>=0;i--)
	{
		if (m_fireworksList2[i].IsActive())
		{
			m_fireworksList2[i].Update(deltaTime);
		}

		else if (m_fireworksList2.size() > m_poolSize)
		{
			m_fireworksList2.erase(m_fireworksList2.begin() + i);
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
			f.Draw(&m_tex);
		}
	}

	for (auto& f : m_fireworksList2)
	{
		if (f.IsActive())
		{
			f.Draw(&m_tex);
		}
	}

	D3D.SetBlendState(BlendMode::Alpha);

	char str[100];
	sprintf_s(str, sizeof(str), "%d", m_fireworksList.size() + m_fireworksList2.size());
	DWriteCustom::Instance().Draw(str, { 0,0 });
}

//花火を打ち上げる
void FireworksManager::Shot(Math::Vector2 pos,Math::Vector2 targetP)
{
	//使用していない花火をオブジェクトプール内から探す
	if (m_fireworksList.size() == m_poolSize)
	{
		for (auto& f : m_fireworksList)
		{
			if (!f.IsActive())
			{
				//初期化
				f.Init();
				//花火を打ち上げる
				f.Shot(pos,targetP);
				//処理を終わる
				return;
			}
		}
	}
	//新しく花火オブジェクトを作成
	m_fireworksList.emplace_back();
	m_fireworksList.back().Init();
	m_fireworksList.back().Shot(pos, targetP);
}

void FireworksManager::Shot2(Math::Vector2 pos, Math::Vector2 targetP)
{
	//使用していない花火をオブジェクトプール内から探す
	if (m_fireworksList2.size() == m_poolSize)
	{
		for (auto& f : m_fireworksList2)
		{
			if (!f.IsActive())
			{
				//初期化
				f.Init();
				//花火を打ち上げる
				f.Shot(pos, targetP);
				//処理を終わる
				return;
			}
		}
	}
	//新しく花火オブジェクトを作成
	m_fireworksList2.emplace_back();
	m_fireworksList2.back().Init();
	m_fireworksList2.back().Shot(pos, targetP);
}

//初期化（１回しか呼ばれない）
void FireworksManager::Init()
{
	//オブジェクトプールの作成
	for (int i = 0; i < m_poolSize; i++)
	{
		m_fireworksList.emplace_back();
		m_fireworksList.back().Init();
	}
	for (int i = 0; i < m_poolSize; i++)
	{
		m_fireworksList2.emplace_back();
		m_fireworksList2.back().Init();
	}

	//画像の読み込み
	m_tex.Load("Texture/Bullet3.png");
}

//解放（１回しか呼ばれない）
void FireworksManager::Release()
{
	m_fireworksList.clear();
}
