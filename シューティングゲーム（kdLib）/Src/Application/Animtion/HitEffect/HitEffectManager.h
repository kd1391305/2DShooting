#pragma once

class HitEffect;

class HitEffectManager
{
public:

	void Update(float deltaTime);

	void Draw();

	void Emit(Math::Vector2& pos,Math::Vector2&move);

private:

	std::vector<std::shared_ptr<HitEffect>> m_list;
};
