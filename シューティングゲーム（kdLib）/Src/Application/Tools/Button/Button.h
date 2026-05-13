#pragma once

class Button
{
public:

	enum ActionType
	{
		Key,
		Mouse,
	};

	Button() :
		m_pos(0, 0), m_radius(10, 10), m_color(1, 1, 1, 1), m_bSelect(false), m_selectScale(1.2f,1.2f)
	{
		m_scale = { 1, 1 };
		m_mat = Math::Matrix::CreateTranslation(0, 0, 0);
	}

	Button(Math::Vector2 pos,Math::Vector2 radius,Math::Color color,Math::Color selectingColor) :
		m_pos(pos), m_radius(radius), m_color(color),m_selectColor(selectingColor), m_bSelect(false),m_selectScale(1.2f,1.2f)
	{
		m_scale = { 1, 1 };
		m_mat = Math::Matrix::CreateTranslation(0, 0, 0);
	}

	virtual ~Button() {}

	//更新
	virtual void Update();

	//描画
	virtual void Draw();

	//今選択されているか？
	bool IsSelect() { return m_bSelect; }

	void SetPos(Math::Vector2 set) { m_pos = set; }
	void SetRadius(Math::Vector2 set) { m_radius = set; }
	void SetColor(Math::Color set) { m_color = set; }
	void SetScale(Math::Vector2 set) { m_scale = set; }
	void SetSelectColor(Math::Color set) { m_selectColor = set; }
	void SetSelectScale(Math::Vector2 set) { m_selectScale = set; }
	void SetSelect(bool set);

	Math::Vector2 GetPos() { return m_pos; }
	Math::Vector2 GetRadius() { return m_radius; }
	Math::Vector2 GetSelectScale() { return m_selectScale; }

protected:
	
	Math::Vector2 m_pos;					//座標
	Math::Vector2 m_radius;					//半径
	Math::Color m_color;					//色（通常）
	Math::Color m_selectColor;				//選択時の色
	Math::Vector2 m_scale;					//通常時の拡縮
	Math::Vector2 m_selectScale;			//選択時の拡縮
	Math::Matrix m_mat;						//行列
	bool m_bSelect = false;					//選択中フラグ

	
};

class ButtonEx :public Button
{
public:

	void SetTex(std::string path);

	void Draw()override;

private:

	std::string m_texPath;
};