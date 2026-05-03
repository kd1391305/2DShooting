#include"UI.h"
#include"Score/Score.h"
#include"../main.h"
#include"../Tools/Gauge/Gauge.h"
#include"../Chara/Player/Player.h"
#include"../Mouse/Mouse.h"
#include"Explan/Explan.h"

//初期化
void UI::Init(Player* player)
{
	//スコアの初期化
	m_score = std::make_shared<Score>();
	m_score->Init();

	//ゲージの初期化
	m_gauge = std::make_shared<Gauge>();
	m_gauge->Init(
		{ -470,300 },					//ゲージの中心座標
		{ 80,8 },						//ゲージの半径
		player->GetHPMaxAddress(),		//MaxHPのアドレス
		player->GetHPAddress(),			//HPのアドレス
		1);								//ゲージが減るスピード

	m_pPlayerPos = player->GetPosAddress();

	m_explan = std::make_shared<Explan>();
	m_explan->Init(m_pPlayerPos);
}

//更新
void UI::Update(float deltaTime)
{
	//ゲージの更新
	m_gauge->Update();

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

//初期化
void UI::Draw()
{
	//UI背景描画
	SHADER.m_spriteShader.ClearMatrix();
	Math::Vector2 radius = { 250,40 };
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

	//スコアの描画
	m_score->Draw();

	//ゲージの描画
	m_gauge->Draw();

	if (m_explan)m_explan->Draw();

}