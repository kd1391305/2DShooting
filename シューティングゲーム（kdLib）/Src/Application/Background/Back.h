#pragma once
#include"Sky.h"

class Back
{
public:
	Back() {}

	void Init();

	void Update();

	void Draw();

private:

	KdTexture m_tex;		
	static const int s_drawNum = 2;			//•`‰æ‰ñ”
	Math::Vector2 m_pos[s_drawNum];			//À•W
	Math::Matrix m_mat[s_drawNum];			//s—ñ

	Sky m_sky;	//‹ó
};
