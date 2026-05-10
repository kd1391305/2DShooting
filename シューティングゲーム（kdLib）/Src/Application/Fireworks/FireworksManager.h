#pragma once

class BaseFireworks;
class Fireworks1;
class Fireworks2;
class Fireworks3;
class Fireworks4;
class Fireworks5;

//花火を管理するクラス
class FireworksManager
{
public:

	//花火の種類
	enum Type
	{
		Circle,
		NewCircle,
		Petal,
		Kind
	};



	FireworksManager()
	{
		Init();
	}

	~FireworksManager() { Release(); }

	void Update(float deltaTime);

	void Draw(float backScale);

	//花火を撃つ
	void Shoot(Type name, Math::Vector2& pos, Math::Vector2& startMove, float beforeScale, float afterScale, Math::Color& color, const float seVolume = 0.0f);

	//花火を弾けさせる（弾けた状態のまま出現）
	void Explode(Type name, Math::Vector2& pos, float afterScale, Math::Color& color, const float seVolume = 0.0f);

	//プレイヤーが撃った花火配列を返す
	std::vector<std::shared_ptr<BaseFireworks>>& GetList() { return m_list; }

	//処理速度が早い花火の種類をランダムで返す
	static Type GetRandomType_Quick()
	{
		switch (rand() % 3)
		{
		case 0:
			return Type::Circle;
		case 1:
			return Type::NewCircle;
		case 2:
			return Type::Petal;
		default:
			return Type::Petal;
		}
	}

private:

	//初期化
	void Init();

	//解放
	void Release();

	//オブジェクトプールから取り出す
	std::shared_ptr<BaseFireworks>PopPool(Type type);

	void PushPool(Type type, std::shared_ptr<BaseFireworks> fireworks);

	//活性状態の花火リスト
	std::vector<std::shared_ptr<BaseFireworks>>	m_list;

	//オブジェクトプールリスト
	std::vector<std::shared_ptr<Fireworks1>> m_pool1;
	std::vector<std::shared_ptr<Fireworks3>> m_pool3;
	std::vector<std::shared_ptr<Fireworks4>> m_pool4;

	const int m_poolSize = 30;

	float m_timer = 0;
};