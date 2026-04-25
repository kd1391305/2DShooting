#include"BaseFireworks.h"
#include"../../main.h"

bool BaseFireworks::IsScreenOut()
{
	float left = m_pos.x - m_radius;
	float right = m_pos.x + m_radius;
	float top = m_pos.y + m_radius;
	float bottom = m_pos.y - m_radius;
	return left < SCREEN_LEFT || right > SCREEN_RIGHT || top > SCREEN_TOP || bottom < SCREEN_BOTTOM;
}