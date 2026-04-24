#pragma once
#include"../BaseChara/BaseChara.h"

class BulletManager;

class BaseEnemy :public BaseChara
{
	//==============================================
	//継承先で変更を加えるもの 
	//==============================================
public:
	//初期化
	virtual void Init();

	//倒れた時の処理
	virtual void Dead()override { m_bActive = false; }

	//出現させるときの処理
	virtual void Spawn();
protected:
	//敵の行動パターン通りに動く
	virtual void ActionPutturn();
	//==============================================

public:

	BaseEnemy() {}
	virtual~BaseEnemy() {}

	//更新
	void Update(float deltaTime);

	//描画
	virtual void Draw(KdTexture* tex);

	bool IsActive() { return m_bActive; }

	void SetActive(bool set) { m_bActive = set; }

	Math::Vector2 GetPos() { return m_pos; }

	static void SetBulletManager(BulletManager* set) { s_pBulletManager = set; }

	unsigned int GetID() { return m_id; }

	void SetID(unsigned int set) { m_id = set; }
protected:

	static	BulletManager* s_pBulletManager;		//弾を打つためにアドレスをEnemy共通で持っておく
	float	m_shotWaitTimer;						//撃つまでのクールタイムを測る
	float   m_shotWait;								//クールタイム
	bool	m_bActive = false;						//敵の活性状態
	float	m_timer;								//出現している時間を測る

private:
	float m_id;
};
