#pragma once
#include"../BaseScene/BaseScene.h"

class FireworksManager;
class Back;
class ButtonEx;

class GameClearScene :public BaseScene
{
public:
	GameClearScene() = delete;
	GameClearScene(std::shared_ptr<Back>back,long score,long highScore,long explodeNum):
		m_back(back),m_score(score),m_highScore(highScore),m_explodeNum(explodeNum)
	{}
	~GameClearScene() override { Release(); }

	void Init()override;
	void Update()override;
	void Draw()override;

private:

	void DrawScore(int score, Math::Vector2 pos);

	void Release()override;

	std::shared_ptr<Back> m_back;

	std::shared_ptr<ButtonEx> m_titleButton;		//タイトルへ行くボタン
	std::shared_ptr<ButtonEx> m_gameButton;			//ゲームへ戻るボタン

	float m_shotCoolTimer;

	long m_score;
	long m_highScore;
	long m_explodeNum;
};
