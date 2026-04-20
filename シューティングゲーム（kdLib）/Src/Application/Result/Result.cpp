#include"Result.h"	
#include"../SceneManager.h"
#include"../Toolkit/Font/DrawString.h"
#include"../Title/Title.h"

void C_Result::Update()
{
	if (GetAsyncKeyState('Z') & 0x8000)
	{
		SCENE_MANAGER.ChangeState(new C_Title());
	}
}

void C_Result::Draw()
{
	DRAW_STRING.Draw("ƒŠƒUƒ‹ƒg‰æ–Ê", Math::Vector2{ 0,0 });
}