#pragma once
#include"../BaseScene/BaseScene.h"

class FireworksManager;
class Back;

class GameClearScene :public BaseScene
{
public:
	GameClearScene() = delete;
	GameClearScene(std::shared_ptr<Back>back,std::shared_ptr<FireworksManager>fireworksManager):
		m_back(back),m_fireworksManager(fireworksManager)
	{}
	~GameClearScene() override { Release(); }

	void Init()override;
	void Update()override;
	void Draw()override;

private:

	void Release()override;

	std::shared_ptr<Back> m_back;
	std::shared_ptr<FireworksManager> m_fireworksManager;
};
