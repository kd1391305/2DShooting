#include"HitEffectManager.h"
#include"HitEffect.h"
#include"../../TextureCache/TextureCache.h"

void HitEffectManager::Update(float deltaTime)
{
	for (int i = m_list.size() - 1; i >= 0; i--)
	{
		if (!m_list[i]->Update(deltaTime))
		{
			m_list.erase(m_list.begin() + i);
		}
	}
}

void HitEffectManager::Draw()
{
	std::shared_ptr<KdTexture>tex = TextureCache::Instance().Get("Texture/Effect/HitEffect.png");
	for (auto& effect : m_list)
	{
		effect->Draw(tex);
	}
}

void HitEffectManager::Emit(Math::Vector2& pos,Math::Vector2&move)
{
	std::shared_ptr<HitEffect> effect = std::make_shared<HitEffect>();
	effect->Emit(pos,move);
	m_list.push_back(effect);
}
