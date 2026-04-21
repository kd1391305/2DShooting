#include"FireworksManager.h"

void C_FireworksManager::Update()
{
	for (auto& f : m_fireworksList)
	{
		if(f.IsActive())
			f.Update();
	}
}

void C_FireworksManager::Draw()
{
	D3D.SetBlendState(BlendMode::Add);
	for (auto& f : m_fireworksList)
	{
		if (f.IsActive())
			f.Draw(&m_tex);
	}
	D3D.SetBlendState(BlendMode::Alpha);
}

void C_FireworksManager::Shot(Math::Vector2 pos)
{
	//使用していない花火をオブジェクトプール内から探す
	if (m_fireworksList.size() == m_poolSize)
	{
		for (auto& f : m_fireworksList)
		{
			if (!f.IsActive())
			{
				//花火を打ち上げる
				f.Shot(pos);
				//処理を終わる
				return;
			}
		}
	}
	//新しく花火オブジェクトを作成
	m_fireworksList.emplace_back();
	m_fireworksList.back().Shot(pos);
}

void C_FireworksManager::Release()
{
	m_fireworksList.clear();
}
