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
	m_score = 0;
	m_highScore = 0;
	LoadHighScore();
}

void Score::Update()
{
	//スコア加算の待機配列の先頭の値を取り出す
	if (!m_waitScore.empty())
	{
		m_score += m_waitScore.front();
		m_waitScore.pop();
	}
}

void Score::Draw()
{
	const float tempFontSize = DWriteCustom::Instance().GetFontSize();
	DWriteCustom::Instance().SetFontSize(13);

	D2D1_COLOR_F tempColor = DWriteCustom::Instance().GetColor();
	DWriteCustom::Instance().SetShadow({ -1,-1 }, { 0.0f, 0.7f, 0.7f, 0.7f });

	DWriteCustom::Instance().Draw("      Score :", { -370,340 });
	DWriteCustom::Instance().Draw("HighScore :", { -370,310 });
	DWriteCustom::Instance().SetShadow({}, {});
	

	DWriteCustom::Instance().SetFontSize(20);
	DWriteCustom::Instance().ChangeFont(FontName::Orbitron);
	//現在のスコアを描画
	{
		char score[10];
		sprintf_s(score, sizeof(score), "%.8ld", m_score);
		float scorePosY = 345;
		//一桁ずつ描画
		for (int i = 0; i < 8; i++)
		{
			std::string digit;
			digit = score[i];
			if (score[i] == '1')
			{
				//+4右にずらす
				DWriteCustom::Instance().Draw(digit, { -290 + i * 18.0f + 4,scorePosY });
			}
			else
			{
				DWriteCustom::Instance().Draw(digit, { -290 + i * 18.0f,	scorePosY });
			}
		}
	}
	//ハイスコアを描画する
	{
		char highScore[10];
		sprintf_s(highScore, sizeof(highScore), "%.8ld", m_highScore);
		float highScorePosY = 315;
		//一桁ずつ描画
		for (int i = 0; i < 8; i++)
		{
			std::string digit;
			digit = highScore[i];
			if (highScore[i] == '1')
			{
				//+4右にずらす
				DWriteCustom::Instance().Draw(digit, { -290 + i * 18.0f + 4,highScorePosY });
			}
			else
			{
				DWriteCustom::Instance().Draw(digit, { -290 + i * 18.0f,	highScorePosY });
			}
		}
	}
	//フォントを元に戻す
	DWriteCustom::Instance().ChangeFont(FontName::KleeOne);
	DWriteCustom::Instance().SetFontSize(tempFontSize);
}

void Score::Add(int addScore)
{
	//スコアをs_waitCnt分に分割し、分割したスコアを毎フレーム足していく
	int temp[s_waitCnt];		//分割したスコアの一時的な入れ物

	int sum = 0;
	for (int i = 0; i < s_waitCnt; i++)
	{
		temp[i] = addScore / s_waitCnt;
		sum += temp[i];
	}
	//切り捨てられた値があったら
	if (sum < addScore)
	{
		//切り捨てられた分を最後の分割したスコアに加算
		temp[s_waitCnt - 1] += addScore - sum;
	}

	//スコア加算の待機配列に値を追加
	for (int i = 0; i < s_waitCnt; i++)
	{
		m_waitScore.push(temp[i]);
	}
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
		fprintf_s(fp, "%ld", m_highScore);
		fclose(fp);
		//セーブが出来た
		return true;
	}

	//セーブができなかった
	return false;
}