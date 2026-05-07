#pragma once

class HitEffect
{
public:

	bool Update(float deltaTime);

	void Draw(std::shared_ptr<KdTexture> tex);

	void Emit(Math::Vector2& pos,Math::Vector2&move);

private:

	static const int	s_animSpeed = 16;
	float		m_animTimer = 0;
	static const int	s_animTimeEnd = 7;

	static constexpr float s_scale = 0.8f;
	static const int s_animDiameter = 64;
	Math::Matrix m_mat;
	Math::Vector2 m_pos;
	Math::Vector2 m_move;
	Math::Color m_color;
};
