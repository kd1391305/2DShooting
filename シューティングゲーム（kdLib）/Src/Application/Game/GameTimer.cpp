#include"GameTimer.h"
#include"../main.h"

void C_GameTimer::Update()
{
	//経過したフレーム数を測る
	m_timer++;

	//Stop()が前のゲームループで呼ばれていたら、処理を止める
	if (m_stopTime)
	{
		Sleep(m_stopTime);
		m_stopTime = 0;
	}

	m_slowCnt--;
	//遅くなるframe数を測り終えたら
	if (m_slowCnt == 0)
		APP.m_maxFps = 60;		//Fpsを元に戻す
}

void C_GameTimer::Slow(int frame, float slow)
{
	m_slowCnt = frame;

	APP.m_maxFps = 60 * slow;
}
