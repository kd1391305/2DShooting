#pragma once
#include"../BaseScene/BaseScene.h"

class ResultScene :public BaseScene
{
public:
	ResultScene() {}
	~ResultScene()override {}
	void Init();
	void Update();

	void Draw();

private:
	void Release();
};