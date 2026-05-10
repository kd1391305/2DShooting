#pragma once

class Score
{
public:

	//コンストラクタ
	Score():m_score(0),m_highScore(0)
	{
		LoadHighScore();
	}

	//デストラクタ
	~Score();

	void Init();

	void Update();
	
	void Draw();

	//スコアを追加する
	void Add(int addScore);
	long Get() { return m_score; }

	long GetHighScore() {
		while (!m_waitScore.empty())
		{
			m_score += m_waitScore.front();
			m_waitScore.pop();
		}
		if (m_highScore > m_score)
			return m_highScore;
		else return m_score;
	}
	
	//打ち上げ数をカウント
	void AddExplodeNum(int add) { m_explodeNum += add; }
	long GetExplodeNum() { return m_explodeNum; }

private:

	//ハイスコアを読み込む(ハイスコアを返す)
	void LoadHighScore();

	//ハイスコアをセーブする
	bool SaveHighScore();

	long m_score=0;			//現在のスコア
	long m_highScore=0;		//最大スコア

	std::queue<int>m_waitScore;			//加算するのを待っているスコア待機列
	static const int s_waitCnt = 6;		//6フレーム間で平等にスコアを加算する

	long m_explodeNum = 0;
};
