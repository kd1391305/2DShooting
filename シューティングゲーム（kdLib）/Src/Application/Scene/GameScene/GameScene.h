#pragma once
#include"../BaseScene/BaseScene.h"

class Back;
class FireworksManager;
class Player;
class EnemyManager;
class BulletManager;
class UI;

class Game :public BaseScene
{
public:

	Game() = delete;
	Game::Game(std::shared_ptr<Back> back, std::shared_ptr<FireworksManager> fireworksManager);
	~Game()override { Release(); }

	void Init();
	void Update();
	void Draw();

	std::shared_ptr<FireworksManager> GetFireworksManager() { return m_fireworksManager; }

	void GameOver();

private:

	void Release();

	std::shared_ptr<FireworksManager> m_fireworksManager;
	std::shared_ptr<Player> m_player = nullptr;					//ƒvƒŒƒCƒ„[
	std::shared_ptr<EnemyManager> m_enemyManager = nullptr;		//“G
	std::shared_ptr<BulletManager> m_bulletManager=nullptr;		//’e
	std::shared_ptr<Back> m_back = nullptr;						//”wŒi
	std::shared_ptr<UI> m_UI = nullptr;							//UI

};
