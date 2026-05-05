#pragma once
#include"../BaseScene/BaseScene.h"

class Game;
class Button;

class GameOverScene :public BaseScene
{
public:

	GameOverScene() = delete;
	GameOverScene(std::shared_ptr<Game>game) :
		m_game(game)
	{}

	~GameOverScene() { Release(); }

	void Init();

	void Update();

	void Draw();

private:

	void Release();

	std::shared_ptr<Game>	m_game;

	std::shared_ptr<Button> m_titleButton;						//タイトルへ行くボタン
	std::shared_ptr<Button> m_gameButton;						//ゲームへ戻るボタン
		
	Math::Color m_color;
	const float m_maxAlpha = 0.7f;
	const float m_deltaAlpha = 2;								//1 / 2 = 0.5秒で 1 上がる

	
};
