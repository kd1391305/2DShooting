#include"Explan.h"
#include"../../TextureCache/TextureCache.h"
#include"../../Font/DrawString.h"
void Explan::Init(Math::Vector2* pPlayerPos)
{
	m_pPlayerPos = pPlayerPos;

	m_timer = 0;
	m_endTime = 6.0f;

	

	m_deltaAlpha = 3;

	m_color = { 0.9f,0.9f,0.9f,0.8f };


	fontSize = 20;

	{
		std::shared_ptr<KdTexture>tex = TextureCache::Instance().Get("Texture/UI/Back.png");
		m_moveExplan.m_playerOffset = { -150,100 };
		m_moveExplan.m_pos = *m_pPlayerPos + m_moveExplan.m_playerOffset;
		m_chargeExplan.m_backPos =m_moveExplan.m_pos;
		m_chargeExplan.m_backPos.y -= fontSize;

		m_moveExplan.m_radius = { 70,30 };
		m_moveExplan.m_backRadius = m_moveExplan.m_radius;
		m_moveExplan.m_backRadius.y += fontSize;

		m_moveExplan.m_scale.x = m_moveExplan.m_radius.x  / tex->GetRadius().x;
		m_moveExplan.m_scale.y = m_moveExplan.m_radius.y / tex->GetRadius().y;
		m_moveExplan.m_backScale.x = m_moveExplan.m_backRadius.x / tex->GetRadius().x;
		m_moveExplan.m_backScale.y = m_moveExplan.m_backRadius.y / tex->GetRadius().y;

		m_chargeExplan.m_playerOffset = { -150,-100 };
		m_chargeExplan.m_pos = *m_pPlayerPos + m_chargeExplan.m_playerOffset;
		m_chargeExplan.m_backPos = m_chargeExplan.m_pos;
		m_chargeExplan.m_backPos.y -= fontSize;

		m_chargeExplan.m_radius = { 70,30 };
		m_chargeExplan.m_backRadius = m_chargeExplan.m_radius;
		m_chargeExplan.m_backRadius.y += fontSize;
		m_chargeExplan.m_scale.x = m_chargeExplan.m_radius.x / tex->GetRadius().x;
		m_chargeExplan.m_scale.y = m_chargeExplan.m_radius.y / tex->GetRadius().y;
		m_chargeExplan.m_backScale.x = m_chargeExplan.m_backRadius.x / tex->GetRadius().x;
		m_chargeExplan.m_backScale.y = m_chargeExplan.m_backRadius.y / tex->GetRadius().y;
	}

	//行列作成
	{
		Math::Matrix scaleMat, transMat;

		scaleMat = Math::Matrix::CreateScale(m_moveExplan.m_scale.x, m_moveExplan.m_scale.y, 0);
		transMat = Math::Matrix::CreateTranslation(m_moveExplan.m_pos.x, m_moveExplan.m_pos.y, 0);
		m_moveExplan.m_mat = scaleMat * transMat;
		scaleMat = Math::Matrix::CreateScale(m_moveExplan.m_backScale.x, m_moveExplan.m_backScale.y, 0);
		transMat = Math::Matrix::CreateTranslation(m_moveExplan.m_backPos.x, m_moveExplan.m_backPos.y, 0);
		m_moveExplan.m_backMat = scaleMat * transMat;

		scaleMat = Math::Matrix::CreateScale(m_chargeExplan.m_scale.x, m_chargeExplan.m_scale.y, 0);
		transMat = Math::Matrix::CreateTranslation(m_chargeExplan.m_pos.x, m_chargeExplan.m_pos.y, 0);
		m_chargeExplan.m_mat = scaleMat * transMat;
		scaleMat = Math::Matrix::CreateScale(m_chargeExplan.m_backScale.x, m_chargeExplan.m_backScale.y, 0);
		transMat = Math::Matrix::CreateTranslation(m_chargeExplan.m_backPos.x, m_chargeExplan.m_backPos.y, 0);
		m_chargeExplan.m_backMat = scaleMat * transMat;
	}
	
}

void Explan::Update(float deltaTime)
{
	m_timer += deltaTime;
	if (m_timer >= m_endTime)
	{
		m_color.A(m_color.A() - m_deltaAlpha * deltaTime);
	}

	m_moveExplan.m_pos = *m_pPlayerPos + m_moveExplan.m_playerOffset;
	m_moveExplan.m_backPos = m_moveExplan.m_pos;
	m_moveExplan.m_backPos.y -= fontSize;

	m_chargeExplan.m_pos = *m_pPlayerPos + m_chargeExplan.m_playerOffset;
	m_chargeExplan.m_backPos = m_chargeExplan.m_pos;
	m_chargeExplan.m_backPos.y -= fontSize;

	Math::Matrix scaleMat, transMat;

	scaleMat = Math::Matrix::CreateScale(m_moveExplan.m_scale.x, m_moveExplan.m_scale.y, 0);
	transMat = Math::Matrix::CreateTranslation(m_moveExplan.m_pos.x, m_moveExplan.m_pos.y, 0);
	m_moveExplan.m_mat = scaleMat * transMat;
	scaleMat = Math::Matrix::CreateScale(m_moveExplan.m_backScale.x, m_moveExplan.m_backScale.y, 0);
	transMat = Math::Matrix::CreateTranslation(m_moveExplan.m_backPos.x, m_moveExplan.m_backPos.y, 0);
	m_moveExplan.m_backMat = scaleMat * transMat;

	scaleMat = Math::Matrix::CreateScale(m_chargeExplan.m_scale.x, m_chargeExplan.m_scale.y, 0);
	transMat = Math::Matrix::CreateTranslation(m_chargeExplan.m_pos.x, m_chargeExplan.m_pos.y, 0);
	m_chargeExplan.m_mat = scaleMat * transMat;
	scaleMat = Math::Matrix::CreateScale(m_chargeExplan.m_backScale.x, m_chargeExplan.m_backScale.y, 0);
	transMat = Math::Matrix::CreateTranslation(m_chargeExplan.m_backPos.x, m_chargeExplan.m_backPos.y, 0);
	m_chargeExplan.m_backMat = scaleMat * transMat;
}

void Explan::Draw()
{
	std::shared_ptr<KdTexture> tex = TextureCache::Instance().Get("Texture/UI/Back.png");

	SHADER.m_spriteShader.SetMatrix(m_moveExplan.m_backMat);
	SHADER.m_spriteShader.DrawTex_Src(tex, m_color);
	SHADER.m_spriteShader.SetMatrix(m_moveExplan.m_mat);
	SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/UI/Key.png"), m_color);
	Math::Vector2 pos;
	pos.x = m_moveExplan.m_pos.x;
	pos.y = m_moveExplan.m_pos.y - m_moveExplan.m_radius.y;
	DWriteCustom::Instance().Draw("移動", pos, fontSize);



	SHADER.m_spriteShader.SetMatrix(m_chargeExplan.m_backMat);
	SHADER.m_spriteShader.DrawTex_Src(tex, m_color);
	SHADER.m_spriteShader.SetMatrix(m_chargeExplan.m_mat);
	SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get("Texture/UI/Mouse.png"), m_color);

	pos;
	pos.x = m_chargeExplan.m_pos.x;
	pos.y = m_chargeExplan.m_pos.y - m_chargeExplan.m_radius.y;
	DWriteCustom::Instance().Draw("長押し : チャージ", pos, fontSize);
}