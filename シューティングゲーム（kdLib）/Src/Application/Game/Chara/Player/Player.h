#pragma once
#include"../CharaBase.h"

class FireworksManager;

class Player :public CharaBase
{
public:

	Player();

	void Init(FireworksManager* set);

	//更新
	void Update(float deltaTime);

	//描画
	void Draw();

	//行動
	void Action(float deltaTime);

	//ダメージ
	void Damage(float damage) {
		m_hp -= damage;
		if (m_hp <= 0)
		{
			m_hp = 0;
			Dead();			//倒れた時の処理を呼び出す
		}
		if (m_hp > m_hpMax)m_hp = m_hpMax;
	}

	//回復
	void Recover(float recover)
	{
		m_hp + recover;
		if (m_hp < 0)m_hp = 0;
		if (m_hp > m_hpMax)m_hp = m_hpMax;
	}

	//プレイヤーを無敵にする(引数が何秒間無敵にするか)
	void Invincible(float time)
	{
		m_bInvincible = true;
		m_invincibleTime = time;
		m_color = { 1,1,1,0.2f };
	}

	//倒れたときの処理
	void Dead();

	//無敵かどうか
	bool IsInvincible() { return m_bInvincible; }

	float GetHP() { return m_hp; }

	float* GetHPAddress() { return &m_hp; }
	float* GetHPMaxAddress() { return &m_hpMax; }

	KdTexture* GetTex() { return &m_tex; }


private:

	static constexpr float s_speedMax = 200.0f;

	float m_hp;
	float m_hpMax;

	KdTexture m_tex;
	float m_scale;

	float m_shotWait;					//弾を撃つ待機時間

	bool m_bInvincible;					//無敵かどうかのフラグ
	float m_invincibleTime;				//無敵時間
	Math::Color m_color;

	FireworksManager *m_pFireworksManager;

	float m_sumDeltaTime;				//経過した時間を足していく(無敵状態のときの透明度を切り替える処理で使用)
};