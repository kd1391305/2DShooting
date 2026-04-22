#include"Score.h"
#include"../../Toolkit/Font/DrawString.h"
#include"../../Save/Save.h"

//デストラクタ
C_Score::~C_Score()
{
	//ハイスコアを書き換える
	if (m_score > m_highScore)
		C_Save::SaveHighScore(m_score, m_level);
}

void C_Score::Init(int level)
{
	//level情報をこのクラスでも保有しておく
	//このクラスがdeleteされるときのセーブに使用
	m_level = level;
	m_highScore = C_Save::LoadHighScore(m_level);
}

void C_Score::Draw()
{
	const float tempFontSize = DRAW_STRING.GetFontSize();
	DRAW_STRING.SetFontSize(20);


	//現在のスコアを描画する
	char score[100];
	sprintf_s(score, sizeof(score), "     Score : %.8d", m_score);
	DRAW_STRING.Draw(
		score,						//描画するスコア文字列 
		{ -347,330 });			//描画位置

	//ハイスコアを描画する
	char highScore[100];
	sprintf_s(highScore, sizeof(highScore), "HighScore : %.8d", m_highScore);
	DRAW_STRING.Draw(
		highScore,				//描画するスコア文字列 
		{-350,300});			//描画位置

	DRAW_STRING.SetFontSize(tempFontSize);
}