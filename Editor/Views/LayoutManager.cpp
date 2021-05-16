#include "LayoutManager.h"

#include <imgui.h>
#include <string>

namespace Orb {

	LayoutManager::LayoutManager() {

	}

	LayoutManager::~LayoutManager() {

	}

	void LayoutManager::AddView(std::shared_ptr<IView> view, DockType dockType) {
		this->viewCollection.push_back(
			ViewDockTypePair {dockType, view}
		);
	}

	void LayoutManager::OnRender() {


		for (auto viewDock : this->viewCollection) {
			if (viewDock.Type == DockType::Dock_Float) {
				viewDock.View->OnRender();
			}
		}

		// ---------------------------

		int tabID = 1;

		// ---------------------------

		int leftViewPosY = ImGui::GetMainViewport()->WorkPos.y + 1;
		int leftViewSizeY = ImGui::GetMainViewport()->WorkSize.y;
		static int leftViewSizeX = 300;

		static ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoSavedSettings;

		ImGui::SetNextWindowPos(ImVec2(0, leftViewPosY));
		ImGui::SetNextWindowSize(ImVec2(leftViewSizeX, leftViewSizeY));
		bool leftPanelOpen = true;
		if (ImGui::Begin("LeftPanel", &leftPanelOpen, flags)) {
			if (ImGui::BeginTabBar("LeftTabBar", ImGuiTabBarFlags_None)) {
				for (auto viewDock : this->viewCollection) {
					if (viewDock.Type == DockType::Dock_Left) {
						if (ImGui::BeginTabItem((viewDock.View->Name + "##" + std::to_string(tabID)).c_str())) {
							viewDock.View->OnRender();
							ImGui::EndTabItem();
						}
						leftViewSizeX = ImGui::GetWindowSize().x;
						tabID++;
					}
				}
				ImGui::EndTabBar();
			}
			ImGui::End();
		}

		// ---------------------------

		static int rightViewSizeX = 300 - 1;
		int rightViewPosX  = ImGui::GetMainViewport()->Size.x - rightViewSizeX + 1;
		int rightViewPosY  = ImGui::GetMainViewport()->WorkPos.y + 1;
		int rightViewSizeY = ImGui::GetMainViewport()->WorkSize.y;

		ImGui::SetNextWindowPos(ImVec2(rightViewPosX, rightViewPosY));
		ImGui::SetNextWindowSize(ImVec2(rightViewSizeX, rightViewSizeY));
		bool rightPanelOpen = true;
		if (ImGui::Begin("RightPanel", &rightPanelOpen, flags)) {
			if (ImGui::BeginTabBar("RightTabBar", ImGuiTabBarFlags_None)) {
				for (auto viewDock : this->viewCollection) {
					if (viewDock.Type == DockType::Dock_Right) {
						if (ImGui::BeginTabItem((viewDock.View->Name + "##" + std::to_string(tabID)).c_str())) {
							viewDock.View->OnRender();
							ImGui::EndTabItem();
						}
						rightViewSizeX = ImGui::GetWindowSize().x;
						tabID++;
					}
				}
				ImGui::EndTabBar();
			}
			ImGui::End();
		}

		// ---------------------------

		static ImGuiWindowFlags flagsTopCentralView =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoScrollWithMouse | 
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoBringToFrontOnFocus;

		int topCentralPosX  = leftViewSizeX + 1;
		int topCentralPosY  = ImGui::GetMainViewport()->WorkPos.y + 1;
		int topCentralSizeX = ImGui::GetMainViewport()->WorkSize.x - rightViewSizeX - leftViewSizeX - 1;
		static int topCentralSizeY = ImGui::GetMainViewport()->WorkSize.y - 500;

		ImGui::SetNextWindowSizeConstraints(ImVec2(-1, 0), ImVec2(-1, FLT_MAX));      // Vertical only
		ImGui::SetNextWindowPos(ImVec2(topCentralPosX, topCentralPosY));
		ImGui::SetNextWindowSize(ImVec2(topCentralSizeX, topCentralSizeY));
		bool centralTopPanelOpen = true;
		if (ImGui::Begin("CentralTopPanel", &centralTopPanelOpen, flagsTopCentralView)) {
			if (ImGui::BeginTabBar("CentralTopTabBar", ImGuiTabBarFlags_None)) {
				for (auto viewDock : this->viewCollection) {
					if (viewDock.Type == DockType::Dock_Central_Top) {
						if (ImGui::BeginTabItem((viewDock.View->Name + "##" + std::to_string(tabID)).c_str())) {
							viewDock.View->OnRender();
							ImGui::EndTabItem();
						}
						topCentralSizeY = ImGui::GetWindowSize().y;
						tabID++;
					}
				}
				ImGui::EndTabBar();
			}
			ImGui::End();
		}

		// ---------------------------

		static ImGuiWindowFlags flagsBottomCentralView =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoScrollWithMouse |
			ImGuiWindowFlags_NoDecoration | 
			ImGuiWindowFlags_NoSavedSettings;

		int bottomCentralPosX  = leftViewSizeX + 1;
		int bottomCentralPosY  = topCentralSizeY + 21;
		int bottomCentralSizeX = ImGui::GetMainViewport()->WorkSize.x - rightViewSizeX - leftViewSizeX - 1;
		int bottomCentralSizeY = ImGui::GetMainViewport()->WorkSize.y - topCentralSizeY - 2;

		
		ImGui::SetNextWindowPos(ImVec2(bottomCentralPosX, bottomCentralPosY));
		ImGui::SetNextWindowSize(ImVec2(bottomCentralSizeX, bottomCentralSizeY));
		bool centralBottomPanelOpen = true;
		if (ImGui::Begin("CentralBottomPanel", &centralBottomPanelOpen, flagsBottomCentralView)) {
			if (ImGui::BeginTabBar("CentralBottomTabBar", ImGuiTabBarFlags_None)) {
				for (auto viewDock : this->viewCollection) {
					if (viewDock.Type == DockType::Dock_Central_Bottom) {
						if (ImGui::BeginTabItem((viewDock.View->Name + "##" + std::to_string(tabID)).c_str())) {
							viewDock.View->OnRender();
							ImGui::EndTabItem();
						}
						topCentralSizeX = ImGui::GetWindowSize().x;
						tabID++;
					}
				}
				ImGui::EndTabBar();
			}
			ImGui::End();
		}

		return;
	}

}