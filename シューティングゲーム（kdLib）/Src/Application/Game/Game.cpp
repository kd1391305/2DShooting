#include"Game.h"
#include"../Result/Result.h"
#include"Bullet/BulletManager.h"
#include"Chara/Enemy/EnemyManager.h"
#include"../Toolkit/Collision.h"
#include"../Timer/Timer.h"
#include"../Fireworks/FireworksManager.h"
#include"../Background/Back.h"
#include"HUD/HUD.h"
#include"Chara/Player/Player.h"
#include"GameClear.h"
#include"GameOver.h"

//コンストラクタ
Game::Game(Back* back, FireworksManager* fireworksManager)
{
	m_back = back;
	m_fireworksManager = fireworksManager;

	m_player = new Player();
	m_enemyManager = new EnemyManager();
	m_bulletManager = new BulletManager;
	m_HUD = new HUD();
	m_gameClear = nullptr;
	m_gameOver = nullptr;

	Enemy::SetBulletManager(m_bulletManager);
}

void Game::Init()
{
	m_player->Init(m_fireworksManager);

	//HUDの初期化
	m_HUD->Init(m_player);

	//ゲーム画面からスタート
	m_state = GameState::GameScene;

	//タイマークラスをリセット
	Timer::Instance().Reset();
}

//更新
void Game::Update()
{
	if (m_state == GameState::GameScene)
		UpdateGame();
	else if (m_state == GameState::GameClearScene)
		UpdateGameClear();
	else if (m_state == GameState::GameOverScene)
		UpdateGameOver();
}

//描画
void Game::Draw()
{
	if (m_state == GameState::GameScene)
		DrawGame();
	else if (m_state == GameState::GameClearScene)
		DrawGameClear();
	else if (m_state == GameState::GameOverScene)
		DrawGameOver();
}

void Game::Release()
{
	if (m_back)
	{
		delete m_back;
		m_back = nullptr;
	}
}

//ゲーム中の更新
void Game::UpdateGame()
{
	const double deltaTime = Timer::Instance().GetDeltaTime();

	//プレイヤーの行動
	m_player->Action(deltaTime);

	//当たり判定
	//プレイヤー　と　敵
	Collision(m_player, m_enemyManager->GetEnemyList());

	//プレイヤー　と　敵の弾
	Collision(m_player, m_bulletManager->GetEnemyList());

	//プレイヤーの弾　と　敵
	Collision(m_fireworksManager->GetFireworksList(), m_enemyManager->GetEnemyList(), m_HUD);

	//背景の更新
	m_back->Update();

	//プレイヤーの更新
	m_player->Update(deltaTime);

	//すべての敵の更新
	m_enemyManager->Update(deltaTime);

	//全ての弾の更新
	m_bulletManager->Update(deltaTime);

	//HeadUpDisplay（UI）の更新
	m_HUD->Update();

	m_fireworksManager->Update(deltaTime);
}

//ゲームクリア中の更新
void Game::UpdateGameClear()
{
	m_gameClear->Update();
}

//ゲームオーバー中の更新
void Game::UpdateGameOver()
{
	m_gameOver->Update();
}

//ゲーム中の描画
void Game::DrawGame()
{
	m_back->Draw();

	m_player->Draw();
	m_bulletManager->Draw();
	m_enemyManager->Draw();

	m_fireworksManager->Draw();

	m_HUD->Draw();
}

//ゲームクリア中の描画
void Game::DrawGameClear()
{
	m_gameClear->Draw();
}

//ゲームオーバー中の描画
void Game::DrawGameOver()
{
	m_gameOver->Draw();
}
