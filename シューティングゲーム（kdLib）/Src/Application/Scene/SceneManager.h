#pragma once

class BaseScene;

class SceneManager
{
public:

	//シーン切り替え
	void ChangeState(std::shared_ptr<BaseScene> set)
	{
		m_pendingState = set;
	}

	//更新
	void Update();

	//描画
	void Draw();

	std::shared_ptr<BaseScene> GetCurrentState() { return m_currentState; }

private:

	std::shared_ptr<BaseScene> m_currentState = nullptr;			//現在のシーン
	std::shared_ptr<BaseScene> m_pendingState = nullptr;			//次のシーンを格納する用

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