#pragma once

class C_Save
{
public:

	~C_Save()
	{
		Close();
	}

	//ハイスコアを読み込む
	long LoadHighScore(int level);

	//ハイスコアをセーブする
	bool SaveHighScore(long highScore, int level);

	//ファイルを開く
	bool Open(int level);
	
	//毎フレーム、プレイヤーの情報を記録する
	bool Write(Math::Vector2 pos);

	//毎フレーム、プレイヤーの情報を引き出す
	bool Read(Math::Vector2* pPos, int level);

	//ファイルを閉じる
	void Close()
	{
		if (m_playerPos.write)fclose(m_playerPos.write);
		m_playerPos.write = nullptr;
	}

	//セーブファイルがあるかどうかを全てチェックする（フラグに情報を格納）
	void CheckFile();

	//何レベルまで解放されているかを返す
	int GetReleaseLevel() { return m_releaseLevel; }

private:

	struct FP
	{
		FILE* write;		//毎フレーム、セーブするためのファイルポインター
		FILE* read[4];	//毎フレーム読み込むためのファイルポインター
	};

	FP m_playerPos;			//プレイヤーの座標
	FP m_shotTime;			//弾を撃つ時間
	FP m_enemy;				//敵

	//何レベルまで解放されているかを管理する
	INT8 m_releaseLevel;
	

	//シングルトン
private:
	C_Save() 
	{
		CheckFile();
	}
public:
	static C_Save& GetInstance()
	{
		static C_Save instance;
		return instance;
	}
};

#define SAVE C_Save::GetInstance()



