#pragma once
#include"Score.h"
#include"HUDBack.h"
#include"HUDPlayer.h"
#include"HUDTimer.h"

class C_Player;

//プレイヤーの体力やスコアを表示するクラス

//HeadUpDisplay
class C_HUD
{
public:

	//初期化
	void Init(C_Player* player , int level);

	//更新
	void Update();

	//描画
	void Draw();

	//スコアを加算
	void AddScore(long addScore) { m_score.Add(addScore); }




private:
	C_HUDBack m_back;		//HUDの背景
	C_Score m_score;			//スコア
	C_HUDPlayer m_player;
};
