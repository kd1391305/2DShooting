#pragma once

static const int SCREEN_LEFT = -640;
static const int SCREEN_RIGHT =640;
static const int SCREEN_TOP = 360;
static const int SCREEN_BOTTOM = -360;
static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 720;

//============================================================
// アプリケーションクラス
//	APP.～ でどこからでもアクセス可能
//============================================================
class Application
{
// メンバ
public:

	// アプリケーション実行
	void Execute();

	// アプリケーション終了
	void End() { m_endFlag = true; }


	//=====================================================
	// グローバルデータ
	//=====================================================

	// ゲームウィンドウクラス
	KdGameWindow		m_window;

	// FPS制御
	int				m_fps = 0;						// 現在のFPS値
	int				m_maxFps = 60;				// 最大FPS
	double	GetDeltaTime() { return m_deltaTime /1000; }	

private:

	double			m_deltaTime = 0;					//経過時間（単位は秒）

	// アプリケーション初期化
	bool Init(int w, int h);

	// アプリケーション解放
	void Release();

	// ゲーム終了フラグ trueで終了する
	bool		m_endFlag = false;

//=====================================================
// シングルトンパターン
//=====================================================
private:
	// 
	Application() {}

public:
	static Application &GetInstance(){
		static Application Instance;
		return Instance;
	}
};

// 簡単にアクセスできるようにするためのマクロ
#define APP Application::GetInstance()
