#pragma once
#include"../BaseScene/BaseScene.h"

class Back;
class FireworksManager;
class Player;
class EnemyManager;
class BulletManager;
class UI;
class HitEffectManager;
class AttackAreaManager;

class Game :public BaseScene
{
public:

	Game() = delete;
	Game(std::shared_ptr<Back> back);
	~Game()override { Release(); }

	void Init();
	void Update();
	void Draw();

	void GameOver();

	void GameClear();

	std::shared_ptr<Player> GetPlayer() { return m_player; }
	std::shared_ptr<EnemyManager>GetEnemyManager() { return m_enemyManager; }
	std::shared_ptr<BulletManager> GetBulletManager() { return m_bulletManager; }
	std::shared_ptr<Back>GetBack() { return m_back; }
	std::shared_ptr<UI>GetUI() { return m_UI; }
	std::shared_ptr<AttackAreaManager>GetAttackAreaManager() { return m_attackAreaManager; }

	void SetContinueFlg(bool set) { m_bContinue = set; }
	
	bool GetGameClearFlg() { return m_bGameClearFlg; }

private:

	void Release();

	std::shared_ptr<Player> m_player = nullptr;					//プレイヤー
	std::shared_ptr<EnemyManager> m_enemyManager = nullptr;		//敵
	std::shared_ptr<BulletManager> m_bulletManager = nullptr;		//弾
	std::shared_ptr<Back> m_back = nullptr;						//背景
	std::shared_ptr<UI> m_UI = nullptr;							//UI
	std::shared_ptr<HitEffectManager> m_hitEffectManager;		//ヒットエフェクト
	std::shared_ptr<AttackAreaManager> m_attackAreaManager;		//攻撃範囲

	//スタート時のフラグ
	//スタート時にはプレイヤーが画面外から出現する。その時の処理用変数
	bool m_bStartFlg;
	const float  m_appearSpeed = 500.0f;
	const float m_StartPosX = -300;

	bool m_bContinue = false;

	bool m_bGameClearFlg = false;
	float m_changeSceneWait;
};

