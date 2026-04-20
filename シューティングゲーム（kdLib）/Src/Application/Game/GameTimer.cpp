#include"GameTimer.h"
#include"../main.h"

void C_GameTimer::Slow(int frame, float slow)
{
	m_slowCnt = frame;
	m_beforeFps = APP.m_maxFps;
	APP.m_maxFps *= slow;
}
