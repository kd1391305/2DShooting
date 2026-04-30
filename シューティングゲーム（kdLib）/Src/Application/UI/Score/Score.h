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
	
	void Draw();

	//スコアを追加する
	void Add(int addScore) { m_score += addScore; }

	long Get() { return m_score; }
	
private:

	//ハイスコアを読み込む(ハイスコアを返す)
	void LoadHighScore();

	//ハイスコアをセーブする
	bool SaveHighScore();

	long m_score;			//現在のスコア
	long m_highScore;		//最大スコア
};
