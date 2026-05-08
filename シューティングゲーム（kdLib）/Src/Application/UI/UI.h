#pragma once

class Player;
class Score;
class Gauge;
class Explan;

class UI
{
public:

	//‰Šú‰»
	void Init(Player*player);

	//XV
	void Update(float deltaTime);

	//•`‰æ
	void Draw();

	std::shared_ptr<Score>& GetScoreInst() { return m_score; }

	void CreateBossGauge(float* hpMax,float* hp);

private:

	std::shared_ptr<Score> m_score;

	std::shared_ptr<Gauge> m_playerGauge;

	std::shared_ptr<Gauge>m_bossGauge = nullptr;

	std::shared_ptr<Explan>m_explan = nullptr;

};
