#pragma once
#include"../SceneBase.h"

class Back;
class FireworksManager;
class Player;
class EnemyManager;
class BulletManager;
class HUD;
class GameClear;
class GameOver;

//ゲームの状態
enum GameState
{
	GameScene,			//ゲーム画面
	GameClearScene,		//ゲームクリア画面
	GameOverScene,		//ゲームオーバー画面
};

class Game :public SceneBase
{
public:

	Game() = delete;
	Game::Game(Back* back, FireworksManager* fireworksManager);
	~Game()override { Release(); }

	void Init();
	void Update();
	void Draw();

	//ゲームの状態を変化させる
	void ChangeState(GameState change) { m_state = change; }

	FireworksManager* GetFireworksManager() { return m_fireworksManager; }

private:

	void Release();

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

	FireworksManager* m_fireworksManager;

	Player *m_player=nullptr;					//プレイヤー
	EnemyManager* m_enemyManager;				//敵
	BulletManager* m_bulletManager;				//弾
	Back *m_back=nullptr;						//背景
	HUD *m_HUD=nullptr;						//HeadUpDisplay(UI)
	GameClear *m_gameClear=nullptr;		//ゲームクリア
	GameOver *m_gameOver=nullptr;		//ゲームオーバー

	GameState m_state;			//ゲームの状態
};
