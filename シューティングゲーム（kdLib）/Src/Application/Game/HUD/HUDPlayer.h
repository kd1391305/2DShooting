#pragma once
#include"../../Toolkit/Gauge.h"

class C_Player;			//前方宣言

//HUDで描画するプレイヤー関連のクラス
class C_HUDPlayer
{
public:
	C_HUDPlayer() {}
	
	void Init(C_Player*player);
	void Update();
	void Draw();

private:

	C_Gauge m_gauge;
};
