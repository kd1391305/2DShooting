#pragma once

class BaseFireworks;

//花火を管理するクラス
class FireworksManager
{
public:

	//花火の種類
	enum Type
	{
		Circle,
		Circle_Line,
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

	void Draw();

	//花火を撃つ
	void FireworksManager::Shot(FireworksManager::Type name, Math::Vector2& pos, Math::Vector2& startMove, float beforeScale, float afterScale, Math::Color& color);


	//プレイヤーが撃った花火配列を返す
	std::vector<std::shared_ptr<BaseFireworks>>& GetList() { return m_list; }

private:

	//初期化
	void Init();

	//解放
	void Release();

	//当たり判定ありの花火（当たったら弾ける）= プレイヤーが撃った花火
	std::vector<std::shared_ptr<BaseFireworks>>	m_list;
};