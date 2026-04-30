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
	//初期化
	virtual void Init() = 0;
	//更新
	virtual void Update(float deltaTime) = 0;
	//描画
	virtual void Draw() = 0;
	//倒れたときの処理
	virtual void Dead() = 0;
private:
	//解放
	virtual void Release() = 0;
	//=============================================
	
public:
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
	Math::Color GetColor() { return m_color; }
	float GetHp() { return m_hp; }

	void SetPos(Math::Vector2 set) { m_pos = set; }
	void SetMove(Math::Vector2 set) { m_move = set; }
	void SetRadius(Math::Vector2 set) { m_radius = set; }

protected:

	Math::Vector2 m_pos;		//座標
	Math::Vector2 m_move;		//移動量
	float m_moveSpeed;			//移動スピード
	Math::Vector2 m_radius;		//半径（当たり判定、画面端判定）
	float m_scale;				//画像の拡縮
	Math::Matrix m_mat;			//行列
	Math::Color m_color;		//色
	float m_hp;					//体力
	float m_hpMax;				//最大体力
	float m_animCnt;			//アニメーションカウンター
	float m_animCntMax;			//アニメーションの終わり（何コマか？）
	float m_animSpeed;			//アニメーションスピード

	float	m_shotWaitTimer;	//撃つまでのクールタイムを測る
	float   m_shotWait;			//クールタイム
};