#include "Button.h"
#include"../../Mouse/Mouse.h"
#include"../../TextureCache/TextureCache.h"

void Button::Update()
{
	if (MOUSE.Hit(m_pos, m_radius))
	{
		if (MOUSE.IsMove())
		{
			m_bSelect = true;
			m_actionType = ActionType::Mouse;
		}
	}
	else
	{
		if (m_actionType == ActionType::Mouse)
		{
			m_bSelect = false;
		}
	}

	Math::Matrix scaleMat, transMat;
		
	//‘I‘ð’†‚Å‚ ‚é‚È‚ç
	if (m_bSelect)
	{
		scaleMat = Math::Matrix::CreateScale(m_selectScale.x, m_selectScale.y, 0);
	}
	else
	{
		scaleMat = Math::Matrix::CreateScale(m_scale.x, m_scale.y, 0);
	}
	transMat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scaleMat * transMat;
}

void Button::Draw()
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	if (m_bSelect)		SHADER.m_spriteShader.DrawBox(0, 0, m_radius.x, m_radius.y, & m_selectColor, true);
	else							SHADER.m_spriteShader.DrawBox(0, 0, m_radius.x, m_radius.y, &m_color,				true);
}

void Button::SetSelect(bool set)
{
	m_bSelect = set;
	m_actionType = ActionType::Key;
}

void ButtonEx::SetTex(std::string path)
{
	m_texPath = path;
	std::shared_ptr<KdTexture> tex = TextureCache::Instance().Get(m_texPath);

	m_scale.x = m_radius.x / tex->GetRadius().x;
	m_scale.y = m_radius.y / tex->GetRadius().y;

	m_selectScale *= m_scale;
}

void ButtonEx::Draw()
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	if (m_bSelect)	SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get(m_texPath), m_selectColor);
	else			SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get(m_texPath), m_color);
}