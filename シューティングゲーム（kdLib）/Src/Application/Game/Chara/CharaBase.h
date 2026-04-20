#pragma once

class C_CharaBase
{
public:

	C_CharaBase() :
		m_radius(32,32)
	{}
	virtual ~C_CharaBase() {}

	//更新
	virtual void Update() = 0;
	//描画
	virtual void Draw() = 0;

	Math::Vector2 GetPos() { return m_pos; }
	Math::Vector2 GetMove() { return m_move; }
	Math::Vector2 GetRadius() { return m_radius; }

	void SetPos(Math::Vector2 set) { m_pos = set; }
	void SetMove(Math::Vector2 set) { m_move = set; }
	void SetRadius(Math::Vector2 set) { m_radius = set; }

protected:

	Math::Vector2 m_pos;			//座標
	Math::Vector2 m_move;		//移動量
	Math::Vector2 m_radius;		//半径
	Math::Matrix m_mat;				//行列
};