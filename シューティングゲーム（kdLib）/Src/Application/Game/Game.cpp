#include"Game.h"
#include"../SceneManager.h"
#include"../Result/Result.h"
#include"Bullet/BulletManager.h"
#include"Chara/Enemy/EnemyManager.h"
#include"../Toolkit/Collision.h"
#include"GameTimer.h"
#include"FadeEffect/FadeEffectManager.h"
#include"../Save/Save.h"

C_Game::C_Game(int level):
	m_gameLevel(level)
{
	//HUDの初期化
	m_HUD.Init(&m_player,level);

	//セーブデータから登場するプレイヤーの初期化
	m_fPlayers.Init(level, m_player.GetTex());

	//セーブ＆ロードに使用するファイルを開ける
	SAVE.Open(level);
}

C_Game::~C_Game()
{
	//セーブ＆ロードに使用したファイルを閉じる
	SAVE.Close();
}

void C_Game::Update()
{
	GAME_TIMER.Update();

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		SCENE_MANAGER.ChangeState(new C_Result());
	}
	m_player.Action();


	//当たり判定
	if (!m_player.IsInvincible())		//無敵中は判定を行わない
	{
		//プレイヤー　と　敵
		Collision(&m_player, ENEMY_MANAGER.GetEnemyList());

		//プレイヤー　と　敵の弾
		Collision(&m_player, BULLET_MANAGER.GetEnemyList());
	}
	//プレイヤーの弾　と　敵
	Collision(BULLET_MANAGER.GetPlayerList(), ENEMY_MANAGER.GetEnemyList(), &m_HUD);

	m_back.Update();

	m_player.Update();

	ENEMY_MANAGER.Update();
	BULLET_MANAGER.Update();
	FADE_EFFECT.Update();

	m_fPlayers.Update();

	m_HUD.Update();

	//プレイヤーの座標をセーブする
	SAVE.Write(m_player.GetPos());
}
void C_Game::Draw()
{
	m_back.Draw();

	m_fPlayers.Draw();

	FADE_EFFECT.Draw();

	m_player.Draw();
	BULLET_MANAGER.Draw();
	ENEMY_MANAGER.Draw();
	
	m_HUD.Draw();
}

bool C_Game::IsGameOver()
{
	return false;
}

bool C_Game::IsGameClear()
{
	return false;
}
