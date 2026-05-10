#include"AttackAreaManager.h"
#include"AttackArea.h"
#include"../../../../TextureCache/TextureCache.h"

void AttackAreaManager::Update(float deltaTime)
{
	for (int i = m_list.size() - 1; i >= 0; i--)
	{
		m_list[i]->Update(deltaTime);
		if(!m_list[i]->IsActive())
		{
			m_list.erase(m_list.begin() + i);
		}
	}
}

void AttackAreaManager::Draw()
{
	KdTexture* tex = TextureCache::Instance().Get("Texture/Enemy/Boss/AttackArea.png").get();
	for (auto& area : m_list)
	{
		area->Draw(tex);
	}
}

std::shared_ptr<AttackArea> AttackAreaManager::Emit(Math::Vector2& pos, Math::Vector2& radius)
{
	std::shared_ptr<AttackArea>area;
	area = std::make_shared<AttackArea>();
	area->Emit(pos, radius);
	m_list.push_back(area);
	return area;
}
