#pragma once

#include"../BaseEnemy/BaseEnemy.h"


class Enemy1 :public BaseEnemy
{
public:
	void Action(float deltaTime)override;
};


class Enemy2:public BaseEnemy
{
public:
	void Action(float deltaTime)override;
	void InitOriginal(float shotNum, float shotArcRadian);
private:
	int m_shotNum;							//弾を打ち出す数
	float m_shotArcRadian;						//弾を打ち出す弧の角度（プレイヤーを中心に45度ずつ外側まで発射する）
};