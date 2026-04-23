#include "HUDBack.h"
#include"../../main.h"

HUDBack::HUDBack()
{
	
}

void HUDBack::Draw()
{
	SHADER.m_spriteShader.ClearMatrix();
	Math::Vector2 radius = { 280,60 };
	float cx = SCREEN_LEFT + radius.x;
	float cy = SCREEN_TOP - radius.y;
	SHADER.m_spriteShader.DrawBox(cx, cy, radius.x, radius.y, &Math::Color{ 0.3f,0.3f,0.3f,0.6f });
}
