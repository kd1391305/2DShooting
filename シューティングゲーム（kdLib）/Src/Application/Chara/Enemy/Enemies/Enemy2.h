#pragma once
#include"../BaseEnemy/BaseEnemy.h"

class Enemy2 :public BaseEnemy
{
public:

	Enemy2() {}
	~Enemy2()override {}

	//初期化
	void Init()override;

	//倒れた時の処理
	void Dead()override;

	//出現させるときの処理
	void Spawn()override;

	//スポーンさせるに当たり初期化（セットする）
	void InitSpawn(Math::Vector2 startPos, Math::Vector2 endPos, Math::Vector2 centerMoveSpeed, float radian, float deltaRadian, float gapCenter);
	
private:
	//敵の行動パターン通りに動く
	void Action(float deltaTime)override;

	float m_radian;			//角度
	float m_deltaRadian;	//角度の変化量（毎秒）

	Math::Vector2 m_centerPos;			//回転するときの中心座標
	Math::Vector2 m_centerMoveSpeed;	//中心座標の移動スピード
	Math::Vector2 m_endPos;				//移動を終わる地点
	float m_gapCenter;			//回転する中心との距離
};
