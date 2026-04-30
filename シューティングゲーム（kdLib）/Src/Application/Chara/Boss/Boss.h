#pragma once
#include"../BaseChara/BaseChara.h"

class BulletManager;

class Boss:public BaseChara

{
public:

	Boss() = delete;
	Boss(std::shared_ptr<BulletManager>pBulletManager, Math::Vector2 *pPlayerPos);
	~Boss()override {}


	void Init()override;

	void Update(float deltaTime)override;
	void Draw()override;


	void Spawn(Math::Vector2 pos,Math::Vector2 move);

	void Action(float deltaTime);

	void Dead();

private:

	void Release()override;

	//通常弾のカウンター
	int m_shotCnt;
	int m_shotCntMax;				//最大になったら倍のクールタイムを設ける
	float m_shotSpeed;				//弾速


	std::shared_ptr<BulletManager> m_pBulletManager;
	Math::Vector2* m_pPlayerPos;

	//３６０度に弾を発射する時のクールタイム
	float m_circleShotWait;
	float m_circleShotWaitTimer;
	int m_circleShotCnt;			
	int m_circleShotCntMax;			//最大になったら倍のクールタイムを設ける
	float m_circleShotSpeed;		//弾速
	const int m_circleShotBulletNum = 30;

};
