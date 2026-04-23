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

private:
	POINT m_pos;				//座標
	Math::Matrix m_mat;		//行列
	KdTexture m_tex;			//画像

	//シングルトン
private:
	Mouse() 
	{
		m_tex.Load("Texture/Mouse.png");
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
