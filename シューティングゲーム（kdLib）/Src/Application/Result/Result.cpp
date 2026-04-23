#include"Result.h"	
#include"../SceneManager.h"
#include"../Toolkit/Font/DrawString.h"
#include"../Title/Title.h"
#include"../Fireworks/FireworksManager.h"

void Result::Init()
{

}

void Result::Update()
{
	if (GetAsyncKeyState('Z') & 0x8000)
	{
		SceneManager::Instance().ChangeState(new Title());
	}
}

void Result::Draw()
{
	DWriteCustom::Instance().Draw("ƒŠƒUƒ‹ƒg‰æ–Ê", Math::Vector2{0,0});
}

void Result::Release()
{

}
