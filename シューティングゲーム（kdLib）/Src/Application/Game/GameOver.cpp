#include"GameOver.h"
#include"../Toolkit/Font/DrawString.h"

//XV
void GameOver::Update()
{

}

//•`‰æ
void GameOver::Draw()
{
	DWriteCustom::Instance().Draw("Game Over", {-50,30});
}