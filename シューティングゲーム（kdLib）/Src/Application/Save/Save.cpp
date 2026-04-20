#include "Save.h"

long C_Save::LoadHighScore(int level)
{
	long highScore = 0;
	FILE* fp;
	if ((fp = fopen("Data/HighScore.csv", "r")) != nullptr)
	{
		char dummy[100];
		//欲しいレベルまでのハイスコアを読み飛ばす
		for (int i = 0; i < level - 1; i++)
		{
			//EOFに到達したら（ハイスコアを0で返す）
			if ((fgets(dummy, sizeof(dummy), fp)) == NULL)
			{
				fclose(fp);
				highScore = 0;
				return highScore;
			}
		}

		//欲しいレベルのハイスコアを読み込む
		if (fscanf_s(fp, "%ld", &highScore) == EOF)
		{
			fclose(fp);
			highScore = 0;
			return highScore;
		}

		fclose(fp);
	}
	return highScore;
}

bool C_Save::SaveHighScore(long highScore, int level)
{
	long tempHighScore[5] = { 0,0,0,0,0 };
	FILE* fp;
	
	//ハイスコア一覧を読み込む
	char dummy[100];
	if ((fp = fopen("Data/HighScore.csv", "r")) != nullptr)
	{
		for (int i = 0; i < 5; i++)
		{
			if (fscanf_s(fp, "%ld", &tempHighScore[i]) == EOF)
				break;
			if (fgets(dummy, sizeof(dummy), fp) == NULL)
				break;
		}
		fclose(fp);
	}

	//ハイスコアを入れ替える
	tempHighScore[level - 1] = highScore;

	//ハイスコアを新たに書き込む
	if ((fp = fopen("Data/HighScore.csv", "w")) != nullptr)
	{
		for (int i = 0; i < 5; i++)
		{
			fprintf_s(fp, "%ld\n", tempHighScore[i]);
		}
		fclose(fp);
	}

	//セーブが出来た
	return true;
}

bool C_Save::Open(int level)
{
	char name[50];
	//毎フレーム書き込むファイルを作る
	sprintf_s(name, sizeof(name), "Data/Save%d.csv", level);
	if ((m_playerPos.write = fopen(name, "w")) == nullptr)
		//ファイルを開けなかったら（作れなかったら）
		return false;

	//毎フレーム読み込むファイルを開く
	for (int i = 1; i < level; i++)
	{
		sprintf_s(name, sizeof(name), "Data/Save%d.csv", i);
		if ((m_playerPos.read[i - 1] = fopen(name, "r")) == nullptr)
			//ファイルを開けなかったら
			return false;
	}

	//ファイルを問題なく開けた
	return true;
}

bool C_Save::Write(Math::Vector2 pos)
{
	fprintf_s(m_playerPos.write, "%f, %f\n", pos.x, pos.y);
	return true;
}

bool C_Save::Read(Math::Vector2* pPos,int level)
{
	if (fscanf_s(m_playerPos.read[level - 1], "%f,%f", &pPos->x, &pPos->y) != EOF)
	{
		//読み取り位置を次の行にする
		char dummy[100];
		fgets(dummy, sizeof(dummy), m_playerPos.read[level - 1]);
		return true;
	}

	//EOFに到達したか、問題が発生した
	return false;
}

//セーブファイルがあるかどうかを全てチェックする（フラグに情報を格納）
void C_Save::CheckFile()
{
	m_releaseLevel = 1;
	for (int i = 0; i < 4; i++)
	{
		FILE* fp;
		char name[50];
		sprintf_s(name, sizeof(name), "Data/Save%d.csv", i);
		//ファイルを開く
		if ((fp = fopen(name, "r")) != nullptr)
		{
			//解放レベルをあげる
			m_releaseLevel++;
			fclose(fp);
		}
	}
}
