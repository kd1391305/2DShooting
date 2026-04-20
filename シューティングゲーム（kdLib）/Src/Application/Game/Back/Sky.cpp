#include"Sky.h"
#include"../../main.h"

//コンストラクタ
C_Sky::C_Sky()
{
	//画像の読み込み
	m_tex.Load("Texture/Sky.jpg");

	//データの初期化
	for (int i = 0; i < s_drawNum; i++)
	{
		m_pos[i] = { 0 + (float)m_tex.GetInfo().Width * i,0 };									//一つ目は中心スタート、二つ目はその右隣からスタート
		Math::Matrix scaleMat = Math::Matrix::CreateScale(1 - (i % 2) * 2, 1, 0);		//偶数はそのまま、奇数は反転させて描画する
		Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos[i].x, m_pos[i].y, 0);
		m_mat[i] = scaleMat * transMat;
	}
}

//更新
void C_Sky::Update()
{
	for (int i = 0; i < s_drawNum; i++)
	{
		//左にスクロールする
		m_pos[i].x -= 10.0f;
		
		//画面端に到達したら、画面右端にセットする
		float radiusX = m_tex.GetInfo().Width / 2.0f;		//画像の半径
		float right = m_pos[i].x + radiusX;						//画像の右端
		if (right < SCREEN_LEFT)m_pos[i].x += radiusX * 2 * 2;		

		//行列作成
		Math::Matrix scaleMat, transMat;
		scaleMat = Math::Matrix::CreateScale(1 - (i % 2) * 2, 1, 0);					//偶数なら左右反転、奇数ならそのまま
		transMat = Math::Matrix::CreateTranslation(m_pos[i].x, m_pos[i].y, 0);
		m_mat[i] = scaleMat * transMat;
	}
}

//描画
void C_Sky::Draw()
{
	for (int i = 0; i < 2; i++)
	{
		SHADER.m_spriteShader.SetMatrix(m_mat[i]);
		//SHADER.m_spriteShader.DrawTex_Src(&m_tex, Math::Color{ 0.95f,0.95f,0.95f,1.0f });
	}
}


