#pragma once

class HUDBack
{
public:
	HUDBack();

	void Draw();
private:
	KdTexture tex;
	Math::Vector2 pos;
	Math::Matrix m_mat;
};
