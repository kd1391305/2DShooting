#pragma once

class C_Score
{
public:

	//コンストラクタ
	C_Score():m_score(0),m_highScore(0)
	{
	}

	//デストラクタ
	~C_Score();

	void Init(int level);
	
	void Draw();

	//スコアを追加する
	void Add(int addScore) { m_score += addScore; }

	//現在のスコアを獲得する
	long GetScore() { return m_score; }
	//最大スコアを獲得する
	long GetHighScore() { return m_highScore; }

	//最大スコアをセットする（セーブデータ読み込み等）
	void SetHighScore(long set) { m_highScore = set; }

private:

	long m_score;			//現在のスコア
	long m_highScore;		//最大スコア
	int m_level;				//現在挑戦中のレベル
};
