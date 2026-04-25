#pragma once

//ドロップした経験値だけのためのクラス（プレイヤーの経験値やレベルはプレイヤークラスで管理）

class DropExp
{
public:

	DropExp() {}
	~DropExp() { Release(); }

	//初期化
	void Init();

	//更新
	void Update();

	//描画
	void Draw();

	//ドロップした経験値を消す
	void Delete(int i)
	{
		if (i < m_dropList.size())
		{
			m_dropList.erase(m_dropList.begin() + i);
		}
	}

	//経験値をドロップする
	void Drop(Math::Vector2 pos)
	{
		m_dropList.push_back(pos);
	}

private:

	//解放
	void Release()
	{
		m_dropList.clear();
	}

	//ドロップしている経験値の情報を格納
	std::vector<Math::Vector2>m_dropList;

	float m_radius;
};


