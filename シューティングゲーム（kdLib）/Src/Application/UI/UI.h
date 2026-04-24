#pragma once

class Player;
class Score;
class Gauge;

class UI
{
public:

	//初期化
	void Init(Player* player);

	//更新
	void Update();

	//描画
	void Draw();

	std::shared_ptr<Score>& GetScoreInst() { return m_score; }

private:

	std::shared_ptr<Score> m_score;

	std::shared_ptr<Gauge> m_gauge;

	Math::Vector2* m_pPlayerPos;		//プレイヤーの座標アドレス

	
};
