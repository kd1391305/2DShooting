#pragma once

class Mouse
{
public:

	void Update();

	void Draw();

	//マウスと当たったか（矩形当たり判定）
	bool Hit(Math::Vector2 pos, Math::Vector2 radius);

	//マウス座標を返す
	POINT GetPos() { return m_pos; }
	Math::Vector2 GetPosf() { return Math::Vector2(m_pos.x, m_pos.y); }

	void ShowCursor(bool show)
	{
		m_bShow = show;
	}

private:
	POINT m_pos;				//座標
	Math::Matrix m_mat;		//行列
	bool m_bShow;

	//シングルトン
private:
	Mouse() 
	{
		ShowCursor(false);
	}
public:
	static Mouse& Instance()
	{
		static Mouse instance;
		return instance;
	}
};

#define MOUSE Mouse::Instance()
