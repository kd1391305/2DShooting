#pragma once

struct Particle
{
	Math::Vector2 m_pos;		//座標
	Math::Vector2 m_move;	//移動量
	Math::Color m_color;				//色
	float m_life;							//生存時間

	//コンストラクタ
	Particle() = delete;
	Particle(Math::Vector2 pos, Math::Vector2 move, Math::Color color, float life) :
		m_pos(pos), m_move(move), m_color(color), m_life(life)
	{
	}

	//更新
	bool Update(double deltaTime)
	{
		//座標更新
		m_pos += m_move * deltaTime;
		//生存時間を減らす
		m_life -= deltaTime;
		//生存時間が終了したか？
		return m_life > 0;
	}
};

//棒状のパーティクル用
struct Particle2
{
	Math::Vector2 m_pos;		//座標
	Math::Vector2 m_move;			//移動量	
	Math::Color m_color;				//色	
	float m_life;						//生存時間
	Math::Matrix m_mat;			//行列
	float m_radian;					//角度

	//コンストラクタ
	Particle2() = delete;
	Particle2(Math::Vector2 pos, Math::Vector2 move, Math::Color color, float life) :
		m_pos(pos), m_move(move), m_color(color), m_life(life)
	{
		//角度を求める
		m_radian = atan2f(move.y, move.x);
	}

	//更新
	bool Update(double deltaTime)
	{
		//座標更新
		m_pos += m_move * deltaTime;

		//行列作成
		Math::Matrix scale = Math::Matrix::CreateScale(0.5f, 0.5f, 0);
		Math::Matrix rotation = Math::Matrix::CreateRotationZ(m_radian);
		Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
		m_mat = scale * rotation * trans;
	
		//生存時間を減らす
		m_life -= deltaTime;
		//生存時間が終了したか？
		return m_life > 0;
	}
};

//花火クラス
class C_Fireworks
{
public:
	~C_Fireworks()
	{
		//解放処理
		m_circleList.clear();
		m_lineList.clear();
	}

	//更新
	void Update();

	//描画
	void Draw(KdTexture* tex);

	//花火を打ち上げる
	void Shot(Math::Vector2 pos);

	//花火が終わったか？
	bool IsDead() { return m_bExploded && m_circleList.empty() && m_lineList.empty(); }

	//花火が活性化しているか？
	bool IsActive() { return m_bActive; }

private:

	Math::Vector2 m_pos;						//座標
	Math::Vector2 m_move;					//移動量
	std::vector<Particle> m_circleList;		//花火の火（丸形）
	std::vector<Particle2> m_lineList;		//花火の火（棒形）
	Math::Color m_color;							//色

	bool m_bExploded = false;					//爆発したか（弾けたか）
	bool m_bActive = false;						//活性化状態か
};