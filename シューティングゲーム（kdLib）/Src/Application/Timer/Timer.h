#pragma once

class Timer
{
public:

	//初期化(リセット)
	void Reset()
	{
		m_timer = 0;

		m_stopTime = 0;
		
		m_slowTimer = 0;
		m_slow = 0.0f;
	}

	//更新
	void Update();

	//秒単位で停止する（実際に停止処理を行うのはUpdate()の中）
	void Stop(float s) {
		m_stopTime = s;
	}

	//時間を遅くする（何秒間、減速率）
	void Slow(float time, float slow);

	//経過した時間（秒）を返す
	float GetTime() { return m_timer; }

	//１フレームで経過した時間を返す
	float GetDeltaTime(){ return m_deltaTime; }

	float GetStopTime() { return m_stopTime; }

private:

	float m_timer;			//経過した時間（秒）を数える（ただし、このクラス内で行われるSleep()は時間に含めない、Slow()は遅くした時間で数える）

	float m_stopTime;		//止める時間（秒）を保存

	float m_slowTimer;		//遅くなる時間(秒)をカウントする（0以下になったら元に戻る）
	float m_slow;			//遅くなる割合

	DWORD m_beforeTime;	//前回の時間
	float m_deltaTime;		//前回と比べて何秒経過したか（ただし、Stop関数で止められた秒数は省く）
	
	int m_cnt;
	float m_sumTime;

	//シングルトン
private:
	//メンバ変数の初期化
	Timer():
	m_timer(0),m_stopTime(0),m_slowTimer(0),m_slow(0),m_beforeTime(0),m_deltaTime(0),m_cnt(0),m_sumTime(0)
	{}
public:
	static Timer& Instance()
	{
		static Timer instance;
		return instance;
	}
};