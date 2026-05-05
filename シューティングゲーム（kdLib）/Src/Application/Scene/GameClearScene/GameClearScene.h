#pragma once
#include"../BaseScene/BaseScene.h"

class FireworksManager;
class Back;
class ButtonEx;

class GameClearScene :public BaseScene
{
public:
	GameClearScene() = delete;
	GameClearScene(std::shared_ptr<Back>back):
		m_back(back)
	{}
	~GameClearScene() override { Release(); }

	void Init()override;
	void Update()override;
	void Draw()override;

private:

	void Release()override;

	std::shared_ptr<Back> m_back;

	std::shared_ptr<ButtonEx> m_titleButton;		//タイトルへ行くボタン
	std::shared_ptr<ButtonEx> m_gameButton;		//ゲームへ戻るボタン
};
