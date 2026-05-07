#pragma once
#include"../../BaseChara/BaseChara.h"

class BulletManager;

class BaseEnemy :public BaseChara
{
	//==============================================
	//継承先で変更を加えるもの 
	//==============================================
protected:
	//敵の行動
	//弾を発射する処理などを書く
	//敵の動きを書く
	virtual void Action(float deltaTime) {}
	//==============================================

public:

	BaseEnemy() {}
	virtual~BaseEnemy()override {}

	//初期化
	void Init();

	//更新
	void Update(float deltaTime)override;

	//描画
	virtual void Draw()override;

	//出現させるときの処理
	virtual void Spawn(
		Math::Vector2& pos,						//出現する場所
		Math::Vector2& radius,					//敵の大きさ
		float moveSpeed,						//移動スピード
		float moveDeg,							//移動方向
		Math::Color& normalColor,				//通常の色
		Math::Color& hitColor,					//当たった時の色
		float hp,								//HP
		float bulletSpeed,						//弾のスピード
		float shotCoolTime,							//クールタイム
		const float shotCoolTimeNoiseMax = 0.0f,	//クールタイムのノイズ
		const float spawnShotCoolTime = 0.0f);		//スポーン時の追加クールタイム

	void Damage(float damage)override;

	//倒れた時の処理
	virtual void Dead()override { m_bActive = false; }

	bool IsActive() { return m_bActive; }

	void SetActive(bool set) { m_bActive = set; }

	void SetFireworksNum(int set) { m_fireworksNum = set; }

	void SetDeadScreenOutFlg(bool set) { m_bDead_ScreenOut = set; }

	Math::Vector2 GetPos() { return m_pos; }

	static void SetBulletManager(BulletManager* set) { s_pBulletManager = set; }
	static void SetPlayerPos(Math::Vector2* set) { s_pPlayerPos = set; }

	Math::Color GetColor()override { return m_normalColor; }

	int GetFireworksNum() { return m_fireworksNum; }

protected:

	virtual void Release()override{}

	static	BulletManager* s_pBulletManager;	//弾を打つためにアドレスをEnemy共通で持っておく
	static Math::Vector2* s_pPlayerPos;			//プレイヤーに撃つためにプレイヤーの座標

	bool m_bActive;								//敵の活性状態

	float m_radian;								//敵画像の回転角度

	bool	m_bDead_ScreenOut;					//スクリーンアウトしたときに非活性状態にするか？

	float m_bulletSpeed = 200.0f;					//敵の弾速
	float m_shotCoolTimeNoiseMax;					//弾発射のクールタイムのランダムなずれ時間の最大値

	Math::Color m_normalColor;					//通常時の色
	Math::Color m_hitColor;						//当たった時の色

	bool m_bHitFlg;								//当たったか？
	float m_hitEffectTimer;						//ヒットエフェクト発生中のタイマー（０になったらエフェクトを切る）
	const float m_hitEffectTime = 0.1f;			//ヒットエフェクトの時間

	int m_fireworksNum = 1;						//倒したときに出現させる花火の数
};
