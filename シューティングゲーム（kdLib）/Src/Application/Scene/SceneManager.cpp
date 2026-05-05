#include"SceneManager.h"
#include"BaseScene/BaseScene.h"

void SceneManager::Update()
{
	//次のシーンが決まっていたら次のシーンに切り替える
	if (m_pendingState)
	{
		//解放＆削除
		if (m_currentState)
		{
			m_currentState->Release();
		}
		m_currentState = m_pendingState;
		m_pendingState = nullptr;

		//初期化
		m_currentState->Init();
	}

	//現在のシーンの更新
	if (m_currentState)m_currentState->Update();
}

void SceneManager::Draw()
{
	if (m_currentState)
	{
		m_currentState->Draw();
	}
}