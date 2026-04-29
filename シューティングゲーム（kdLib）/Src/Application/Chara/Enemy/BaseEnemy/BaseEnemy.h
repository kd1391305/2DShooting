#pragma once
#include"../../BaseChara/BaseChara.h"

class BulletManager;

class BaseEnemy :public BaseChara
{
	//==============================================
	//継承先で変更を加えるもの 
	//==============================================
public:
	//初期化
	virtual void Init();

	//描画
	virtual void Draw()override;

	//倒れた時の処理
	virtual void Dead()override { m_bActive = false; }

	//出現させるときの処理
	virtual void Spawn();
protected:
	//敵の行動
	virtual void Action(float deltaTime);
	//==============================================

public:

	BaseEnemy(){}
	virtual~BaseEnemy()override {}

	//更新
	void Update(float deltaTime)override;

	bool IsActive() { return m_bActive; }

	void SetActive(bool set) { m_bActive = set; }

	Math::Vector2 GetPos() { return m_pos; }

	static void SetBulletManager(BulletManager* set) { s_pBulletManager = set; }
	static void SetPlayerPos(Math::Vector2* set) { s_pPlayerPos = set; }

protected:

	virtual void Release()override{}

	static	BulletManager* s_pBulletManager;		//弾を打つためにアドレスをEnemy共通で持っておく
	static Math::Vector2* s_pPlayerPos;				//プレイヤーに撃つためにプレイヤーの座標

	bool	m_bActive = false;						//敵の活性状態

	float m_timer;									//敵が作られてから何秒経ったか？

	bool	m_bDead_ScreenOut;						//スクリーンアウトしたときに非活性状態にするか？

	const float m_shotSpeed = 200.0f;				//敵の弾速
};
