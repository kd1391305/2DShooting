#include "main.h"
#include "Scene.h"
#include"Scene/SceneManager.h"
#include"Scene/TitleScene/TitleScene.h"
#include"Font/DWriteCustom.h"
#include"Mouse/Mouse.h"
#include"Key/KeyManager.h"
#include"Timer/Timer.h"
#include"Background/Back.h"
#include"Scene/GameClearScene/GameClearScene.h"
#include"Scene/GameOverScene/GameOverScene.h"
#include"Scene/GameScene/GameScene.h"

void Scene::Draw2D()
{
	SceneManager::Instance().Draw();
	
	MOUSE.Draw();
}

void Scene::Update()
{
	Timer::Instance().Update();
	KEY.Update();
	MOUSE.Update();
	SceneManager::Instance().Update();
}

void Scene::Init()
{
	srand(timeGetTime());
	DWriteCustom::Instance().Init();

	/*std::shared_ptr<Back>back = std::make_shared<Back>();
	back->Init();
	SceneManager::Instance().ChangeState(std::make_shared< GameClearScene>(back));*/

	/*std::shared_ptr<Back>back = std::make_shared<Back>();
	back->Init();
	std::shared_ptr<Game> game = std::make_shared<Game>(back);
	SceneManager::Instance().ChangeState(std::make_shared< GameOverScene>(game));*/

	SceneManager::Instance().ChangeState(std::make_shared<TitleScene>());
	Timer::Instance().Reset();
}

void Scene::Release()
{
	
}

void Scene::ImGuiUpdate()
{
	//return;

	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_Once);

	// デバッグウィンドウ
	if (ImGui::Begin("Debug Window"))
	{
		ImGui::Text("FPS : %d", APP.m_fps);
		ImGui::Text("MAX_FPS : %d", APP.m_maxFps);
		ImGui::Text("MouseX : %d\nMuseY : %d", MOUSE.GetPos().x, MOUSE.GetPos().y);
		ImGui::End();
	}
}
