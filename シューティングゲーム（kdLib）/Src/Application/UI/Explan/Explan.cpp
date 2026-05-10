#include"Explan.h"
#include"../../TextureCache/TextureCache.h"
#include"../../Font/DWriteCustom.h"
void Explan::Init()
{
	m_timer = 0;
	m_endTime = 8.0f;

	m_color = { 0.9f,0.9f,0.9f,0.7f };
	m_fontColor = { 1,1,1,0.7f };
}

bool Explan::Update(float deltaTime)
{
	m_timer += deltaTime;
	if (m_timer >= m_endTime)
	{
		//透明度を減らす
		m_color.A(m_color.A() - 2 * deltaTime);
		m_fontColor.A(m_fontColor.A() - 2 * deltaTime);
		if (m_color.A() <= 0.05f)return false;
	}

	return true;
}

void Explan::Draw()
{
	Math::Matrix scaleMat, transMat;
	
	int fontSize = 25;
	//背景の描画
	{
		std::shared_ptr<KdTexture> backTex = TextureCache::Instance().Get("Texture/UI/ExplanBack.png");

		Math::Vector2 backScale = { 1.0f,0.95f };
		//移動キーの背景
		scaleMat = Math::Matrix::CreateScale(backScale.x, backScale.y, 0);
		transMat = Math::Matrix::CreateTranslation(-500, -240, 0);
		SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
		SHADER.m_spriteShader.DrawTex_Src(backTex, m_color);

		//チャージキー背景
		transMat = Math::Matrix::CreateTranslation(-290, -240, 0);
		SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
		SHADER.m_spriteShader.DrawTex_Src(backTex, m_color);
	}

	//キーの描画
	{
		//移動キー
		scaleMat = Math::Matrix::CreateScale(0.21f, 0.21f, 1);
		transMat = Math::Matrix::CreateTranslation(-500, -220, 0);
		SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
		SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/UI/TransKey.png"), m_color);

		DWriteCustom::Instance().Draw("移動", { -525, -270 }, fontSize, m_fontColor);

		//チャージキー
		scaleMat = Math::Matrix::CreateScale(0.22f, 0.22f, 0);
		transMat = Math::Matrix::CreateTranslation(-290, -220, 0);
		SHADER.m_spriteShader.SetMatrix(scaleMat * transMat);
		SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/UI/ZKey.png"), m_color);
		
		DWriteCustom::Instance().Draw("チャージ", { -338,-270 }, fontSize, m_fontColor);
	}
}