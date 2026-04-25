#include"GameScene.h"
#include"../ResultScene/ResultScene.h"
#include"../../Bullet/BulletManager.h"
#include"../../Chara/Enemy/EnemyManager.h"
#include"../../Tools/Collision/Collision.h"
#include"../../Timer/Timer.h"
#include"../../Fireworks/FireworksManager.h"
#include"../../Background/Back.h"
#include"../../UI/UI.h"
#include"../../Chara/Player/Player.h"
#include"GameClear.h"
#include"GameOver.h"
#include"../../Chara/Enemy/BaseEnemy.h"

//コンストラクタ
Game::Game(std::shared_ptr<Back> back, std::shared_ptr<FireworksManager> fireworksManager)
{
	m_back = back;
	m_fireworksManager = fireworksManager;

	m_player = std::make_shared<Player>();
	m_enemyManager = std::make_shared<EnemyManager>();
	m_bulletManager = std::make_shared<BulletManager>();
	m_UI = std::make_shared<UI>();

	BaseEnemy::SetBulletManager(m_bulletManager.get());
}

void Game::Init()
{
	m_player->Init(m_fireworksManager.get());

	//UIの初期化
	m_UI->Init(m_player.get());

	//タイマークラスをリセット
	Timer::Instance().Reset();
}

//更新
void Game::Update()
{
	const double deltaTime = Timer::Instance().GetDeltaTime();

	//プレイヤーの行動
	m_player->Action(deltaTime);

	//当たり判定
	//プレイヤー　と　敵
	CollisionPlayer_Enemy(m_player, m_enemyManager->GetEnemyList());

	//プレイヤー　と　敵の弾
	CollisionPlayer_EBullet(m_player, m_bulletManager->GetEnemyList());

	//プレイヤーの弾　と　敵
	CollisionFireworks_EBullet(m_fireworksManager->GetList(), m_enemyManager->GetEnemyList(), m_UI->GetScoreInst());

	//背景の更新
	m_back->Update();

	//プレイヤーの更新
	m_player->Update(deltaTime);

	//すべての敵の更新
	m_enemyManager->Update(deltaTime);

	//全ての弾の更新
	m_bulletManager->Update(deltaTime);

	//HeadUpDisplay（UI）の更新
	m_UI->Update();

	m_fireworksManager->Update(deltaTime);
}

//描画
void Game::Draw()
{
	m_back->Draw();

	m_player->Draw();
	m_bulletManager->Draw();
	m_enemyManager->Draw();

	m_fireworksManager->Draw();

	m_UI->Draw();
}

void Game::Release()
{
}

