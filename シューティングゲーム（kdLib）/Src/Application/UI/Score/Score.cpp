#include"Score.h"
#include"../../Font/DWriteCustom.h"

//デストラクタ
Score::~Score()
{
	//ハイスコアを書き換える
	if (m_score > m_highScore)
	{
		m_highScore = m_score;
		SaveHighScore();
	}
}

void Score::Init()
{
	LoadHighScore();
}

void Score::Draw()
{
	const float tempFontSize = DWriteCustom::Instance().GetFontSize();
	DWriteCustom::Instance().SetFontSize(20);

	DWriteCustom::Instance().Draw("      Score :", { -350,330 });
	DWriteCustom::Instance().Draw("HighScore :", { -350,300 });

	//現在のスコアを描画
	char score[100];
	sprintf_s(score, sizeof(score), "%.8d", m_score);
	DWriteCustom::Instance().Draw(score, { -230,330 });

	//ハイスコアを描画する
	char highScore[100];
	sprintf_s(highScore, sizeof(highScore), "%.8d", m_highScore);
	DWriteCustom::Instance().Draw(highScore, { -230,300 });

	//フォントを元に戻す
	DWriteCustom::Instance().SetFontSize(tempFontSize);
}

void Score::LoadHighScore()
{
	FILE* fp;
	if ((fp = fopen("Data/HighScore.csv", "r")) != nullptr)
	{
		//欲しいレベルのハイスコアを読み込む
		fscanf_s(fp, "%ld", &m_highScore);
		fclose(fp);
	}
}

bool Score::SaveHighScore()
{
	FILE* fp;

	//ハイスコアを新たに書き込む
	if ((fp = fopen("Data/HighScore.csv", "w")) != nullptr)
	{
		for (int i = 0; i < 5; i++)
		{
			fprintf_s(fp, "%ld", m_highScore);
		}
		fclose(fp);
		//セーブが出来た
		return true;
	}

	//セーブができなかった
	return false;
}