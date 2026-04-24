#include"GameClear.h"
#include"../../Font/DrawString.h"

//XV
void GameClear::Update()
{

}

//•`‰æ
void GameClear::Draw()
{
	DWriteCustom::Instance().Draw("Game Clear", {-50,30});
}