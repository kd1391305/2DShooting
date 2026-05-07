#pragma once

struct BackObject
{
	void Update(float scrollX, float allScale)
	{
		m_pos.x += scrollX;
		m_mat = Math::Matrix::CreateScale(m_scale*allScale, m_scale*allScale, 0) * Math::Matrix::CreateTranslation(m_pos.x * allScale, m_pos.y*allScale, 0);
	}

	void Draw(std::shared_ptr<KdTexture>tex)
	{
		SHADER.m_spriteShader.SetMatrix(m_mat);
		SHADER.m_spriteShader.DrawTex_Src(tex,m_color);
	}

	Math::Vector2 m_pos;
	float m_scale;
	Math::Matrix m_mat;
	Math::Color m_color;
};

//最背面にある画像専用の構造体
struct FarBackground
{
	Math::Vector2 m_pos;
	Math::Vector2 m_scale;
	Math::Matrix m_mat;

	void CreateMat(float allScale)
	{
		m_mat = Math::Matrix::CreateScale(m_scale.x * allScale, m_scale.y * allScale, 0) * Math::Matrix::CreateTranslation(m_pos.x * allScale, m_pos.y * allScale, 0);
	}

	void Draw(std::shared_ptr<KdTexture>tex)
	{
		SHADER.m_spriteShader.SetMatrix(m_mat);
		SHADER.m_spriteShader.DrawTex_Src(tex);
	}
};

struct LightParticle
{
	Math::Vector2 m_pos;
	Math::Vector2 m_move;
	Math::Vector2 m_radius;
	float m_degree;
	float m_deltaDegree;
	float m_alphaMin;		//アルファ値の最大
	float m_alphaMax;		//アルファ値の最小
	Math::Color m_color;
	float m_life;			//生存時間（０になった瞬間に消さない）

	void Update(float deltaTime);

	void Draw();
};

class FireworksManager;

class Back
{
public:
	Back() {}

	void Init();

	void Update(float deltaTime);

	void Draw();

	//ズームインする（ゲーム画面に遷移時に呼び出す）
	void StartZoomIn();

	//ズームアウトする（タイトル画面に遷移時に呼び出す）
	void StartZoomOut();

	std::shared_ptr<FireworksManager>GetFireworks() { return m_fireworks; }

private:

	void Respawn(LightParticle* light);

	//ズーム中のアップデート処理
	void UpdateZooming(float deltaTime);

	//通常時の更新
	void UpdateNormal(float deltaTime);

	
	std::list<FarBackground> m_farBg;			//最背面の背景用リスト
	float m_farBgWidth;							//最背面の背景の横幅
	const float m_scrollSpeed = 40.0f;

	float m_allScale;
	const float m_zoomInBackScale = 1.0f;		//ゲーム画面でのスケール
	const float m_zoomOutBackScale = 0.8f;		//タイトル画面でのスケール
	const float m_zoomSpeed = 0.4f;				//ズームイン ( アウト ) するスピード
	bool m_bZoomInFlg;							//ズームインしているか？(trueならズームイン falseならズームアウト)
	bool m_bZoomingFlg;							//ズーム中か？

	std::list<BackObject> m_lantern;			//ランタン（提灯）
	float m_lanternDistance;					//ランタンどうしの距離	
	const float m_lanternRenderScale = 0.2f;

	std::list< BackObject> m_fance;						//フェンス（柵）
	float m_fanceWidth ;
	const float m_fanceRenderWidth = 115.0f;	//描画用のフェンスの幅（※画像の半径ではなく、ちょうどよい描画感覚）
	const float m_fanceRenderScale = 1.0f;

	LightParticle m_lightParticleList[30];		//埃のような粒子

	std::shared_ptr<FireworksManager> m_fireworks;
};

