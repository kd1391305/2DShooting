#pragma once
#include"../BaseEnemy/BaseEnemy.h"

class Enemy3 :public BaseEnemy
{
public:

	//初期化
	virtual void Init();

	//倒れた時の処理
	virtual void Dead()override;

	//出現させるときの処理
	virtual void Spawn();

	void OnHit()override;

	Math::Color GetColor()override { return m_normalColor; }
private:
	//敵の行動
	virtual void Action(float deltaTime);

	Math::Color m_hitColor;
	Math::Color m_normalColor;

	bool m_bHitFlg;
	float m_hitEffectTimer;					//エフェクト発生中のタイマー（０になったらエフェクトを切る）
	const float m_hitEffectTime = 0.1f;		//エフェクトの時間
};