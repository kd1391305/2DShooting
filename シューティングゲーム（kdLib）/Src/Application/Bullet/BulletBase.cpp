#include"BulletBase.h"

void BulletBase::Update(float deltaTime)
{
	//座標更新
	m_pos += m_move * deltaTime;

	//画面外に出たら非活性状態にする
	if (m_pos.x < SCREEN_LEFT || m_pos.x > SCREEN_RIGHT ||
		m_pos.y > SCREEN_TOP || m_pos.y < SCREEN_BOTTOM)
	{
		m_bActive = false;
		return;
	}

	//行列作成
	Math::Matrix scaleMat = Math::Matrix::CreateScale(1, 1, 0);
	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	m_mat = scaleMat * transMat;
}

