#include"FireworksManager.h"
#include"../Font/DWriteCustom.h"
#include"Fireworks.h"
#include"../SoundCache/SoundCache.h"
#include"../Light/Light.h"
#include"../main.h"

//更新
void FireworksManager::Update(float deltaTime)
{
	//花火が消されない場合、６秒経つごとに自動的に削除
	m_timer += deltaTime;
	if (m_timer >= 6)
	{
		m_timer = 0;
		if (!m_list.empty())
		{
			PushPool((Type)m_list.front()->GetType(), m_list.front());
			m_list.erase(m_list.begin());
		}
	}

	//花火
	for (int i = m_list.size() - 1; i >= 0; i--)
	{
		m_list[i]->Update(deltaTime);
		if (!m_list[i]->IsActive())
		{
			PushPool((Type)m_list[i]->GetType(), m_list[i]);
			m_list.erase(m_list.begin() + i);
		}
	}

	if (APP.m_fps < 50)
	{
		if (m_list.size() >= 15)
		{
			for (int i = 0; i < 5; i++)
			{
				PushPool((Type)m_list.front()->GetType(), m_list.front());
				m_list.erase(m_list.begin());
			}
		}
	}
}

//描画
void FireworksManager::Draw(float backScale)
{
	//加算合成で描画
	D3D.SetBlendState(BlendMode::Add);

	for (auto& f : m_list)
	{
		if (f->IsActive())
		{
			f->Draw(backScale);
		}
	}

	D3D.SetBlendState(BlendMode::Alpha);
}

//花火を打ち上げる
void FireworksManager::Shoot(FireworksManager::Type name, Math::Vector2& pos, Math::Vector2& startMove, float beforeScale, float afterScale, Math::Color& color,const float seVolume)
{
	m_list.push_back(PopPool(name));
	m_list.back()->Shoot(pos, startMove, beforeScale, afterScale, color);
	
	if (seVolume > 0)
	{
		std::shared_ptr<KdSoundInstance> se = SoundCache::Instance().Get("Sound/SE/Fireworks_Shot.wav", SoundCache::SoundState::NotPlaying);
		se->SetVolume(seVolume);
		se->Play(false);
	}
}

void FireworksManager::Explode(FireworksManager::Type name, Math::Vector2& pos, float afterScale, Math::Color& color,const float seVolume)
{
	m_list.push_back(PopPool(name));
	m_list.back()->Init_Explode(pos,afterScale,color);
	m_list.back()->Explode();
	if (seVolume > 0)
	{
		std::shared_ptr<KdSoundInstance> se = SoundCache::Instance().Get("Sound/SE/Fireworks_Explode.wav", SoundCache::SoundState::NotPlaying);
		se->SetVolume(seVolume);
		se->Play(false);
	}
}

//初期化（１回しか呼ばれない）
void FireworksManager::Init()
{
	for (int i = 0; i < m_poolSize; i++)
	{
		m_pool1.push_back(std::make_shared<Fireworks1>());
		m_pool1.back()->Init();
		m_pool3.push_back(std::make_shared<Fireworks3>());
		m_pool3.back()->Init();
		m_pool4.push_back(std::make_shared<Fireworks4>());
		m_pool4.back()->Init();
	}
}

//解放（１回しか呼ばれない）
void FireworksManager::Release()
{
	m_list.clear();
}

std::shared_ptr<BaseFireworks> FireworksManager::PopPool(Type type)
{
	std::shared_ptr<BaseFireworks> fireworks;
	switch (type)
	{
	case Type::Circle:
		if (m_pool1.empty())break;
		fireworks = m_pool1.back();
		m_pool1.pop_back();
		return fireworks;
	case Type::NewCircle:
		if (m_pool3.empty())break;
		fireworks = m_pool3.back();
		m_pool3.pop_back();
		return fireworks;
	case Type::Petal:
		if (m_pool4.empty())break;
		fireworks = m_pool4.back();
		m_pool4.pop_back();
		return fireworks;
	}

	//オブジェクトプールが空だったら、新しく作成する
	switch (type)
	{
	case Type::Circle:
		fireworks = std::make_shared<Fireworks1>();
		fireworks->Init();
		return fireworks;
	case Type::NewCircle:
		fireworks = std::make_shared<Fireworks3>();
		fireworks->Init();
		return fireworks;
	case Type::Petal:
		fireworks = std::make_shared<Fireworks4>();
		fireworks->Init();
		return fireworks;
	}
}

void FireworksManager::PushPool(Type type, std::shared_ptr<BaseFireworks> fireworks)
{
	if (type == Type::Circle)
	{
		auto p = std::dynamic_pointer_cast<Fireworks1>(fireworks);
		m_pool1.push_back(p);
	}
	else if (type == Type::NewCircle)
	{
		auto p = std::dynamic_pointer_cast<Fireworks3>(fireworks);
		m_pool3.push_back(p);
	}
	else if (type == Type::Petal)
	{
		auto p = std::dynamic_pointer_cast<Fireworks4>(fireworks);
		m_pool4.push_back(p);
	}
}
