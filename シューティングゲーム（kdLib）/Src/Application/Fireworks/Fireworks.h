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

