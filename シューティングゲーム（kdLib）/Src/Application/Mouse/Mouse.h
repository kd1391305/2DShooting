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

	void ShowCursorTex(bool show)
	{
		m_bShow = show;
	}

	bool IsMove() { return m_bMove; }

private:
	POINT m_pos;			//座標
	Math::Matrix m_mat;		//行列
	bool m_bShow;

	bool m_bMove;			//マウスが動いたか？
	int m_stopCnt = 0;			//止まっているカウントを数える
	POINT m_beforePos;		//前のフレームの座標

	//シングルトン
private:
	Mouse() 
	{
		ShowCursor(false);
		m_bShow = true;
	}
public:
	static Mouse& Instance()
	{
		static Mouse instance;
		return instance;
	}
};

#define MOUSE Mouse::Instance()
