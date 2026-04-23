#pragma once
#include"../SceneBase.h"

class Back;
class FireworksManager;

class Button;

class Title :public SceneBase
{
public:
	Title() {}
	~Title() override { Release(); }

	void Init();
	void Update();
	void Draw();

private:

	void Release();

	Button* m_start = nullptr;

	Back* m_back = nullptr;

	FireworksManager* m_fireworksManager = nullptr;

};
