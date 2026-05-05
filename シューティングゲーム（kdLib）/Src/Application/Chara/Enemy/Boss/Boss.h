#pragma once
#include"../../BaseChara/BaseChara.h"

class Game;

class Boss:public BaseChara

{
public:

	Boss() = delete;
	Boss(Game*game);
	~Boss()override {}


	void Init()override;

	void Update(float deltaTime)override;
	void Draw()override;


	void Spawn(Math::Vector2 pos,Math::Vector2 move);

	void Action(float deltaTime);

	void Dead();

	void OnHit()override;

private:

	Game* m_pGame;

	void Release()override;

	//通常弾のカウンター
	int m_shotCnt;
	int m_shotCntMax;				//最大になったら倍のクールタイムを設ける
	float m_bulletSpeed;				//弾速


	//３６０度に弾を発射する時のクールタイム
	float m_circleshotCoolTime;
	float m_circleshotCoolTimer;
	int m_circleShotCnt;			
	int m_circleShotCntMax;			//最大になったら倍のクールタイムを設ける
	float m_circlebulletSpeed;		//弾速
	const int m_circleShotBulletNum = 30;

	float m_endPosX = 200;		//移動するときX座標２００以下にはいかない（）

};
