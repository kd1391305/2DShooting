#pragma once
#include"../BaseBullet/BaseBullet.h"

class PlayerBullet :public BaseBullet
{
public:

	PlayerBullet() { 
		m_bActive = true; 
		m_pierceNum = 0;
	}
	~PlayerBullet()override{}

	//描画
	void Draw()override;

	//敵を貫く（敵を貫いたエフェクト＆パワー減少）＝ 実装予定
	//引数（貫いた場所）
	void Pierce();

	//現在のパワーを返す
	int GetPower() { return m_power; }

	//敵を貫通した数を返す
	int GetPierceNum() { return m_pierceNum; }
	
	void SetPower(float set) { m_power = set; }

	void SetPos(Math::Vector2 pos) { m_pos = pos; }
private:

	int m_power;			//チャージしたパワー（パワー１ = 敵を一体倒す）
	int m_pierceNum;		//敵を貫いた回数
};
