#pragma once

//ゲージクラス

//ゲージは段々と減っていく（）
//四角形描画を使用する
class C_Gauge
{
public:
	
	//コンストラクタ
	C_Gauge() {}

	//デストラクタ
	~C_Gauge() {}

	//初期化
	void Init(	const Math::Vector2& pos,			//中心座標
					const Math::Vector2& radius,		//半径
					const float* pPointMax,				//最大値のアドレス
					const float* pPoint,					//現在値のアドレス
					const float speed = 1.0f);			//ゲージが増減するスピード
	

	//更新
	void Update();
	//描画
	void Draw();

	void SetPos(Math::Vector2& pos) { m_pos = pos; }
	void SetRadius(Math::Vector2& radius) { m_radius = radius; }

	Math::Vector2& GetPos() { return m_pos; }
	Math::Vector2& GetRadius() { return m_radius; }

private:

	Math::Vector2 m_pos;			//中心座標
	Math::Vector2 m_radius;		//半径
	const float* m_pPointMax;				//最大値	のアドレス
	const float* m_pPoint;					//現在の値のアドレス
	float m_formerPoint;				//前の値（現在と前の値が違うとき段々とゲージが増減していく処理に必要）（前の値を段々と今の値に変更していく）
	float m_speed;						//ゲージが増減するスピード（m_speed( 1 )のとき、毎フレーム１ずつ増減していく）
};


