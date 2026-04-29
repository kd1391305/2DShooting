#include"ChargeAnim.h"
#include"../../TextureCache/TextureCache.h"
#include"../../Tools/RandEx/RandEx.h"
#include"../../Tools/Collision/Collision.h"

void ChargeParticle::Update(float deltaTime)
{
	m_pos += m_move * deltaTime;
	Math::Matrix scale = Math::Matrix::CreateScale(m_scale.x, m_scale.y, 0);
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scale * trans;
}

void ChargeParticle::Draw(std::string path)
{
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex_Src(TextureCache::Instance().Get(path), m_color);
}

void ChargeAnim::Init()
{

}

void ChargeAnim::Update(float deltaTime)
{
	Math::Matrix scaleMat, transMat;
	std::shared_ptr<KdTexture> tex = TextureCache::Instance().Get("Texture/Particle.png");
	float texWidth = tex->GetInfo().Width;
	float texHeight = tex->GetInfo().Height;
	float radian = DirectX::XMConvertToRadians(randRange(0, 360));
	m_particleList.push_back(std::make_shared<ChargeParticle>());
	m_particleList.back()->m_move.x = -cosf(radian) * m_moveSpeed;
	m_particleList.back()->m_move.y = -sinf(radian) * m_moveSpeed;
	m_particleList.back()->m_pos.x = m_centerPos.x + cosf(radian) * randRange(m_radius.x - 10, m_radius.y);
	m_particleList.back()->m_pos.y = m_centerPos.y + sinf(radian) * randRange(m_radius.x - 10, m_radius.y);
	m_particleList.back()->m_life = randRange(0.3f, 0.6f);
	m_particleList.back()->m_color = { 1,1,1,1 };
	float r = randRange(3, 6);
	m_particleList.back()->m_radius = { r,r };
	m_particleList.back()->m_scale.x = m_particleList.back()->m_radius.x / texWidth;
	m_particleList.back()->m_scale.y = m_particleList.back()->m_radius.y / texHeight;

	scaleMat = Math::Matrix::CreateScale(m_particleList.back()->m_scale.x, m_particleList.back()->m_scale.y, 0);
	scaleMat = Math::Matrix::CreateTranslation(m_particleList.back()->m_pos.x, m_particleList.back()->m_pos.y, 0);
	m_particleList.back()->m_mat = scaleMat * transMat;

	for (int i = m_particleList.size() - 1; i >= 0; i--)
	{
		m_particleList[i]->Update(deltaTime);
		if (IsCollision(m_particleList[i]->m_pos, 5, m_centerPos, 5) ||
			m_particleList[i]->m_life <= 0)
		{
			m_particleList.erase(m_particleList.begin() + i);
		}
	}
}

void ChargeAnim::Draw()
{
	D3D.SetBlendState(BlendMode::Add);
	char path[50];
	sprintf_s(path, sizeof(path), "Texture/Particle.png");
	for (auto& p : m_particleList)
	{
		p->Draw(path);
	}
	D3D.SetBlendState(BlendMode::Alpha);
}

void ChargeAnim::Start(Math::Vector2 pos, Math::Vector2 radius,float moveSpeedMin,float moveSpeedMax)
{
	m_centerPos = pos;
	m_radius = radius;
	m_moveSpeedMin = moveSpeedMin;
	m_moveSpeed = m_moveSpeedMin;
	m_moveSpeedMax = moveSpeedMax;

	std::shared_ptr<KdTexture> tex = TextureCache::Instance().Get("Texture/Particle.png");
	float texWidth = tex->GetInfo().Width;
	float texHeight = tex->GetInfo().Height;
	Math::Matrix scaleMat, transMat;

	for (int i = 0; i < 30; i++)
	{
		float radian = DirectX::XMConvertToRadians(randRange(0, 360));

		m_particleList.push_back(std::make_shared<ChargeParticle>());
		m_particleList[i]->m_move.x = -cosf(radian) * m_moveSpeed;
		m_particleList[i]->m_move.y = -sinf(radian) * m_moveSpeed;
		m_particleList[i]->m_pos.x = m_centerPos.x + cosf(radian) * randRange(m_radius.x - 10, m_radius.y);
		m_particleList[i]->m_pos.y = m_centerPos.y + sinf(radian) * randRange(m_radius.x - 10, m_radius.y);
		m_particleList[i]->m_life = randRange(0.3f, 0.6f);
		m_particleList[i]->m_color = { 1,1,1,1 };
		float r = randRange(3, 6);
		m_particleList[i]->m_radius = { r,r };
		m_particleList[i]->m_scale.x = m_particleList[i]->m_radius.x / texWidth;
		m_particleList[i]->m_scale.y = m_particleList[i]->m_radius.y / texHeight;

		scaleMat = Math::Matrix::CreateScale(m_particleList[i]->m_scale.x, m_particleList[i]->m_scale.y, 0);
		scaleMat = Math::Matrix::CreateTranslation(m_particleList[i]->m_pos.x, m_particleList[i]->m_pos.y, 0);
		m_particleList[i]->m_mat = scaleMat * transMat;
	}
}

void ChargeAnim::StartChargeMaxAnim()
{

}

bool ChargeAnim::IsEnd()
{
	return m_particleList.empty();
}

void ChargeAnim::Release()
{

}
