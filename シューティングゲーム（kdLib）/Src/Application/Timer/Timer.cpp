#include"Timer.h"
#include"../main.h"

void Timer::Update()
{
	//１フレームにかかった時間を計算する

	//現在の時刻を入手
	DWORD nowTime = timeGetTime();

	//経過した時間（秒）を入手
	m_deltaTime = (nowTime - m_beforeTime) / 1000.0;

	//次のUpdate()に使用する時間を保存しておく
	m_beforeTime = nowTime;

	//ストップ時間が残っていたら
	if (m_stopTime > 0)
	{
		//ストップした時間は経過時間に含まないようにする
		m_stopTime -= m_deltaTime;
		if (m_stopTime <= 0)
		{
			m_stopTime = 0;
		}
	}

	//もし、経過時間がマイナスになったら、その時は60fpsでの経過時間にする(49日に一回あるtimeGetTime()で帰ってくる値が０からスタートするため)
	if (m_deltaTime < 0)
	{
		m_deltaTime = 1 / 60.0f;
	}

	//もし、経過時間が0.3秒以上かかったのなら,60fpsでの経過時間にする
	if (m_deltaTime >= 0.3)
	{
		m_deltaTime = 1 / 60.0f;
	}

	//遅くなる処理
	if (m_slow)
	{
		//遅くなる時間を測るタイマーを進める
		m_slowTimer -= m_deltaTime;

		//経過時間を遅くする
		m_deltaTime *= m_slow;

		//遅くなる時間を測り終えたら
		if (m_slowTimer <= 0)
		{
			m_slow = 0;
		}
	}

	//経過したフレーム数を測る
	m_timer += m_deltaTime;

}

void Timer::Slow(float time, float slow)
{
	m_slowTimer = time;
	m_slow = slow;
}
