#pragma once
#include"../BaseScene/BaseScene.h"

class Back;
class ButtonEx;

class TitleScene :public BaseScene
{
public:
	TitleScene() {}
	TitleScene(std::shared_ptr<Back> back);
	~TitleScene() override { Release(); }

	void Init();
	void Update();
	void Draw();

private:

	void Release();

	std::shared_ptr<ButtonEx> m_start = nullptr;

	std::shared_ptr<Back> m_back = nullptr;

	//タイトル名の画像
	Math::Vector2 m_namePos;
	Math::Matrix m_nameMat;

	//花火を撃つクールタイム
	float m_shotCoolTimer;

	//カーソルが当たっているか？
	bool m_bHitCursor;

	//シーン切り替え用
	const float m_changeSceneWait = 1.0f;
	float m_changeSceneWaitTimer;
	bool m_bChangeScene;

	//エンターキー表示用の変数（選択中はエンターキーを）
	Math::Matrix m_enterKeyMat;
	float m_enterKeyAlpha;
	float m_enterKeyDeltaAlpha;
};
