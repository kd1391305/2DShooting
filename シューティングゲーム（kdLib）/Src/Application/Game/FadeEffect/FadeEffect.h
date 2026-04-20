#pragma once

class C_FadeEffect
{
public:

	C_FadeEffect();

	void Update();
	void Draw();

	void Emit(Math::Vector2 pos, float radius);

	static void LoadTex() { s_tex.Load("Texture/Free Smoke Fx Pixel 2/Free Smoke Fx  Pixel 06.png"); }
	static void ReleaseTex() { s_tex.Release(); }

	bool IsActive() { return m_bActive; }

private:

	static KdTexture s_tex;		//画像
	static const int s_texDiameter = 64;		//画像にある 1コマの直径

	Math::Vector2 m_pos;		//座標
	Math::Matrix m_mat;			//行列
	float m_scale;					//拡縮
	float m_animCnt;				//アニメーションカウント
	static constexpr float s_animCntSpeed = 0.5f;			//アニメーションのスピード（0.2f = 1コマあたり 5frame）
	static constexpr float s_animCntEnd = 11.0f;			//アニメーションの終わり（コマのアニメーション）

	bool m_bActive;
};
