#pragma once

#include"BaseEnemy.h"

class Enemy1 :public BaseEnemy
{
public:

	Enemy1() {}
	~Enemy1()override{}

	//初期化
	void Init()override;

	//倒れた時の処理
	void Dead()override;

	//出現させるときの処理
	void Spawn()override;
private:
	//敵の行動パターン通りに動く
	void ActionPutturn()override;
};
