#include"Mouse.h"
#include"../main.h"

void Mouse::Update()
{
	//ディスプレイ上のマウス座標取得
	GetCursorPos(&m_pos);

	//ウィンドウ上のマウス座標に変換（ウィンドウ左上(0,0)）
	ScreenToClient(APP.m_window.GetWndHandle(), &m_pos);

	//マウス座標を補正（ウィンドウの中心（0,0））
	m_pos.x += SCREEN_LEFT;
	m_pos.y += SCREEN_BOTTOM;
	m_pos.y *= -1;

	Math::Matrix scale = Math::Matrix::CreateScale(0.3f, 0.3f, 0);
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scale * trans;
}

void Mouse::Draw()
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex_Src(&m_tex);
}

bool Mouse::Hit(Math::Vector2 pos, Math::Vector2 radius)
{
	float left = pos.x - radius.x;
	float right = pos.x + radius.x;
	float top = pos.y + radius.y;
	float bottom = pos.y - radius.y;
	if (left < m_pos.x && right > m_pos.x &&
		top > m_pos.y && bottom < m_pos.y)
	{
		return true;
	}
	return false;
}