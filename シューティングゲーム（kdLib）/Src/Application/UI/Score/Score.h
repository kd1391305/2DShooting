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
	
private:

	//ハイスコアを読み込む(ハイスコアを返す)
	void LoadHighScore();

	//ハイスコアをセーブする
	bool SaveHighScore();

	long m_score=0;			//現在のスコア
	long m_highScore=0;		//最大スコア

	std::queue<int>m_waitScore;			//加算するのを待っているスコア待機列
	static const int s_waitCnt = 6;		//6フレーム間で平等にスコアを加算する
};
