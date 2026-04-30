#include"FireworksManager.h"
#include"../Font/DWriteCustom.h"
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
			f->Draw();
		}
	}

	D3D.SetBlendState(BlendMode::Alpha);
}

//花火を打ち上げる
void FireworksManager::Shot(FireworksManager::Type name,Math::Vector2 pos, Math::Vector2 targetPos, float speed, Math::Vector2 beforeScale, Math::Vector2 afterScale, Math::Color color, const bool bTarget)
{
	//新しく花火オブジェクトを作成
	switch (name)
	{
	case FireworksManager::Type::Circle:
		m_list.push_back(std::make_shared<Fireworks1>());
		break;
	case FireworksManager::Type::Circle_Line:
		m_list.push_back(std::make_shared<Fireworks2>());
		break; 
	case FireworksManager::Type::NewCircle:
		m_list.push_back(std::make_shared<Fireworks3>());
		break; 
	case FireworksManager::Type::Petal:
		m_list.push_back(std::make_shared<Fireworks4>());
		break;
	}
	m_list.back()->Init();
	m_list.back()->Shot(pos, targetPos, speed, beforeScale, afterScale, color, bTarget);
}

//初期化（１回しか呼ばれない）
void FireworksManager::Init()
{
	
}

//解放（１回しか呼ばれない）
void FireworksManager::Release()
{
	m_list.clear();
}
