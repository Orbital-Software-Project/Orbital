#include "LayoutManager.h"

#include "Outliner.h"
#include "PropertyEditor.h"
#include "VideoPreview.h"
#include "MapViewer.h"
#include "Sequencer.h"
#include "NodeEditor/NodeEditor.h"
#include "TaskPanel.h"

#include "Editor/Global.h"

#include <imgui.h>

namespace Orb {

	LayoutManager::LayoutManager(std::shared_ptr<Shader> shader) {
		this->AddView(std::make_shared<Outliner>(Global::GetInstance().Renderer));

		this->AddView(std::make_shared<PropertyEditor>(Global::GetInstance().Renderer));
		this->AddView(std::make_shared<TaskPanel>());

		this->AddView(std::make_shared<VideoPreview>());

		this->AddView(std::make_shared<MapViewer>(Global::GetInstance().Renderer, shader));
		this->AddView(std::make_shared<Sequencer>());
		this->AddView(std::make_shared<NodeEditor>(Global::GetInstance().Renderer));


	}

	LayoutManager::~LayoutManager() {

	}

	void LayoutManager::AddView(std::shared_ptr<IView> view) {
		this->viewCollection.push_back(view);
	}

	void LayoutManager::OnRender() {

		// Outliner
		static ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoTitleBar;

		int outlinerPosY  = ImGui::GetMainViewport()->WorkPos.y;
		int outlinerSizeY = ImGui::GetMainViewport()->WorkSize.y;
		static int outlinerSizeX = 300;
		ImGui::SetNextWindowPos(ImVec2(0, outlinerPosY));
		ImGui::SetNextWindowSize(ImVec2(outlinerSizeX, outlinerSizeY));
		if (ImGui::Begin("LeftPanel", &this->viewCollection[0]->Open, flags)) {
			if (ImGui::BeginTabBar("Tabs##1", ImGuiTabBarFlags_None)) {
				if (ImGui::BeginTabItem("Outliner"))
				{
					this->viewCollection[0]->OnRender();

					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			outlinerSizeX = ImGui::GetWindowSize().x;

			ImGui::End();
		}


		static int propertyEdSizeX = 300;
		int propertyEdPosX = ImGui::GetMainViewport()->Size.x - propertyEdSizeX;
		int propertyEdPosY = ImGui::GetMainViewport()->WorkPos.y;
		int propertyEdSizeY = ImGui::GetMainViewport()->WorkSize.y;
		
		ImGui::SetNextWindowPos(ImVec2(propertyEdPosX, propertyEdPosY));
		ImGui::SetNextWindowSize(ImVec2(propertyEdSizeX, propertyEdSizeY));
		if (ImGui::Begin("RightPanel", &this->viewCollection[1]->Open, flags)) {
			if (ImGui::BeginTabBar("Tabs##2", ImGuiTabBarFlags_None)) {
				if (ImGui::BeginTabItem("Property Editor"))
				{
					this->viewCollection[1]->OnRender();

					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Task panel"))
				{
					this->viewCollection[2]->OnRender();

					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}

			propertyEdSizeX = ImGui::GetWindowSize().x;

			ImGui::End();
		}


		static ImGuiWindowFlags flagsTopCentralView =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoTitleBar | 
			ImGuiWindowFlags_NoScrollWithMouse;

		int topCentralPosX  = outlinerSizeX;
		int topCentralPosY  = ImGui::GetMainViewport()->WorkPos.y;
		int topCentralSizeX = ImGui::GetMainViewport()->WorkSize.x - propertyEdSizeX - outlinerSizeX;
		static int topCentralSizeY = ImGui::GetMainViewport()->WorkSize.y - 500;

		ImGui::SetNextWindowPos(ImVec2(topCentralPosX, topCentralPosY));
		ImGui::SetNextWindowSize(ImVec2(topCentralSizeX, topCentralSizeY));
		if (ImGui::Begin("CentralTop", &this->viewCollection[3]->Open, flagsTopCentralView)) {

			if (ImGui::BeginTabBar("Tabs##3", ImGuiTabBarFlags_None)) {
				if (ImGui::BeginTabItem("Video Preview"))
				{
					this->viewCollection[3]->OnRender();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Map viewer"))
				{
					this->viewCollection[4]->OnRender();
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}

			topCentralSizeY = ImGui::GetWindowSize().y;

			ImGui::End();


		}



		
		int bottomCentralPosX = outlinerSizeX;
		int bottomCentralPosY = topCentralSizeY + 18;
		int bottomCentralSizeX = ImGui::GetMainViewport()->WorkSize.x - propertyEdSizeX - outlinerSizeX;
		int bottomCentralSizeY = ImGui::GetMainViewport()->WorkSize.y - topCentralSizeY;

		ImGui::SetNextWindowPos(ImVec2(bottomCentralPosX, bottomCentralPosY));
		ImGui::SetNextWindowSize(ImVec2(bottomCentralSizeX, bottomCentralSizeY));
		if (ImGui::Begin("CentralBottom", &this->viewCollection[5]->Open, flags)) {
			if (ImGui::BeginTabBar("Tabs##4", ImGuiTabBarFlags_None)) {
				if (ImGui::BeginTabItem("Sequencer"))
				{
					this->viewCollection[5]->OnRender();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Node Editor"))
				{
					this->viewCollection[6]->OnRender();
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}

			ImGui::End();
		}


	}

}