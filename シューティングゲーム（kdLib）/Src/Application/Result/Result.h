#pragma once
#include"../SceneBase.h"

class C_Result :public C_SceneBase
{
public:
	C_Result() {}
	~C_Result()override {}
	void Update();

	void Draw();
};