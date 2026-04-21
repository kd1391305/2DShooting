#pragma once
#include"../SceneBase.h"
#include"Chara/Player/Player.h"
#include"Back/Back.h"
#include"HUD/HUD.h"
#include"Chara/Player/FormerPlayers.h"
#include"GameClear.h"
#include"GameOver.h"

//ゲームの状態
enum GameState
{
	Game,				//ゲーム画面
	GameClear,		//ゲームクリア画面
	GameOver,		//ゲームオーバー画面
};

class C_Game :public C_SceneBase
{
public:

	C_Game() = delete;
	C_Game(int level);
	~C_Game()override;

	void Update();
	void Draw();

	//ゲームの状態を変化させる
	void ChangeState(GameState change) { m_state = change; }

	int GetLevel() { return m_gameLevel; }

private:

	//ゲーム中の更新
	void UpdateGame();
	//ゲームクリア更新
	void UpdateGameClear();
	//ゲームオーバー更新
	void UpdateGameOver();
	//ゲーム中の描画
	void DrawGame();
	//ゲームクリアの描画
	void DrawGameClear();
	//ゲームのオーバーの描画
	void DrawGameOver();

	C_Player m_player;					//プレイヤー
	C_FormerPlayers m_fPlayers;		//過去のプレイヤー
	C_Back m_back;						//背景
	C_HUD m_HUD;						//HeadUpDisplay(UI)
	C_GameClear m_gameClear;		//ゲームクリア
	C_GameOver m_gameOver;		//ゲームオーバー

	INT8 m_gameLevel;			//ゲームレベル（1からスタート）（levelと配列の添え字がずれないよう注意）

	GameState m_state;			//ゲームの状態
};
