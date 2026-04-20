#include "Button.h"
#include"../Mouse/Mouse.h"

void C_Button::Update()
{
	Math::Matrix scaleMat, transMat;
		
	//マウスと当たったらサイズを変更する
	if (MOUSE.Hit(m_pos, m_radius))
	{
		m_bSelect = true;
		scaleMat = Math::Matrix::CreateScale(m_selectScale.x, m_selectScale.y, 0);
	}
	else
	{
		m_bSelect= false;
		scaleMat = Math::Matrix::CreateScale(1, 1, 0);
	}
	transMat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scaleMat * transMat;
}

void C_Button::Draw()
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	if (m_bSelect )		SHADER.m_spriteShader.DrawBox(0, 0, m_radius.x, m_radius.y, & m_selectColor, true);
	else						SHADER.m_spriteShader.DrawBox(0, 0, m_radius.x, m_radius.y, &m_color,				true);
}

