#pragma once
#include"BaseFireworks/BaseFireworks.h"

//===================================================
//花火１（円のみで描画）
//===================================================
class Fireworks1 :public BaseFireworks
{
public:

	Fireworks1() {}
	~Fireworks1() { Release(); }

	//初期化
	void Init();

	//更新
	void Update(float deltaTime);

	//描画
	void Draw();

	//花火を打ち上げる
	void Shot(Math::Vector2 startPos, Math::Vector2 targetPos, float speed, Math::Vector2 beforeScale, Math::Vector2 afterScale, Math::Color color, const bool bTarget = false);

	//花火を弾けさせる
	void Explode();

private:

	//解放する
	void Release();

	std::vector<Particle> m_circleList;		//花火の火（丸形）
};
//===================================================

//===================================================
//花火２（円と棒で描画）
//===================================================
class Fireworks2:public BaseFireworks
{
public:

	Fireworks2() {}
	~Fireworks2() { Release(); }

	//初期化
	void Init()override;

	//更新
	void Update(float deltaTime)override;

	//描画
	void Draw()override;

	//花火を打ち上げる
	void Shot(Math::Vector2 startPos, Math::Vector2 targetPos, float speed, Math::Vector2 beforeScale, Math::Vector2 afterScale, Math::Color color, const bool bTarget = false);

	//花火を弾けさせる
	void Explode()override;

protected:

	//解放する
	void Release()override;

	std::vector<Particle> m_circleList;		//花火の火（丸形）
	std::vector<Particle2> m_lineList;		//花火の火（棒形）

	Math::Vector2 m_lineBaseScale;
};
//===================================================


//===================================================
//花火3（チャージ弾)
//===================================================
class Fireworks3
{
public:

	Fireworks3() {}
	~Fireworks3() { Release(); }

	//初期化
	void Init();
	//更新
	void Update(float deltaTime);

	//描画
	void Draw();

	//チャージスタート
	void StartCharge(Math::Vector2 startPos, Math::Vector2 targetPos, Math::Vector2 beforeScale, Math::Vector2 afterScale, Math::Color color);

	//花火を打ち上げる
	void Shot();

	//花火を弾けさせる
	void Explode();

	//敵を貫く（敵を貫いたエフェクト＆パワー減少）＝実装予定
	//引数（貫いた場所）
	void Pierce(Math::Vector2 pos);

	//現在のパワーを返す
	float GetPower() { return m_power; }

	void SetPos(Math::Vector2 set) { 
		m_pos = set;
		for (auto& c : m_fireworksList)c->SetPos(m_pos);
	}
	void SetTargetPos(Math::Vector2 set) { 
		m_targetPos = set;
	for (auto& c : m_fireworksList)c->SetTargetPos(m_pos);
	}
	void SetBeforeScale(Math::Vector2 set) { 
		m_beforeScale = set; 
		for (auto& c : m_fireworksList)c->SetBeforeScale(m_beforeScale);
	}
	void SetAfterScale(Math::Vector2 set) {
		m_afterScale = set; 
		for (auto& c : m_fireworksList)c->SetAfterScale(m_afterScale);
	}
	void SetColor(Math::Color set) { 
		m_color = set; 
		for (auto& c : m_fireworksList)c->SetColor(m_color);
	}
	void SetSpeed(float set) {
		m_speed = set;
	for (auto& c : m_fireworksList)c->SetSpeed(m_speed);
	}

	void SetPower(float set)	{
		m_power = set;
		while (m_fireworksList.size() < m_power)
		{
			Create(); 
		}
	}


	Math::Vector2 GetPos() { return m_pos; }
	Math::Vector2 GetTargetPos() { return m_targetPos; }
	float GetRadius() { return m_radius; }
	float GetSpeed() { return m_speed; }
	Math::Vector2 GetBeforeScale() { return m_beforeScale; }
	Math::Vector2 GetAfterScale() { return m_afterScale; }
	Math::Color GetColor() { return m_color; }

	bool GetExplodedFlg() { return m_bExplodedFlg; }
	bool GetChargeFlg() { return m_bChargeFlg; }
	bool GetActiveFlg() { return m_bActive; }

private:

	//花火を作成する
	void Create();
	
	//解放する
	void Release();

	//弾けるときの花火
	std::vector<std::shared_ptr<BaseFireworks>> m_fireworksList;
	
	//全ての花火に共通する部分（この値をもとにすべての花火を少しランダムにする）
	Math::Vector2 m_pos;
	Math::Vector2 m_move;
	Math::Vector2 m_targetPos;
	Math::Color m_color;
	Math::Vector2 m_beforeScale;
	Math::Vector2 m_afterScale;
	float m_speed;

	float m_radius;				//当たり判定用の半径

	//敵を貫通する数
	float m_power;				//パワー１ = 一体の敵を倒す

	bool m_bExplodedFlg;		//弾けたか？
	bool m_bChargeFlg;			//チャージ中か？
	bool m_bActive;				//活性状態か？
};
//===================================================

//===================================================
//花火4（花弁画像だけで描画）
//===================================================
class Fireworks4 :public BaseFireworks
{
public:

	Fireworks4() {}
	~Fireworks4() { Release(); }

	//初期化
	void Init()override;

	//更新
	void Update(float deltaTime)override;

	//描画
	void Draw()override;

	//花火を打ち上げる
	void Shot(Math::Vector2 startPos, Math::Vector2 targetPos, float speed, Math::Vector2 beforeScale, Math::Vector2 afterScale, Math::Color color, const bool bTarget = false);

	//花火を弾けさせる
	void Explode()override;

protected:

	//解放する
	void Release()override;

	std::vector<Particle> m_petal;		//花びら
};
//===================================================

