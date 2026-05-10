#pragma once
class AttackArea;

class AttackAreaManager
{
public:

	void Update(float deltaTime);

	void Draw();

	std::vector<std::shared_ptr<AttackArea>>& GetList() { return m_list; }

	std::shared_ptr<AttackArea> Emit(Math::Vector2& pos, Math::Vector2& radius);

private:
	std::vector<std::shared_ptr<AttackArea>>m_list;
};
