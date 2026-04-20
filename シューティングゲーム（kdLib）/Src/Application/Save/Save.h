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

	//ファイルを閉じる
	void Close();
	
	//毎フレーム、プレイヤーの情報を書き込む
	bool WritePlayerPos(Math::Vector2 pos);

	//毎フレーム、プレイヤーの情報を引き出す
	bool ReadPlayerPos(Math::Vector2* pPos, int level);

	//敵の出現場所、出現時間を書き込む
	bool WriteEnemy(Math::Vector2 pos, long time);

	//敵のスポーン情報を探す
	bool SearchSpawnEnemy(long time,int level);

	//敵のスポーン座標を取り出す
	Math::Vector2 PopSpawnEnemy(int level);

	//プレイヤーが弾を出した時間を書き込む
	bool WritePlayerShotTime(long time);

	//プレイヤーが弾を出す時間を返す
	long GetPlayerShotTime(int level);

	//プレイヤーが弾を出す時間を取り出す（弾を出す時間を次の時間に切り替える）
	void PopPlayerShotTime(int level);

	//解放されているlevelを確認する。（作られているファイルがあると、解放されていると判断する）
	void CheckReleaseLevel();

	//何レベルまで解放されているかを返す
	int GetReleaseLevel() { return m_releaseLevel; }

private:

	struct FP
	{
		FILE* write;		//毎フレーム、セーブするためのファイルポインター
		FILE* read[4];	//毎フレーム読み込むためのファイルポインター
	};

	FP m_playerPos;					//プレイヤーの座標
	long m_nextShotTime[4];			//次弾を撃つ時間
	FP m_shotTime;					//弾を撃つ時間

	FP m_enemy;				//敵（出現時間、出現場所）
	Math::Vector2 m_nextSpawnPos[4];		//敵の座標を保存する
	long m_nextSpawnTime[4];					//敵の出現タイミング

	//何レベルまで解放されているかを管理する
	INT8 m_releaseLevel;
	

	//シングルトン
private:
	C_Save() 
	{
		CheckReleaseLevel();
	}
public:
	static C_Save& GetInstance()
	{
		static C_Save instance;
		return instance;
	}
};

#define SAVE C_Save::GetInstance()



