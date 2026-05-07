#pragma once
#include"../BaseEnemy/BaseEnemy.h"

class Game;

class Boss:public BaseEnemy

{
public:

	Boss() = delete;
	Boss(Game*game);
	~Boss()override {}

	void InitOriginal();

	//出現させるときの処理
	void Spawn(
		Math::Vector2& pos,								//出現する場所
		Math::Vector2& radius,							//敵の大きさ
		float moveSpeed,								//移動スピード
		float moveDeg,									//移動方向
		Math::Color& normalColor,						//通常の色
		Math::Color& hitColor,							//当たった時の色
		float hp,										//HP
		float bulletSpeed,								//弾のスピード
		float shotCoolTime,								//クールタイム
		const float shotCoolTimeNoiseMax = 0.3f,		//クールタイムのノイズ
		const float spawnShotCoolTime = 0.0f)override;	//スポーン時の追加クールタイム

	void Draw()override;

	void Action(float deltaTime)override;

	void Dead() override;

private:

	Game* m_pGame;

	int m_shotCnt;

	//３６０度に弾を発射する時のクールタイム
	float m_circleShotCoolTimer;
	int m_circleShotCnt;			

	float m_endPosX = 300;		//移動するときX座標２００以下にはいかない（）

	void Action1();
	void Action2();
	void Action3();
	void Action4();

	enum MovePutturn
	{
		Appear,
		Up,
		Down,
	};

	MovePutturn m_movePutturn = MovePutturn::Appear;		//出現状態からスタート

};
