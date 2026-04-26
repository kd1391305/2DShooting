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
	void Draw(KdTexture* tex);

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
	void Draw(KdTexture* tex)override;

	//花火を打ち上げる
	void Shot(Math::Vector2 startPos, Math::Vector2 targetPos, float speed, Math::Vector2 beforeScale, Math::Vector2 afterScale, Math::Color color, const bool bTarget = false);

	//花火を弾けさせる
	void Explode()override;

protected:

	//解放する
	void Release()override;

	std::vector<Particle> m_circleList;		//花火の火（丸形）
	std::vector<Particle2> m_lineList;		//花火の火（棒形）
};
//===================================================


//===================================================
//花火3（チャージ弾)
//===================================================
class Fireworks3 :public BaseFireworks
{
public:

	Fireworks3() {}
	~Fireworks3() { Release(); }

	//初期化
	void Init()override;

	//更新
	void Update(float deltaTime)override;

	//描画
	void Draw(KdTexture* tex)override;

	//花火を打ち上げる
	void Shot(Math::Vector2 startPos, Math::Vector2 targetPos, float speed, Math::Vector2 beforeScale, Math::Vector2 afterScale, Math::Color color, const bool bTarget = false);

	//花火を弾けさせる
	void Explode()override;

	//敵を貫く（敵を貫いたエフェクト＆パワー減少）＝実装予定
	//引数（貫いた場所）
	void Pierce(Math::Vector2 pos);

	//パワーをセットする
	void SetPower(float set) { m_power = set; }

	//現在のパワーを返す
	float GetPower() { return m_power; }

protected:

	//解放する
	void Release()override;

	//弾けるときの花火
	std::vector<std::shared_ptr<BaseFireworks>> m_fireworksList;

	//敵を貫通する数
	float m_power;			//パワー１ = 一体の敵を倒す

};
//===================================================

