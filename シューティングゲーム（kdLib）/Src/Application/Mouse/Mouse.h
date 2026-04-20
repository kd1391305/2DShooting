#pragma once

class C_Mouse
{
public:

	void Update();

	//マウスと当たったか（矩形当たり判定）
	bool Hit(Math::Vector2 pos, Math::Vector2 radius);

	//マウス座標を返す
	POINT GetPos() { return m_pos; }

private:
	POINT m_pos;

	//シングルトン
private:
	C_Mouse() {}
public:
	static C_Mouse& GetInstance()
	{
		static C_Mouse instance;
		return instance;
	}
};

#define MOUSE C_Mouse::GetInstance()
