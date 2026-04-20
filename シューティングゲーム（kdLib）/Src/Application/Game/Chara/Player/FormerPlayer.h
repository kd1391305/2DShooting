#pragma once
#include"../FormerCharaBase.h"

//セーブした内容の通りに動くだけのプレイヤークラス
class C_FormerPlayer:public C_FormerCharaBase
{
public:
	void Update()override;

	void Draw(KdTexture* tex)override;

private:

};
