#pragma once

class C_KeyManager
{
public:

	//更新
	void Update()
	{
		for (auto& k : m_keys)
		{
			//キーが押されているかを判断する
			if (GetAsyncKeyState(k.first) & 0x8000)
			{
				//キーが押された瞬間
				if (k.second == KeyState::None)
					k.second = KeyState::Down | KeyState::Held;
				//キーを押し続けている
				else if (k.second & KeyState::Down)
					k.second = KeyState::Held;
			}
			else
			{
				//キーを離した瞬間
				if (k.second & KeyState::Held)
					k.second = KeyState::Up;
				//キーを離した次のフレーム（キーが押されていない状態にする）
				else if (k.second & KeyState::Up)
					k.second = KeyState::None;
			}
		}
	}

	//キーを押したか？
	bool IsDown(char key)
	{
		if (m_keys.find(key)->second & KeyState::Down)
			return true;
		return false;
	}

	//キーを押し続けているか？（押した瞬間も含まれる）
	bool IsHeld(char key)
	{
		if (m_keys.find(key)->second & KeyState::Held)
			return true;
		return false;
	}
	
	//キーを離したか？
	bool IsUp(char key)
	{
		if (m_keys.find(key)->second & KeyState::Up)
			return true;
		return false;
	}

private:
	enum KeyState
	{
		None		= 0,				//何も押されていない
		Down	= 1 << 0,		//キーを押した瞬間
		Held		= 1 << 1,		//キーを押している間
		Up		= 1 << 2,		//キーを離した瞬間
	};

	std::map<char, INT8> m_keys;
	
	//キーを登録する
	void RegisterKeys()
	{
		//登録するキー一覧
		char registerKeys[] = {
			VK_LEFT,VK_RIGHT,VK_UP,VK_DOWN,
			'W','A','S','D',
			VK_SPACE,
			VK_LBUTTON,
			VK_RETURN
		};
		//キーを登録する
		for (int i = 0; i < strlen(registerKeys); i++)
		{
			m_keys.insert(std::make_pair(registerKeys[i], KeyState::None));
		}
	}

	//シングルトン
private:
	C_KeyManager()
	{
		RegisterKeys();
	}
public:
	static C_KeyManager& GetInstance()
	{
		static C_KeyManager instance;
		return instance;
	}
};

#define KEY C_KeyManager::GetInstance()