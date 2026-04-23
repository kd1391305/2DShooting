#pragma once
#include"SceneBase.h"

class SceneManager
{
public:

	//シーン切り替え
	void ChangeState(SceneBase* set)
	{
		if (m_pendingState)delete m_pendingState;
		m_pendingState = set;
	}

	//更新
	void Update()
	{
		//現在のシーンの更新
		if (m_currentState)m_currentState->Update();
		
		//次のシーンが決まっていたら次のシーンに切り替える
		if (m_pendingState)
		{
			//解放＆削除
			if (m_currentState)
			{
				m_currentState->Release();
				delete m_currentState;
			}
			m_currentState = m_pendingState;
			m_pendingState = nullptr;

			//初期化
			m_currentState->Init();
		}
	}

	//描画
	void Draw()
	{
		if (m_currentState)
		{
			m_currentState->Draw();
		}
	}

private:

	SceneBase* m_currentState = nullptr;			//現在のシーン
	SceneBase* m_pendingState = nullptr;			//次のシーンを格納する用

	//シングルトン
private:
	SceneManager() {}
public:
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}
};