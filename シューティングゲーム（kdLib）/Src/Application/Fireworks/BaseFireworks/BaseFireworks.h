#pragma once

//円用のパーティクル
struct Particle
{
	Math::Vector2 m_pos;	//座標
	Math::Vector2 m_move;	//移動量
	Math::Color m_color;	//色
	float m_life;			//生存時間

	//コンストラクタ
	Particle() {}
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
		if (m_life <= 0)
		{
			m_color.A(m_color.A() - 0.2 * deltaTime);
		}
		//生存時間が終了したか？
		return m_life > 0 || m_color.A() > 0;
	}
};

//棒状のパーティクル用
struct Particle2
{
	Math::Vector2 m_pos;		//座標
	Math::Vector2 m_move;		//移動量	
	Math::Vector2 m_scale;		//拡縮
	Math::Color m_color;		//色	
	float m_life;				//生存時間
	Math::Matrix m_mat;			//行列
	float m_radian;				//角度

	//コンストラクタ
	Particle2() {}
	Particle2(Math::Vector2 pos, Math::Vector2 move, float radian, Math::Color color, float life) :
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
		Math::Matrix scale = Math::Matrix::CreateScale(m_scale.x, m_scale.y, 0);
		Math::Matrix rotation = Math::Matrix::CreateRotationZ(m_radian);
		Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
		m_mat = scale * rotation * trans;
		//生存時間を減らす
		m_life -= deltaTime;
		if (m_life <= 0)
		{
			m_color.A(m_color.A() -0.2 * deltaTime);
		}
		//生存時間が終了したか？
		return m_life > 0 || m_color.A() > 0;
	}
};

//Fireworks3で使用する専用パーティクル
struct Particle3
{
	Math::Vector2 m_pos;		//座標
	Math::Vector2 m_move;		//移動量	
	float m_scale;		//拡縮
	Math::Color m_color;		//色	
	float m_life;				//生存時間
	Math::Matrix m_mat;			//行列

	//更新
	bool Update(float deltaTime, float baseScale)
	{
		//座標更新
		m_pos += m_move * deltaTime;

		//行列作成
		Math::Matrix scale = Math::Matrix::CreateScale(m_scale * baseScale);
		Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
		m_mat = scale * trans;
		//生存時間を減らす
		m_life -= deltaTime;
		if (m_life <= 0)
		{
			m_color.A(m_color.A() - 0.2 * deltaTime);
		}
		//生存時間が終了したか？
		return m_life > 0 || m_color.A() > 0;
	}

	void Draw(std::shared_ptr<KdTexture>tex)
	{
		SHADER.m_spriteShader.SetMatrix(m_mat);
		SHADER.m_spriteShader.DrawTex_Src(tex, m_color);
	}
};

//花火クラス
class BaseFireworks
{
public:

	BaseFireworks() {}

	virtual ~BaseFireworks() {}

	//初期化
	virtual void Init() = 0;

	//更新
	virtual void Update(float deltaTime) = 0;

	//描画
	virtual void Draw() = 0;

	//花火を打ち上げる
	virtual void Shot(Math::Vector2& startPos, Math::Vector2&startMove, float beforeScale, float afterScale, Math::Color& color) = 0;

	//花火を弾けさせる
	virtual void Explode() = 0;

	//花火が活性化しているか？
	bool IsActive() { return m_bActive; }

	//花火が弾けたか？
	bool IsExploded() { return m_bExploded; }

protected:

	//解放する
	virtual void Release() {};

	//スクリーンアウト（画面外に出たか？）
	bool IsScreenOut();

	Math::Vector2 m_pos;	//座標
	Math::Vector2 m_move;	//移動量
	Math::Color m_color;	//色

	bool m_bExploded = false;				//弾けたか
	bool m_bActive = false;					//活性化状態か

	//花火クラスはKdTextureに描画してからKdtextureをBuckbufferに描画する
	//そのための変数
	std::shared_ptr< KdTexture> m_tex;		//画像	
	float m_texRadius;		//画像半径
	float m_beforeScale;	//花火が弾けるまでの拡縮
	float m_afterScale;		//花火が弾けた後の拡縮

	bool m_bDead_ScreenOut = false;			//スクリーンアウトしたら消えるか？

	const float m_explodeSpeedMax = 150;		//弾ける速度の最大値
	const float m_gravity = 70;					//重力
};



