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
#include"../../Chara/Enemy/BaseEnemy/BaseEnemy.h"
#include"../SceneManager.h"
#include"../GameOverScene/GameOverScene.h"
#include"../../Font/DrawString.h"
#include"../../Mouse/Mouse.h"
#include"../../Chara/Boss/Boss.h"

//コンストラクタ
Game::Game(std::shared_ptr<Back> back, std::shared_ptr<FireworksManager> fireworksManager)
{
	m_back = back;
	m_fireworksManager = fireworksManager;

	m_player = std::make_shared<Player>();
	m_enemyManager = std::make_shared<EnemyManager>();
	m_bulletManager = std::make_shared<BulletManager>();
	m_UI = std::make_shared<UI>();
	m_boss = std::make_shared<Boss>();

	BaseEnemy::SetBulletManager(m_bulletManager.get());
	BaseEnemy::SetPlayerPos(m_player->GetPosAddress());
	Mouse::Instance().ShowCursorTex(false);
}

//初期化
void Game::Init()
{
	//プレイヤーの初期化
	m_player->SetGameInst(this);
	m_player->SetFireworksManagerInst(m_fireworksManager.get());
	m_player->Init();

	//UIの初期化
	m_UI->Init(m_player.get());

	//ボスの初期化
	Boss::SetBulletManager(m_bulletManager);
	m_boss->Init();
	m_boss->Spawn({ SCREEN_RIGHT,0 }, { -10,0 });

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
	CollisionFireworks_Enemy(m_fireworksManager->GetList(), m_enemyManager->GetEnemyList(), m_UI->GetScoreInst());

	//プレイヤーのチャージ弾
	CollisionChargeBullet_Enemy(m_fireworksManager->GetChargeBullet(), m_enemyManager->GetEnemyList(), m_UI->GetScoreInst());

	//背景の更新
	m_back->Update(deltaTime);

	//プレイヤーの更新
	m_player->Update(deltaTime);

	//すべての敵の更新
	m_enemyManager->Update(deltaTime);

	if(m_boss)m_boss->Update(deltaTime);

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

	if (m_boss)m_boss->Draw();

	m_fireworksManager->Draw();

	m_UI->Draw();
}

//ゲームオーバーにする
void Game::GameOver()
{
	//ゲームオーバーシーンへ
	SceneManager::Instance().ChangeState(new GameOverScene(
		m_player,
		m_enemyManager,
		m_fireworksManager,
		m_bulletManager,
		m_back));
}

//解放
void Game::Release()
{
	MOUSE.ShowCursorTex(true);
}

