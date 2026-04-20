#pragma once
#include"../SceneBase.h"
#include"Chara/Player/Player.h"
#include"Back/Back.h"
#include"HUD/HUD.h"
#include"Chara/Player/FormerPlayers.h"

class C_Game :public C_SceneBase
{
public:

	C_Game() = delete;
	C_Game(int level);
	~C_Game()override;

	void Update();
	void Draw();

	int GetGameLevel() { return m_gameLevel; }

private:

	bool IsGameOver();
	bool IsGameClear();

	C_Player m_player;
	C_Back m_back;
	C_HUD m_HUD;

	INT8 m_gameLevel;			//ÉQÅ[ÉÄÉåÉxÉã

	C_FormerPlayers m_fPlayers;

};
