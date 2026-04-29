#pragma once
#include"../BaseChara/BaseChara.h"

class BulletManager;

class Boss:public BaseChara

{
public:

	Boss() {}
	~Boss()override {}

	void Init()override;
	void Update(float deltaTime)override;
	void Draw()override;

	void Spawn(Math::Vector2 pos,Math::Vector2 move);

	void Action(float deltaTime);

	void Dead();

	static void SetBulletManager(std::shared_ptr<BulletManager> set) { m_pBulletManager = set; }

private:

	void Release()override;

	//通常弾のカウンター
	int m_shotCnt;
	int m_shotCntMax;				//最大になったら倍のクールタイムを設ける

	static std::shared_ptr<BulletManager> m_pBulletManager;

	//３６０度に弾を発射する時のクールタイム
	float m_circleShotWait;
	float m_circleShotWaitTimer;
	int m_circleShotCnt;			
	int m_circleShotCntMax;			//最大になったら倍のクールタイムを設ける
};
