#pragma once

//特定の場所を明るくするためだけのクラス
//シングルトンパターン
class Light
{
public:

	void Draw(Math::Vector2 pos, Math::Vector2 radius,Math::Color&color);

private:

	KdTexture m_tex;

	Light()
	{
		m_tex.Load("Texture/Light.png");
	}

public:

	static Light& Instance()
	{
		static Light instance;
		return instance;
	}
};
