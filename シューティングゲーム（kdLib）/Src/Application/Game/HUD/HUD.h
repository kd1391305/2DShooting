#pragma once
#include"Score.h"
#include"HUDBack.h"
#include"HUDPlayer.h"
#include"HUDTimer.h"

class Player;

//プレイヤーの体力やスコアを表示するクラス

//HeadUpDisplay
class HUD
{
public:

	//初期化
	void Init(Player* player);

	//更新
	void Update();

	//描画
	void Draw();

	//スコアを加算
	void AddScore(long addScore) { m_score.Add(addScore); }




private:
	HUDBack m_back;		//HUDの背景
	Score m_score;			//スコア
	HUDPlayer m_player;
};
