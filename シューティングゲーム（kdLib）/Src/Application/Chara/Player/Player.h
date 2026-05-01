#pragma once
#include"../BaseChara/BaseChara.h"

class Game;
class ChargeAnim;
class PlayerBullet;
class BulletManager;

class Player :public BaseChara
{
public:

	Player() {}

	void Init()override;

	//更新
	void Update(float deltaTime)override;

	//描画
	void Draw()override;

	void Dead();

	//行動
	void Action(float deltaTime);

	//プレイヤーを無敵にする(引数が何秒間無敵にするか)
	void Invincible(float time)
	{
		m_bInvincible = true;
		m_invincibleTime = time;
		m_color = { 1,1,1,0.2f };
	}

	//無敵かどうか
	bool IsInvincible() { return m_bInvincible; }

	float GetHP() { return m_hp; }

	float* GetHPAddress() { return &m_hp; }
	float* GetHPMaxAddress() { return &m_hpMax; }
	Math::Vector2* GetPosAddress() { return &m_pos; }

	void SetGame(Game* set) { m_pGame = set; }
	void SetBulletManager(std::shared_ptr<BulletManager>set) { m_pBulletManager = set; }
private:

	//倒れた時のアニメーション用の値を初期化
	void InitDeadAnim();

	//倒れた時のアニメーションを描画
	void DrawDeadAnim();
	
	//解放
	void Release()override {}

	Game* m_pGame;

	std::shared_ptr<BulletManager> m_pBulletManager;

	bool m_bInvincible;					//無敵かどうかのフラグ
	float m_invincibleTime;				//無敵時間

	float m_sumDeltaTime;				//経過した時間を足していく(無敵状態のときの透明度を切り替える処理で使用)

	const float m_bulletSpeed = 400;				//弾速

	std::shared_ptr<PlayerBullet> m_bullet;			//弾
	float m_chargeTime;								//チャージ時間
	const float m_chargeTimeMax = 0.8f;				//チャージ最大時間
	const float m_chargeSpeedMax = 800.0f;			//チャージ最大スピード
	const float m_chargePowerMax = 5;				//チャージ最大パワー（敵を倒せる量）
	float m_chargeShotWait;							//チャージショットのクールタイム
	bool m_bChargeMaxFlg;							//チャージマックスになったらtrue
	std::shared_ptr<ChargeAnim> m_chargeAnim=nullptr;		//チャージショットのアニメーション
	Math::Vector2 m_shotPosOffset;						//チャージ弾との距離

	float m_deadTimer;								//倒れ初めからの時間を測る
	std::vector<std::vector<bool>> m_bDraw;			//プレイヤーのビットごとに描画するかどうかのフラグ

	//プレイヤーの明るさを変化させ用
	float m_deg;			//アルファ値をサインカーブで表現する用
	float m_deltaDeg;
	float m_alphaMax;
};