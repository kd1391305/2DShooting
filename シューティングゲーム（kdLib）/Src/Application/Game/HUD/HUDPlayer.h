#pragma once
#include"../../Toolkit/Gauge.h"

class Player;			//前方宣言

//HUDで描画するプレイヤー関連のクラス
class HUDPlayer
{
public:
	HUDPlayer() {}
	
	void Init(Player*player);
	void Update();
	void Draw();

private:

	Gauge m_gauge;
};
