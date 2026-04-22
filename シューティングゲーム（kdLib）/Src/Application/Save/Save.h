#pragma once

class C_Save
{
public:
	//ハイスコアを読み込む(ハイスコアを返す)
	static long LoadHighScore(int level);

	//ハイスコアをセーブする
	static bool SaveHighScore(long highScore, int level);
};


