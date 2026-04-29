#pragma once

class BaseFireworks;
class Fireworks3;

//花火を管理するクラス
class FireworksManager
{
public:

	//花火の種類
	enum Type
	{
		Circle,
		Circle_Line,
		Petal,
		Kind
	};



	FireworksManager()
	{
		Init();
	}

	~FireworksManager() { Release(); }

	void Update(float deltaTime);

	void Draw();

	//花火を撃つ
	void Shot(FireworksManager::Type name,Math::Vector2 pos, Math::Vector2 targetPos, float  speed, Math::Vector2 beforeScale, Math::Vector2 afterScale, Math::Color color, const bool bTarget = false);
	void Wait(std::shared_ptr<Fireworks3>fireworks);
	

	//プレイヤーが撃った花火配列を返す
	std::vector<std::shared_ptr<BaseFireworks>>& GetList() { return m_list; }
	std::vector<std::shared_ptr<Fireworks3>>& GetChargeBullet() { return m_chargeBullet; }

private:

	//初期化
	void Init();

	//解放
	void Release();

	//チャージ弾
	std::vector<std::shared_ptr<Fireworks3>>	m_chargeBullet;

	//当たり判定ありの花火（当たったら弾ける）= プレイヤーが撃った花火
	std::vector<std::shared_ptr<BaseFireworks>>	m_list;
};