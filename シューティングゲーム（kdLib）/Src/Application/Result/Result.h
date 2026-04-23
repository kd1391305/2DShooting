#pragma once
#include"../SceneBase.h"

class Result :public SceneBase
{
public:
	Result() {}
	~Result()override {}
	void Init();
	void Update();

	void Draw();

private:
	void Release();
};