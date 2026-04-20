#pragma once

class C_Sky
{
public:

	C_Sky();

	//XV
	void Update();

	//•`‰æ
	void Draw();

private:

	KdTexture m_tex;			//‰æ‘œ

	//‰æ‘œ‚ğ‚»‚Ì‚Ü‚Ü‚ÆA‚»‚Ì‰¡‚É”½“]‚µ‚½‰æ‘œ‚ğ•`‰æ‚·‚é
	static const int s_drawNum = 2;					//•`‰æ‰ñ”
	Math::Vector2 m_pos[s_drawNum];			//À•W
	Math::Matrix m_mat[s_drawNum];			//s—ñ
};
