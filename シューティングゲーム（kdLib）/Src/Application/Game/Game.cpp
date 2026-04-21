#include"Game.h"
#include"../SceneManager.h"
#include"../Result/Result.h"
#include"Bullet/BulletManager.h"
#include"Chara/Enemy/EnemyManager.h"
#include"../Toolkit/Collision.h"
#include"GameTimer.h"
#include"FadeEffect/FadeEffectManager.h"
#include"../Save/Save.h"

//コンストラクタ
C_Game::C_Game(int level):
	m_gameLevel(level)
{
	//HUDの初期化
	m_HUD.Init(&m_player,level);

	//セーブデータから登場するプレイヤーの初期化
	m_fPlayers.Init(level, m_player.GetTex());

	//セーブ＆ロードに使用するファイルを開ける
	SAVE.Open(level);

	ENEMY_MANAGER.SetGame(this);

	m_state = GameState::Game;
}

//デストラクタ
C_Game::~C_Game()
{
	//セーブ＆ロードに使用したファイルを閉じる
	SAVE.Close();
}

//更新
void C_Game::Update()
{
	if (m_state == GameState::Game)
		UpdateGame();
	else if (m_state == GameState::GameClear)
		UpdateGameClear();
	else if (m_state == GameState::GameOver)
		UpdateGameOver();
}

//描画
void C_Game::Draw()
{
	if (m_state == GameState::Game)
		DrawGame();
	else if (m_state == GameState::GameClear)
		DrawGameClear();
	else if (m_state == GameState::GameOver)
		DrawGameOver();
}

//ゲーム中の更新
void C_Game::UpdateGame()
{
	//ゲームタイマーを更新する
	GAME_TIMER.Update();

	//プレイヤーの行動
	m_player.Action();

	//当たり判定
	//プレイヤー　と　敵
	Collision(&m_player, ENEMY_MANAGER.GetEnemyList());

	//プレイヤー　と　過去の敵
	Collision(&m_player, ENEMY_MANAGER.GetFormerEnemyList());

	//プレイヤー　と　敵の弾
	Collision(&m_player, BULLET_MANAGER.GetEnemyList());

	//プレイヤーの弾　と　敵
	Collision(BULLET_MANAGER.GetPlayerList(), ENEMY_MANAGER.GetEnemyList(), &m_HUD);

	//プレイヤーの弾　と　過去の敵
	Collision(BULLET_MANAGER.GetPlayerList(), ENEMY_MANAGER.GetFormerEnemyList(), &m_HUD);

	//背景の更新
	m_back.Update();

	//プレイヤーの更新
	m_player.Update();

	//過去のプレイヤーの更新
	m_fPlayers.Update();

	//すべての敵の更新
	ENEMY_MANAGER.Update();

	//全ての弾の更新
	BULLET_MANAGER.Update();

	//全ての消滅エフェクトの更新
	FADE_EFFECT.Update();

	//HeadUpDisplay（UI）の更新
	m_HUD.Update();
}

//ゲームクリア中の更新
void C_Game::UpdateGameClear()
{
	m_gameClear.Update();
}

//ゲームオーバー中の更新
void C_Game::UpdateGameOver()
{
	m_gameOver.Update();
}

//ゲーム中の描画
void C_Game::DrawGame()
{
	m_back.Draw();

	m_fPlayers.Draw();

	FADE_EFFECT.Draw();

	m_player.Draw();
	BULLET_MANAGER.Draw();
	ENEMY_MANAGER.Draw();

	m_HUD.Draw();
}

//ゲームクリア中の描画
void C_Game::DrawGameClear()
{
	m_gameClear.Draw();
}

//ゲームオーバー中の描画
void C_Game::DrawGameOver()
{
	m_gameOver.Draw();
}