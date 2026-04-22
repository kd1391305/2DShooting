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