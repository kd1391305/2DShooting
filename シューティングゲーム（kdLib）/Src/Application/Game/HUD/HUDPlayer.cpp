#include"HUDPlayer.h"
#include"../Chara/Player/Player.h"

//コンストラクタ
void C_HUDPlayer::Init(C_Player*player)
{
	//ゲージ構造体の初期化
	m_gauge.Init(
		{ -500,300 },							//ゲージの中心座標
		{ 130,10 },									//ゲージの半径
		player->GetHPMaxAddress(),		//MaxHPのアドレス
		player->GetHPAddress(),			//HPのアドレス
		1);											//ゲージが減るスピード
}

//更新
void C_HUDPlayer::Update()
{
	m_gauge.Update();
}

//描画
void C_HUDPlayer::Draw()
{
	//ゲージを描画
	m_gauge.Draw();
}