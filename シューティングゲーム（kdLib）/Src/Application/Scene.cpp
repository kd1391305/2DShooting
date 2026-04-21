#include "main.h"
#include "Scene.h"
#include"SceneManager.h"
#include"Title/Title.h"
#include"Toolkit/Font/DrawString.h"
#include"Mouse/Mouse.h"
#include"Toolkit/KeyManager.h"

void Scene::Draw2D()
{
	SCENE_MANAGER.Draw();
	MOUSE.Draw();
}

void Scene::Update()
{
	KEY.Update();
	MOUSE.Update();
	SCENE_MANAGER.Update();
}

void Scene::Init()
{
	srand(timeGetTime());
	C_DrawString::GetInstance().Init();
	SCENE_MANAGER.ChangeState(new C_Title());
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
