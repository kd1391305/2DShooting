#include"UI.h"
#include"Score/Score.h"
#include"../main.h"
#include"../Tools/Gauge/Gauge.h"
#include"../Chara/Player/Player.h"
#include"../Mouse/Mouse.h"

//初期化
void UI::Init(Player* player)
{
	//スコアの初期化
	m_score = std::make_shared<Score>();
	m_score->Init();

	//ゲージの初期化
	m_gauge = std::make_shared<Gauge>();
	m_gauge->Init(
		{ -500,300 },					//ゲージの中心座標
		{ 130,10 },						//ゲージの半径
		player->GetHPMaxAddress(),		//MaxHPのアドレス
		player->GetHPAddress(),			//HPのアドレス
		1);								//ゲージが減るスピード

	m_pPlayerPos = player->GetPosAddress();
}

//更新
void UI::Update()
{
	//ゲージの更新
	m_gauge->Update();
}

//初期化
void UI::Draw()
{
	//UI背景描画
	SHADER.m_spriteShader.ClearMatrix();
	Math::Vector2 radius = { 280,60 };
	float cx = SCREEN_LEFT + radius.x;
	float cy = SCREEN_TOP - radius.y;
	SHADER.m_spriteShader.DrawBox(cx, cy, radius.x, radius.y, &Math::Color{ 0.3f,0.3f,0.3f,0.6f });

	//スコアの描画
	m_score->Draw();

	//ゲージの描画
	m_gauge->Draw();

	//プレイヤーからマウスカーソルを線でつなぐ
	Math::Vector2 mouse = MOUSE.GetPosf();
	SHADER.m_spriteShader.DrawLine(mouse.x, mouse.y, m_pPlayerPos->x, m_pPlayerPos->y, &Math::Color{ 1,0.5f,0.5f,0.5f });


}