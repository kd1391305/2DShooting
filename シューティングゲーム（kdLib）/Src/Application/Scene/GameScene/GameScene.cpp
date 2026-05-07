#include"GameScene.h"
#include"../../Bullet/BulletManager.h"
#include"../../Chara/Enemy/EnemyManager.h"
#include"../../Tools/Collision/Collision.h"
#include"../../Timer/Timer.h"
#include"../../Fireworks/FireworksManager.h"
#include"../../Background/Back.h"
#include"../../UI/UI.h"
#include"../../Chara/Player/Player.h"
#include"../../Chara/Enemy/BaseEnemy/BaseEnemy.h"
#include"../SceneManager.h"
#include"../GameOverScene/GameOverScene.h"
#include"../GameClearScene/GameClearScene.h"
#include"../../Font/DWriteCustom.h"
#include"../../main.h"
#include"../../UI/Score/Score.h"
#include"../../SoundCache/SoundCache.h"
#include"../../Animtion/HitEffect/HitEffectManager.h"
#include"../../Mouse/Mouse.h"

//コンストラクタ
Game::Game(std::shared_ptr<Back> back)
{
	m_back = back;

	m_player = std::make_shared<Player>();
	m_enemyManager = std::make_shared<EnemyManager>();
	m_bulletManager = std::make_shared<BulletManager>();
	m_UI = std::make_shared<UI>();
	m_hitEffectManager = std::make_shared<HitEffectManager>();

	BaseEnemy::SetBulletManager(m_bulletManager.get());
	BaseEnemy::SetPlayerPos(m_player->GetPosAddress());
	
	m_player->SetGame(this);
	m_player->SetBulletManager(m_bulletManager);
	m_player->Init();

	m_enemyManager->SetGame(this);

	//プレイヤーの初期化
	m_player->Init();
}

//初期化
void Game::Init()
{
	//タイマークラスをリセット
	Timer::Instance().Reset();

	m_back->StartZoomIn();

	std::shared_ptr<KdSoundInstance> bgm = SoundCache::Instance().Get("Sound/BGM/hanamatsuri.wav");
	bgm->SetVolume(0.002f);
	bgm->Play(true);

	//スタート直後のフラグを立てる
	m_bStartFlg = true;

	MOUSE.ShowCursorTex(false);
}

//更新
void Game::Update()
{
	const double deltaTime = Timer::Instance().GetDeltaTime();

	if (m_bStartFlg)
	{
		m_player->SetPos({ m_player->GetPos().x + m_appearSpeed * (float)deltaTime, m_player->GetPos().y });
		m_player->CreateMatrix();
		if (m_player->GetPos().x >= m_StartPosX)
		{
			m_bStartFlg = false;

			//UIの初期化
			m_UI->Init(m_player.get());
		}
		else
		{
			//背景だけ更新処理
			m_back->Update(deltaTime);
			return;
		}
	}

	//プレイヤーの行動
	m_player->Action(deltaTime);

	//当たり判定
	//プレイヤー　と　敵
	CollisionPlayer_Enemy(m_player, m_enemyManager->GetEnemyList());

	//プレイヤー　と　敵の弾
	CollisionPlayer_EBullet(m_player, m_bulletManager->GetEnemyList());

	//プレイヤーの弾　と　敵
	CollisionPlayerBullet_Enemy(m_bulletManager->GetPlayerList(), m_enemyManager->GetEnemyList(), m_back->GetFireworks(),m_hitEffectManager ,m_UI->GetScoreInst());
	{
		std::shared_ptr<Boss> boss = m_enemyManager->GetBoss();
		if (boss)
		{
			//プレイヤーの弾　と　ボス
			CollisionPlayer_Boss(m_player, boss);

			//プレイヤー　と　ボス
			CollisionPlayerBullet_Boss(m_bulletManager->GetPlayerList(), boss, m_back->GetFireworks(),m_hitEffectManager);
		}
	}
	//背景の更新
	m_back->Update(deltaTime);

	//プレイヤーの更新
	m_player->Update(deltaTime);

	//すべての敵の更新
	m_enemyManager->Update(deltaTime);

	//全ての弾の更新
	m_bulletManager->Update(deltaTime);

	//全てのヒットエフェクトの更新
	m_hitEffectManager->Update(deltaTime);

	//HeadUpDisplay（UI）の更新
	m_UI->Update(deltaTime);
}

//描画
void Game::Draw()
{
	m_back->Draw();

	m_player->Draw();

	m_enemyManager->Draw();

	m_hitEffectManager->Draw();

	m_bulletManager->Draw();

	if (!m_bStartFlg)
	{
		m_UI->Draw();
	}
}
//ゲームオーバーにする
void Game::GameOver()
{
	//ゲームオーバーシーンへ
	SceneManager::Instance().ChangeState(std::make_shared<GameOverScene>(
		std::dynamic_pointer_cast<Game> (SceneManager::Instance().GetCurrentState())));
	std::shared_ptr<KdSoundInstance> bgm = SoundCache::Instance().Get("Sound/BGM/hanamatsuri.wav");
	bgm->Stop();
	MOUSE.ShowCursorTex(true);
}

void Game::GameClear()
{
	SceneManager::Instance().ChangeState(std::make_shared<GameClearScene>(m_back));

	std::shared_ptr<KdSoundInstance> bgm = SoundCache::Instance().Get("Sound/BGM/hanamatsuri.wav");
	bgm->Stop();

	MOUSE.ShowCursorTex(true);
}

//解放
void Game::Release()
{
}

