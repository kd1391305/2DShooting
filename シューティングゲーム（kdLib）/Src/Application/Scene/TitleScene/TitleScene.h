#pragma once
#include"../BaseScene/BaseScene.h"

class Back;
class FireworksManager;
class Button;

class TitleScene :public BaseScene
{
public:
	TitleScene() {}
	~TitleScene() override { Release(); }

	void Init();
	void Update();
	void Draw();

private:

	void Release();

	std::shared_ptr<Button> m_start = nullptr;

	std::shared_ptr<Back> m_back = nullptr;

	std:: shared_ptr<FireworksManager> m_fireworksManager = nullptr;


	//ƒ^ƒCƒgƒ‹–¼‚Ì‰æ‘œ
	KdTexture m_nameTex;
	Math::Vector2 m_namePos;
	Math::Matrix m_nameMat;
};
