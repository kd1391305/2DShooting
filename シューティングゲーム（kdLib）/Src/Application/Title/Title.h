#pragma once
#include"../SceneBase.h"
#include"../Toolkit/Button.h"

class C_Title :public C_SceneBase
{
public:
	C_Title();
	~C_Title() override {}

	void Update();
	void Draw();

private:
	C_Button m_start;

	C_Button m_levelButton[5];

	bool m_bFirstMenu;			//最初の選択画面か次のレベル選択画面かどうかのフラグ			
};
