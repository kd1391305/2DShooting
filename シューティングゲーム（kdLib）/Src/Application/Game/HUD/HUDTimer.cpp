#include"HUDTimer.h"
#include"../../Toolkit/Font/DrawString.h"
#include"../GameTimer.h"
#include"../../main.h"

void HUDTimer::Draw()
{
	//残り時間を計算
	float s = GAME_TIMER.GetGameEndTime() - (float)GAME_TIMER.ElapsedTime() / 60.0f;
	int ms = (s - int(s)) * 100;			//少数点以下の下二桁を抽出する

	//文字列に変換
	char time[20];															
	sprintf_s(time, sizeof(time), "%.2d : %.2d", (int)s, ms);

	
	//フォントを変更する
	DRAW_STRING.ChangeFont(FontName::WDXLLubrifontJPN_Regular);
	//描画
	DRAW_STRING.Draw(time, { 0,300 }, 30);
	//フォントを元に戻す
	DRAW_STRING.ChangeFont(FontName::craftmincho);
}