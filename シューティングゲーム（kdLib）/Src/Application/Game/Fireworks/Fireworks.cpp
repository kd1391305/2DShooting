#include"Fireworks.h"
#include"../../Toolkit/RandEx.h"
#include"../../main.h"

void C_Fireworks::Update()
{
	if (!m_bExploded)
	{
		//座標更新
		m_pos += m_move * APP.GetDeltaTime();
		//重力
		m_move.y -= 0.001f * APP.GetDeltaTime();

		if (m_move.y <= 0)
		{
			//爆発する（弾ける）
			m_bExploded = true;
			for (auto& p : m_circleList)
			{
				Math::Vector2 move = { randRange(0.0f,150.0f),randRange(0.0f,150) };
				if (rand() % 2)
					move.x *= -1;
				if (rand() % 2)
					move.y *= -1;
				p.m_move = move;
				p.m_color = m_color;
				p.m_life = randRange(1.5f, 2.5f);
			}
			for (auto& p : m_lineList)
			{
				Math::Vector2 move = { randRange(0.0f,150.0f),randRange(0.0f,150.0f) };
				if (rand() % 2)
					move.x *= -1;
				if (rand() % 2)
					move.y *= -1;
				p.m_move = move;
				p.m_radian = atan2f(p.m_move.y, p.m_move.x);
				p.m_color = m_color;
				p.m_life = randRange(1.5f, 2.5f);
			}
		}
	}

	//花火の火の更新
	for (int i = m_circleList.size() - 1; i >= 0; i--)
	{
		if (m_bExploded)
			m_circleList[i].m_move -= m_lineList[i].m_move * 0.98f * APP.GetDeltaTime();
		if(!m_circleList[i].Update(APP.GetDeltaTime()))
			//寿命が尽きたら削除
			m_circleList.erase(m_circleList.begin() + i);
	}

	for (int i = m_lineList.size() - 1; i >= 0; i--)
	{
		if (m_bExploded)
			m_lineList[i].m_move -= m_lineList[i].m_move * 0.95f *APP.GetDeltaTime();
		if (!m_lineList[i].Update(APP.GetDeltaTime()))
			//寿命が尽きたら削除
			m_lineList.erase(m_lineList.begin() + i);
	}

	//火が全てなくなったら非活性状態にする
	if (m_circleList.empty() && m_lineList.empty())
		m_bActive;
}

//花火を打ち上げる
void C_Fireworks::Shot(Math::Vector2 pos)
{
	//花火の中心（軸）の初期化
	//この値を元に、花火の火の部分を作成する
	m_pos = pos;
	m_move = { 0,randRange(5.0f,6.0f) };
	m_color = { randRange(0, 0.6f), randRange(0, 0.6f), randRange(0, 0.6f), randRange(0.4f, 0.6f) };
	m_bExploded = false;
	m_bActive = true;

	//花火の火の部分を作成
	//円
	for (int i = 0; i < 50; i++)
	{
		//座標を中心（軸）から少しずらす
		Math::Vector2 pos = {
			m_pos.x + randRange(-0.5f,0.5f),
			m_pos.y + randRange(-0.5f,0.5f)
		};
		//移動量を中心（軸）と比べて少しだけ変化させる
		Math::Vector2 move = {
			m_move.x + randRange(-0.5f,0.5f),
			m_move.x + randRange(-0.5f,0.5f)
		};
		//色も少し変える
		float r = m_color.R() + randRange(-0.1f, 0.1f);
		float g = m_color.G() + randRange(-0.1f, 0.1f);
		float b = m_color.B() + randRange(-0.1f, 0.1f);
		float a = m_color.A() + randRange(-0.1f, 0.1f);
		m_circleList.emplace_back(pos, move, Math::Color{ r,g,b,a }, 1000);
	}

	//棒
	for (int i = 0; i < 150; i++)
	{
		Math::Vector2 pos = {
			m_pos.x + randRange(-0.5f,0.5f),
			m_pos.y + randRange(-0.5f,0.5f)
		};
		Math::Vector2 move = {
			m_move.x + randRange(-0.05f,0.05f),
			m_move.y + randRange(-0.05f,0.05f)
		};
		float r = m_color.R() + randRange(-0.1f, 0.1f);
		float g = m_color.G() + randRange(-0.1f, 0.1f);
		float b = m_color.B() + randRange(-0.1f, 0.1f);
		float a = m_color.A() + randRange(-0.1f, 0.1f);
		m_lineList.emplace_back(pos, move, Math::Color{ r,g,b,a }, 1000);
	}
}

//描画
void C_Fireworks::Draw(KdTexture* tex)
{
	for (auto& p : m_circleList)
	{
		SHADER.m_spriteShader.ClearMatrix();
		SHADER.m_spriteShader.DrawCircle(p.m_pos.x, p.m_pos.y, 5, &p.m_color, true);
		SHADER.m_spriteShader.DrawCircle(p.m_pos.x, p.m_pos.y, 6, &p.m_color, true);
	}
	for (auto& p : m_lineList)
	{
		SHADER.m_spriteShader.SetMatrix(p.m_mat);
		SHADER.m_spriteShader.DrawTex_Src(tex, p.m_color);
	}
}