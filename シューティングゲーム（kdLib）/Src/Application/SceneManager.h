#pragma once
#include"SceneBase.h"

class C_SceneManager
{
public:

	//シーン切り替え
	void ChangeState(C_SceneBase* set)
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
			if (m_currentState) delete m_currentState;
			m_currentState = m_pendingState;
			m_pendingState = nullptr;
		}
	}

	//描画
	void Draw() {
		if (m_currentState)
		{
			m_currentState->Draw();
		}
	}

private:

	C_SceneBase* m_currentState = nullptr;			//現在のシーン
	C_SceneBase* m_pendingState = nullptr;			//次のシーンを格納する用

	//シングルトン
private:
	C_SceneManager() {}
public:
	static C_SceneManager& GetInstance()
	{
		static C_SceneManager instance;
		return instance;
	}
};

#define SCENE_MANAGER C_SceneManager::GetInstance()
