#pragma once

class C_HUDBack
{
public:
	C_HUDBack();

	void Draw();
private:
	KdTexture tex;
	Math::Vector2 pos;
	Math::Matrix m_mat;
};
