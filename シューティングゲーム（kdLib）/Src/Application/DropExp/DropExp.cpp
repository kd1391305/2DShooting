#include"DropExp.h"

void DropExp::Init()
{

}

void DropExp::Update()
{

}

void DropExp::Draw()
{
	SHADER.m_spriteShader.ClearMatrix();
	//ドロップした経験値の描画
	for (auto d : m_dropList)
	{
		SHADER.m_spriteShader.DrawCircle(d.x, d.y, m_radius, &Math::Color{ 1.0f,0.5f,0.5f,1.0f }, true);
		SHADER.m_spriteShader.DrawCircle(d.x, d.y, m_radius + 1, &Math::Color{ 1.0f,0.5f,0.5f,1.0f }, true);
	}
}


