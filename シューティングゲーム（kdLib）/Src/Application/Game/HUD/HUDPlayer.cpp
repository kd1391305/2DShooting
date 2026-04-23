#include"HUDPlayer.h"
#include"../Chara/Player/Player.h"

//コンストラクタ
void HUDPlayer::Init(Player*player)
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
void HUDPlayer::Update()
{
	m_gauge.Update();
}

//描画
void HUDPlayer::Draw()
{
	//ゲージを描画
	m_gauge.Draw();
}