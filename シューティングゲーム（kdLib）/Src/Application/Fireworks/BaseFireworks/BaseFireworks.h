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

		//空気抵抗
		m_move *= 0.98f;

		//生存時間を減らす
		m_life -= deltaTime;
		if (m_life <= 0)
		{
			m_color *= 0.98f;
		}
		//生存時間が終了したか？
		return m_life > 0 || m_color.A() > 0.1f;
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

		//空気抵抗
		m_move *= 0.98f;

		//行列作成
		Math::Matrix scale = Math::Matrix::CreateScale(m_scale.x, m_scale.y, 0);
		Math::Matrix rotation = Math::Matrix::CreateRotationZ(m_radian);
		Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
		m_mat = scale * rotation * trans;
		//生存時間を減らす
		m_life -= deltaTime;
		if (m_life <= 0)
		{
			m_color *= 0.98f;
		}
		//生存時間が終了したか？
		return m_life > 0 || m_color.A() > 0.1f;
	}
};

//Fireworks3で使用する専用パーティクル
struct Particle3
{
	Math::Vector2 m_pos;		//座標
	Math::Vector2 m_move;		//移動量	
	float m_scale;				//拡縮
	Math::Color m_color;		//色	
	float m_life;				//生存時間
	Math::Matrix m_mat;			//行列

	//更新
	bool Update(float deltaTime, float baseScale)
	{
		//座標更新
		m_pos += m_move * deltaTime;

		//空気抵抗
		m_move *= 0.98f;

		//行列作成
		Math::Matrix scale = Math::Matrix::CreateScale(m_scale * baseScale);
		Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
		m_mat = scale * trans;
		//生存時間を減らす
		m_life -= deltaTime;
		if (m_life <= 0)
		{
			m_color *= 0.98f;
		}
		//生存時間が終了したか？
		return m_life > 0 || m_color.A() > 0.1f;
	}

	void Draw(std::shared_ptr<KdTexture>tex)
	{
		SHADER.m_spriteShader.SetMatrix(m_mat);
		SHADER.m_spriteShader.DrawTex_Src(tex, m_color);
	}
	void Draw(KdTexture*tex)
	{
		SHADER.m_spriteShader.SetMatrix(m_mat);
		SHADER.m_spriteShader.DrawTex_Src(tex, m_color);
	}
};

struct Trail
{
	std::deque<Math::Vector2> m_pos;
	float m_radian;
	int m_maxTrailSize = 7;							// 何フレーム分の軌跡をほぞんするか？

	void Update(Math::Vector2 currentPos) {
		// 1. 最新の座標を末尾に追加
		m_pos.push_back(currentPos);

		// 2. 設定した長さを超えたら、一番古い先頭を削除
		if (m_pos.size() > m_maxTrailSize) {
			m_pos.pop_front();
		}
	}
};

struct Particle4
{
	Math::Vector2 m_pos;
	Math::Vector2 m_move;
	Trail m_trail;
	Math::Color m_color;
	float m_life;

	//更新
	bool Update(float deltaTime)
	{
		//座標更新
		m_pos += m_move * deltaTime;

		//空気抵抗
		m_move *= 0.98f;

		//トレイルの更新
		m_trail.Update(m_pos);

		//生存時間を減らす
		m_life -= deltaTime;
		if (m_life <= 0)
		{
			m_color *= 0.98f;
		}
		//生存時間が終了したか？
		return m_life > 0 || m_color.A() > 0.01f;
	}

	void Draw(KdTexture* tex)
	{
		if(m_trail.m_pos.size() < 2)return;

		Math::Color color;
		Math::Matrix scaleMat, rotateMat, transMat;
		for (int i = 0; i < m_trail.m_pos.size() - 1; i++)
		{
			color = m_color / m_trail.m_maxTrailSize;
			color.A(color.A() / 2.0f);
			color = color * i;
			scaleMat = Math::Matrix::CreateScale(0.08f * i, 0.04f * i, 0);
			rotateMat = Math::Matrix::CreateRotationZ(m_trail.m_radian);
			transMat = Math::Matrix::CreateTranslation(m_trail.m_pos[i].x, m_trail.m_pos[i].y, 0);
			SHADER.m_spriteShader.SetMatrix(scaleMat * rotateMat * transMat);
			SHADER.m_spriteShader.DrawTex_Src(tex, color);
		}
	}
};

//花火クラス
class BaseFireworks
{
public:

	enum Type
	{
		Circle,
		Circle_Line,
		NewCircle,
		Petal,
		Trail,
	};

	BaseFireworks() {}

	virtual ~BaseFireworks() {}

	//初期化
	virtual void Init() = 0;

	//更新
	virtual void Update(float deltaTime) = 0;

	//描画
	virtual void Draw(float backScale) = 0;

	//花火を打ち上げる
	virtual void Shoot(Math::Vector2& startPos, Math::Vector2&startMove, float beforeScale, float afterScale, Math::Color& color) = 0;

	//花火を弾けさせる
	virtual void Explode() = 0;

	//花火が活性化しているか？
	bool IsActive() { return m_bActive; }

	//花火が弾けたか？
	bool IsExploded() { return m_bExploded; }

	void Init_Explode(Math::Vector2& pos, float afterScale, Math::Color& color)
	{
		m_pos = pos;
		m_afterScale = afterScale;
		m_color = color;
	}

	Type GetType() { return m_type; }
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

	const float m_explodeSpeedMax = 350;		//弾ける速度の最大値
	const float m_shotGravity = 70;				//撃ちあがる時の重力
	const float m_particleGravity = 20;			//パーティクルの重力
	Type m_type;
};



