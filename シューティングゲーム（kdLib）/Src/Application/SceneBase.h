#pragma once

class C_SceneBase
{
public:
	C_SceneBase() {}
	virtual ~C_SceneBase() {}
	virtual void Update() =0;
	virtual void Draw() = 0;
};
