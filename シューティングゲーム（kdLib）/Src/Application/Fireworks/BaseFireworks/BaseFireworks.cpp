#include"BaseFireworks.h"
#include"../../main.h"

bool BaseFireworks::IsScreenOut()
{
	float left = m_pos.x - 30;
	float right = m_pos.x + 30;
	float top = m_pos.y + 30;
	float bottom = m_pos.y - 30;
	return left < SCREEN_LEFT || right > SCREEN_RIGHT || top > SCREEN_TOP || bottom < SCREEN_BOTTOM;
}