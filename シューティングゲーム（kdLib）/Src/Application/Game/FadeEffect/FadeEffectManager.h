#pragma once
#include"FadeEffect.h"

class C_FadeEffectManager
{
public:

	void Update()
	{
		for (auto& f : m_fadeEffect)
		{
			if (f.IsActive())
				f.Update();
		}
	}

	void Draw()
	{
		for (auto& f : m_fadeEffect)
		{
			if (f.IsActive())
				f.Draw();
		}
	}

	//フェードエフェクトを発生させる
	void Emit(Math::Vector2 pos, float radius)
	{
		for (auto& f : m_fadeEffect)
		{
			if (!f.IsActive())
			{
				f.Emit(pos, radius);
			}
		}
	}


private:

	std::vector<C_FadeEffect> m_fadeEffect;
	int m_objectPoolSize = 10;

	//シングルトン
private:
	C_FadeEffectManager() 
	{
		//画像を読み込む
		C_FadeEffect::LoadTex();

		//オブジェクトプールを作成しておく
		for (int i = 0; i < m_objectPoolSize; i++)
		{
			m_fadeEffect.push_back(C_FadeEffect());
		}
	}
public:
	static C_FadeEffectManager& GetInstance()
	{
		static C_FadeEffectManager instance;
		return instance;
	}
};

#define FADE_EFFECT C_FadeEffectManager::GetInstance()
