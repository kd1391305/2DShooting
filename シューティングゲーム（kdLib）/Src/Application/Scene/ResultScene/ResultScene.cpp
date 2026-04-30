#include"ResultScene.h"	
#include"../SceneManager.h"
#include"../../Font/DWriteCustom.h"
#include"../TitleScene/TitleScene.h"
#include"../../Fireworks/FireworksManager.h"

void ResultScene::Init()
{

}

void ResultScene::Update()
{
	if (GetAsyncKeyState('Z') & 0x8000)
	{
		SceneManager::Instance().ChangeState(new TitleScene());
	}
}

void ResultScene::Draw()
{
	DWriteCustom::Instance().Draw("ƒŠƒUƒ‹ƒg‰æ–Ê", Math::Vector2{0,0});
}

void ResultScene::Release()
{

}
