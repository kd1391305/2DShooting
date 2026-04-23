#pragma once

class Save
{
public:
	//ハイスコアを読み込む(ハイスコアを返す)
	static long LoadHighScore();

	//ハイスコアをセーブする
	static bool SaveHighScore(long highScore);
};


