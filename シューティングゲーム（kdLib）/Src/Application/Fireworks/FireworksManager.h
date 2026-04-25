#pragma once

class BaseFireworks;

//花火を管理するクラス
class FireworksManager
{
public:

	FireworksManager()
	{
		Init();
	}

	~FireworksManager() { Release(); }

	void Update(float deltaTime);

	void Draw();

	//花火を撃つ
	void Shot(Math::Vector2 pos,Math::Vector2 targetP,Math::Vector2 scale,const bool bTarget=false); 

	//プレイヤーが撃った花火配列を返す
	std::vector<std::shared_ptr<BaseFireworks>>& GetList() { return m_list; }

private:

	//初期化
	void Init();

	//解放
	void Release();

	//当たり判定ありの花火（当たったら弾ける）= プレイヤーが撃った花火
	std::vector<std::shared_ptr<BaseFireworks>>	m_list;

	KdTexture m_tex;				 //Particle2用画像
};