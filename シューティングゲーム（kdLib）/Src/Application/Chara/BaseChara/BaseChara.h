#pragma once

class Game;

class BaseChara
{
public:

	BaseChara() :
		m_radius(32,32)
	{}
	virtual ~BaseChara() {}

	//=============================================
	// 純粋仮想関数
	//=============================================
	//更新
	virtual void Update(float deltaTime) = 0;
	//倒れたときの処理
	virtual void Dead() = 0;
	//=============================================
	
	//描画(EnemyとCharaで描画への引数が異なるため不要)
	//virtual void Draw();
	

	//ダメージ
	void Damage(float damage) {
		m_hp -= damage;
		if (m_hp <= 0)
		{
			m_hp = 0;
			Dead();			//倒れた時の処理を呼び出す
		}
		if (m_hp > m_hpMax)m_hp = m_hpMax;
	}

	Math::Vector2 GetPos() { return m_pos; }
	Math::Vector2 GetMove() { return m_move; }
	Math::Vector2 GetRadius() { return m_radius; }
	float GetHp() { return m_hp; }

	void SetPos(Math::Vector2 set) { m_pos = set; }
	void SetMove(Math::Vector2 set) { m_move = set; }
	void SetRadius(Math::Vector2 set) { m_radius = set; }

protected:

	Math::Vector2 m_pos;		//座標
	Math::Vector2 m_move;		//移動量
	Math::Vector2 m_radius;		//半径（当たり判定、画面端判定）
	float m_scale;				//画像の拡縮
	Math::Matrix m_mat;			//行列
	Math::Color m_color;		//色
	float m_hp;					//体力
	float m_hpMax;				//最大体力
};