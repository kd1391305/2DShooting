#pragma once

class C_GameTimer
{
public:

	//初期化
	void Init()
	{
		m_stopTime = 0;
		m_timer = 0;
		m_slowCnt = 0;
	}

	//更新
	void Update();
	

	//ミリ秒単位で停止する（実際に停止処理を行うのはUpdate()の中）
	void Stop(int ms) { m_stopTime = ms; }

	//時間を遅くする（何フレームの間、減速率）
	void Slow(int frame, float slow);

	//経過したフレーム数
	long ElapsedTime() { return m_timer; }

	//ゲーム終了時間を返す
	int GetGameEndTime() { return m_gameEndTime; }

	long GetTime() { return m_timer; }

private:
	int m_stopTime;

	long m_timer;

	int m_slowCnt;			//遅くなる時間をカウントする（0以下になったら元に戻る）

	const int m_gameEndTime = 60;

	//シングルトン
private:
	//メンバ変数の初期化
	C_GameTimer():
	m_stopTime(0),m_timer(0),m_slowCnt(0)
	{}
public:
	static C_GameTimer& GetInstance()
	{
		static C_GameTimer instance;
		return instance;
	}
};

#define GAME_TIMER C_GameTimer::GetInstance()