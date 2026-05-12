#pragma once

class Explan
{
public:

	void Init();
	bool Update(float deltaTime);
	void Draw();

	bool IsEnd()
	{
		//完全に消えて、終了時間を過ぎていたら
		return m_color.A() <= 0 && m_timer >= m_endTime;
	}

	void End() 
	{
		m_timer = m_endTime;
		m_color.A(0);
	}

private:

	float m_timer = 0;					//経過時間を測る

	float m_endTime;				//終了時間を測る

	Math::Color m_color;			//UIの色
	Math::Color m_fontColor;			//フォントの色
};
