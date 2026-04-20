#include"Back.h"
#include"../../main.h"

//çXêV
void C_Back::Update()
{
	m_sky.Update();
}

//ï`âÊ
void C_Back::Draw()
{
	SHADER.m_spriteShader.ClearMatrix();
	SHADER.m_spriteShader.DrawBox(0, 0, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, &Math::Color{ 0.7f,0.7f,0.7f,1.0f }, true);

	m_sky.Draw();
}