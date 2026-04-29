#pragma once
#include"../BaseScene/BaseScene.h"

class Back;
class FireworksManager;
class Player;
class EnemyManager;
class BulletManager;
class UI;
class DropExp;
class Button;

class GameOverScene :public BaseScene
{
public:

	GameOverScene() = delete;
	GameOverScene(std::shared_ptr<Player> player,
		std::shared_ptr<EnemyManager> enemy,
		std::shared_ptr<FireworksManager>fireworks,
		std::shared_ptr<BulletManager>bullet,
		std::shared_ptr < Back>back);

	~GameOverScene() { Release(); }

	void Init();

	void Update();

	void Draw();

private:

	void Release();

	std::shared_ptr<FireworksManager> m_fireworksManager;		//花火
	std::shared_ptr<Player> m_player = nullptr;					//プレイヤー
	std::shared_ptr<EnemyManager> m_enemyManager = nullptr;		//敵
	std::shared_ptr<BulletManager> m_bulletManager = nullptr;	//弾
	std::shared_ptr<Back> m_back = nullptr;						//背景
	std::shared_ptr<UI> m_UI = nullptr;							//UI

	std::shared_ptr<Button> m_titleButton;		//タイトルへ行くボタン
	std::shared_ptr<Button> m_gameButton;		//ゲームへ戻るボタン
};
