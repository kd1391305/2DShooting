#pragma once
#include"../BaseScene/BaseScene.h"

class Back;
class FireworksManager;
class Player;
class EnemyManager;
class BulletManager;
class UI;
class DropExp;

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

	std::shared_ptr<FireworksManager> m_fireworksManager;		//â‘âŒ
	std::shared_ptr<Player> m_player = nullptr;					//ÉvÉåÉCÉÑÅ[
	std::shared_ptr<EnemyManager> m_enemyManager = nullptr;		//ìG
	std::shared_ptr<BulletManager> m_bulletManager = nullptr;	//íe
	std::shared_ptr<Back> m_back = nullptr;						//îwåi
	std::shared_ptr<UI> m_UI = nullptr;							//UI

};
