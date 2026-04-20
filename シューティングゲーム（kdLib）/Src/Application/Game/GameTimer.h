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
		m_beforeFps = 60;
	}

	//更新
	void Update()
	{
		//経過したフレーム数を測る
		m_timer++;

		//Stop()が前のゲームループで呼ばれていたら、処理を止める
		if (m_stopTime)
		{
			Sleep(m_stopTime);
			m_stopTime = 0;
		}
	}

	//ミリ秒単位で停止する（実際に停止処理を行うのはUpdate()の中）
	void Stop(int ms) { m_stopTime = ms; }

	//時間を遅くする（何フレームの間、fps）
	void Slow(int frame, float slow);

	//経過したフレーム数
	long ElapsedTime() { return m_timer; }

	//ゲーム終了時間を返す
	int GetGameEndTime() { return m_gameEndTime; }

private:
	int m_stopTime;

	long m_timer;

	int m_slowCnt;			//遅くなる時間をカウントする（0以下になったら元に戻る）
	int m_beforeFps;		//遅くなる以前のfpsを保存しておく

	const int m_gameEndTime = 60;

	//シングルトン
private:
	//メンバ変数の初期化
	C_GameTimer():
	m_stopTime(0),m_timer(0),m_slowCnt(0),m_beforeFps(60)
	{}
public:
	static C_GameTimer& GetInstance()
	{
		static C_GameTimer instance;
		return instance;
	}
};

#define GAME_TIMER C_GameTimer::GetInstance()