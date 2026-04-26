#include "Light.h"

void Light::Draw(Math::Vector2 pos, Math::Vector2 radius,Math::Color&color)
{
	//•`‰æ‚·‚é‚Æ‚«‚ÌŠgk‚ğ‹‚ß‚é
	Math::Vector2 scale;
	scale.x = radius.x / (m_tex.GetInfo().Width / 2.0f);
	scale.y = radius.y / (m_tex.GetInfo().Height / 2.0f);

	//s—ñì¬
	Math::Matrix scaleMat, transMat, mat;
	scaleMat = Math::Matrix::CreateScale(scale.x, scale.y, 0);
	transMat = Math::Matrix::CreateTranslation(pos.x, pos.y, 0);
	mat = scaleMat * transMat;

	//•`‰æ
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex_Src(&m_tex, color);
}
