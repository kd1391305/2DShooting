#include"FireworksManager.h"
#include"../Font/DrawString.h"
#include"Fireworks.h"

//更新
void FireworksManager::Update(float deltaTime)
{
	//花火
	for (int i = m_list.size() - 1; i >= 0; i--)
	{
		//活性状態だったら
		if (m_list[i]->IsActive())
		{
			m_list[i]->Update(deltaTime);
		}
		//非活性状態だったら
		else
		{
			m_list.erase(m_list.begin() + i);
		}
	}
}

//描画
void FireworksManager::Draw()
{
	//加算合成で描画
	D3D.SetBlendState(BlendMode::Add);

	for (auto& f : m_list)
	{
		if (f->IsActive())
		{
			f->Draw(&m_tex);
		}
	}

	D3D.SetBlendState(BlendMode::Alpha);
}

//花火を打ち上げる
void FireworksManager::Shot(Math::Vector2 pos, Math::Vector2 targetP, Math::Vector2 scale, const bool bTarget)
{
	//新しく花火オブジェクトを作成
	m_list.push_back(std::make_shared<Fireworks2>());
	m_list.back()->Init();
	m_list.back()->Shot(pos, targetP, scale, bTarget);
}

//初期化（１回しか呼ばれない）
void FireworksManager::Init()
{
	//画像の読み込み
	m_tex.Load("Texture/Bullet3.png");
}

//解放（１回しか呼ばれない）
void FireworksManager::Release()
{
	m_list.clear();
}
