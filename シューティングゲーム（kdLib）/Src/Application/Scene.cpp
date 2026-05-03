#include "main.h"
#include "Scene.h"
#include"Scene/SceneManager.h"
#include"Scene/TitleScene/TitleScene.h"
#include"Font/DWriteCustom.h"
#include"Mouse/Mouse.h"
#include"Key/KeyManager.h"
#include"Timer/Timer.h"

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
	SceneManager::Instance().ChangeState(new TitleScene());
	Timer::Instance().Reset();
}

void Scene::Release()
{
	
}

void Scene::ImGuiUpdate()
{
	return;

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
