#include"UI.h"
#include"Score/Score.h"
#include"../main.h"
#include"../Tools/Gauge/Gauge.h"
#include"../Chara/Player/Player.h"
#include"Explan/Explan.h"
#include"../TextureCache/TextureCache.h"
#include"../Font/DWriteCustom.h"

//初期化
void UI::Init(Player* player)
{
	//スコアの初期化
	m_score = std::make_shared<Score>();
	m_score->Init();

	//ゲージの初期化
	m_playerGauge = std::make_shared<Gauge>();
	m_playerGauge->Init(
		{ -515,310 },					//ゲージの中心座標
		{ 60,6 },						//ゲージの半径
		player->GetHPMaxAddress(),		//MaxHPのアドレス
		player->GetHPAddress(),			//HPのアドレス
		1);								//ゲージが減るスピード



	m_explan = std::make_shared<Explan>();
	m_explan->Init();
}

//更新
void UI::Update(float deltaTime)
{
	//プレイヤーのゲージの更新
	m_playerGauge->Update();

	//ボスのゲージの更新
	if (m_bossGauge)m_bossGauge->Update();

	if (m_explan)
	{
		m_explan->Update(deltaTime);
		if (m_explan->IsEnd())
		{
			m_explan = nullptr;
		}
	}

	m_score->Update();
}

void UI::Draw()
{
	//UI背景描画
	SHADER.m_spriteShader.ClearMatrix();
	Math::Vector2 radius = { 220,32 };
	Math::Color color = { 0.1f,0.1f,0.1f,0.7f };
	float cx = SCREEN_LEFT + radius.x;
	float cy = SCREEN_TOP - radius.y;
	SHADER.m_spriteShader.DrawBox(cx, cy, radius.x, radius.y,&color,true);
	float x1, x2, x3, y1, y2, y3;
	x1 = cx + radius.x;
	x2 = cx + radius.x;
	x3 = x1 + 80;
	y1 = SCREEN_TOP;
	y2 = cy - radius.y;
	y3 = SCREEN_TOP;
	SHADER.m_spriteShader.DrawTriangle(x1, y1, x2, y2, x3, y3, &color,true);
	Math::Color gray = { 0.8f,0.8f,0.8f,0.9f };
	SHADER.m_spriteShader.DrawLine(x2, y2, x3, y3, &gray);
	SHADER.m_spriteShader.DrawLine(x2, y2, SCREEN_LEFT, y2, &gray);

	//プレイヤーアイコンの描画
	{
		Math::Matrix scaleMat, transMat;
		scaleMat = Math::Matrix::CreateScale(1.3f, 1.1f, 0);
		transMat = Math::Matrix::CreateTranslation(-610, 330,0);

		SHADER.m_spriteShader.SetMatrix(scaleMat *transMat);
		SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/Player/PlayerIcon.png"));
	}
	//スコアの描画
	m_score->Draw();

	DWriteCustom::Instance().SetShadow({ -1,-1 }, { 0.0f, 0.7f, 0.7f, 0.7f });
	DWriteCustom::Instance().Draw("体力", { -575, 345 },15);
	DWriteCustom::Instance().SetShadow({}, {});
	//ゲージの描画
	m_playerGauge->Draw();

	//ボスのゲージの描画
	if (m_bossGauge)m_bossGauge->Draw();

	if (m_explan)m_explan->Draw();

}

void UI::CreateBossGauge(float* hpMax, float* hp)
{
	m_bossGauge = std::make_shared<Gauge>();
	m_bossGauge->Init(
		Math::Vector2{ 0,SCREEN_BOTTOM + 12 },					//ゲージの中心座標
		Math::Vector2{ 400,6 },									//ゲージの半径
		hpMax,										//MaxHPのアドレス
		hp,											//HPのアドレス
		2);											//ゲージが減るスピード
	m_bossGauge->SetColor(Math::Color{ 0.5f,0,0.5f,0.7f });
}
