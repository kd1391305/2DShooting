#pragma once
#include"../BaseChara/BaseChara.h"

class FireworksManager;
class Fireworks3;
class Game;

class Player :public BaseChara
{
public:

	Player();

	void Init(Game*g,FireworksManager* f);

	//更新
	void Update(float deltaTime);

	//描画
	void Draw();

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

private:

	Game* m_pGame;
	FireworksManager* m_pFireworksManager;

	const float m_moveSpeed = 100;

	float m_shotWait;					//弾を撃つ待機時間

	bool m_bInvincible;					//無敵かどうかのフラグ
	float m_invincibleTime;				//無敵時間


	float m_sumDeltaTime;				//経過した時間を足していく(無敵状態のときの透明度を切り替える処理で使用)

	//倒れた時のアニメーション用の値を初期化
	void InitDeadAnim();

	//倒れた時のアニメーションを描画
	void DrawDeadAnim();


	std::shared_ptr<Fireworks3> m_chargeBullet;		//チャージ用の弾
	float m_chargeTime;									//チャージ時間

	float m_deadTimer;									//倒れ初めからの時間を測る
	std::vector<std::vector<bool>> m_bDraw;				//プレイヤーのビットごとに描画するかどうかのフラグ
};