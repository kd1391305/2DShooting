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
	//===================================
	//プレイヤーに関するファイルを開く
	//===================================

	//毎フレーム書き込むファイルを作る
	sprintf_s(name, sizeof(name), "Data/SavePlayer%d.csv", level);
	if ((m_playerPos.write = fopen(name, "w")) == nullptr)
		//ファイルを開けなかったら（作れなかったら）
		return false;

	//毎フレーム読み込むファイルを開く
	for (int i = 1; i < level; i++)
	{
		sprintf_s(name, sizeof(name), "Data/SavePlayer%d.csv", i);
		if ((m_playerPos.read[i - 1] = fopen(name, "r")) == nullptr)
			//ファイルを開けなかったら
			return false;
	}

	//===================================
	//敵に関するファイルを開く
	//===================================

	//毎フレーム書き込むファイルを作る
	sprintf_s(name, sizeof(name), "Data/SaveEnemy%d.csv", level);
	if ((m_enemy.write = fopen(name, "w")) == nullptr)
		//ファイルを開けなかったら（作れなかったら）
		return false;

	//毎フレーム読み込むファイルを開く
	for (int i = 1; i < level; i++)
	{
		sprintf_s(name, sizeof(name), "Data/SaveEnemy%d.csv", i);
		if ((m_enemy.read[i - 1] = fopen(name, "r")) == nullptr)
			//ファイルを開けなかったら
			return false;
		else
			//次に出てくるデータを読み込む
			fscanf_s(m_enemy.read[i - 1], "%f,%f,%ld\n", &m_nextSpawnPos[i - 1].x, &m_nextSpawnPos[i - 1].y, &m_nextSpawnTime[i - 1]);
	}

	//===================================
	//弾に関するファイルを開く
	//===================================
	//毎フレーム書き込むファイルを作る
	sprintf_s(name, sizeof(name), "Data/SavePB%d.csv", level);
	if ((m_shotTime.write = fopen(name, "w")) == nullptr)
		//ファイルを開けなかったら（作れなかったら）
		return false;

	//毎フレーム読み込むファイルを開く
	for (int i = 1; i < level; i++)
	{
		sprintf_s(name, sizeof(name), "Data/SavePB%d.csv", i);
		if ((m_shotTime.read[i - 1] = fopen(name, "r")) == nullptr)
			//ファイルを開けなかったら
			return false;
		else
			//次に出てくるデータを読み込む
			fscanf_s(m_shotTime.read[i - 1], "%ld", &m_nextShotTime[i - 1]);
	}



	//ファイルを問題なく開けた
	return true;
}

//ファイルを閉じる
void C_Save::Close()
{
	//プレイヤー
	for (auto p : m_playerPos.read)
		if (p)
		{
			fclose(p);
			p = nullptr;
		}
	if (m_playerPos.write)
	{
		fclose(m_playerPos.write);
		m_playerPos.write = nullptr;
	}

	//敵
	for (auto e : m_enemy.read)
		if (e)
		{
			fclose(e);
			e = nullptr;
		}
	if (m_enemy.write)
	{
		fclose(m_enemy.write);
		m_enemy.write = nullptr;
	}
}

bool C_Save::WritePlayerPos(Math::Vector2 pos)
{
	fprintf_s(m_playerPos.write, "%f, %f\n", pos.x, pos.y);
	return true;
}

bool C_Save::ReadPlayerPos(Math::Vector2* pPos,int level)
{
	if (fscanf_s(m_playerPos.read[level - 1], "%f,%f\n", &pPos->x, &pPos->y) != EOF)
	{
		//読み取り位置を次の行にする
		char dummy[100];
		fgets(dummy, sizeof(dummy), m_playerPos.read[level - 1]);
		return true;
	}

	//EOFに到達したか、問題が発生した
	return false;
}

bool C_Save::WriteEnemy(Math::Vector2 pos, long time)
{
	fprintf_s(m_enemy.write, "%f, %f,%ld\n", pos.x, pos.y, time);
	return true;
}

bool C_Save::SearchSpawnEnemy(long time, int level)
{
	if (time == m_nextSpawnTime[level - 2])
		return true;

	//見つからなかったらfalse
	return false;
}

Math::Vector2 C_Save::PopSpawnEnemy(int level)
{
	//返す座標を取り出す
	const Math::Vector2 temp = m_nextSpawnPos[level - 2];

	char dummy[100];
	fgets(dummy, sizeof(dummy), m_enemy.read[level - 2]);

	//取り出したので、次の座標をセットする
	if (fscanf_s(m_enemy.read[level - 2], "%f,%f,%ld", &m_nextSpawnPos[level - 2].x, &m_nextSpawnPos[level - 2].y, &m_nextSpawnTime[level - 2]) == EOF)
	{
		//取り出すものが無かったら、初期化する
		m_nextSpawnPos[level - 2] = { 0,0 };
		m_nextSpawnTime[level - 2] = 0;
	}

	//取り出した座標を返す
	return temp;
}

bool C_Save::WritePlayerShotTime(long time)
{
	fprintf_s(m_shotTime.write, "%ld\n", time);
	return true;
}

//プレイヤーが弾を出す時間を返す
long C_Save::GetPlayerShotTime(int level)
{
	return m_nextShotTime[level - 1];
}

void C_Save::PopPlayerShotTime(int level)
{
	//読み込む位置を一行ずらす
	char dummy[100];
	fgets(dummy, sizeof(dummy), m_shotTime.read[level - 1]);
	//読み込む（前のデータを上書き）
	fscanf_s(m_shotTime.read[level - 1], "%ld", &m_nextShotTime[level - 1]);
}

void C_Save::CheckReleaseLevel()
{
	m_releaseLevel = 1;
	for (int i = 0; i < 4; i++)
	{
		FILE* fp;
		char name[50];
		sprintf_s(name, sizeof(name), "Data/SavePlayer%d.csv", i);
		//ファイルを開く
		if ((fp = fopen(name, "r")) != nullptr)
		{
			//解放レベルをあげる
			m_releaseLevel++;
			fclose(fp);
		}
	}
}
