#pragma once
#include"../BaseScene/BaseScene.h"

class Back;
class FireworksManager;
class ButtonEx;

class TitleScene :public BaseScene
{
public:
	TitleScene() {}
	TitleScene(std::shared_ptr<Back> back);
	~TitleScene() override { Release(); }

	void Init();
	void Update();
	void Draw();

private:

	void Release();

	std::shared_ptr<ButtonEx> m_start = nullptr;

	std::shared_ptr<Back> m_back = nullptr;

	std:: shared_ptr<FireworksManager> m_fireworksManager = nullptr;


	//タイトル名の画像
	Math::Vector2 m_namePos;
	Math::Matrix m_nameMat;

	//花火を撃つクールタイム
	float m_shotWait;
	float m_shotWaitTimer;
};
