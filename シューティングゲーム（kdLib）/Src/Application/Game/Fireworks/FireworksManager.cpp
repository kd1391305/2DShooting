#include"FireworksManager.h"

//更新
void C_FireworksManager::Update()
{
	for (auto& f : m_fireworksList)
	{
		if(f.IsActive())
			f.Update();
	}
}

//描画
void C_FireworksManager::Draw()
{
	//加算合成で描画
	D3D.SetBlendState(BlendMode::Add);

	for (auto& f : m_fireworksList)
	{
		if (f.IsActive())
			f.Draw(&m_tex);
	}

	//元に戻す
	D3D.SetBlendState(BlendMode::Alpha);
}

//花火を打ち上げる
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
