#include "ViewManager.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <string>
#include <iostream>


namespace Orb {

	ViewManager::ViewManager() {
		this->loadImGuiTheme();
	}

	ViewManager::~ViewManager() {

	}

	void ViewManager::AddView(std::shared_ptr<IView> view, DockType dockType) {
		this->viewCollection.push_back(
			ViewDockTypePair {dockType, view}
		);
	}

	bool ViewManager::HasRequest(Request& r) {
		r = this->req;
		bool hasReq = this->req != Request::None;
		this->req = Request::None; // Reset request
		return hasReq;
	}

	void ViewManager::OnRender() {

		// Window dragging and double click behavior
		auto mp = ImGui::GetMousePos();

		{
			static bool dragging = false;
			if (dragging || (mp.x < ImGui::GetMainViewport()->WorkSize.x - 60 && mp.y <= 20)) {
				// Mouse cursor inside main menu bar
				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
					this->req = Request::Maximize_Window;
					return;
				}
				if (!dragging &&
					ImGui::IsMouseDown(ImGuiMouseButton_Left) && 
					ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
					this->Dx = ImGui::GetMousePos().x;
					this->Dy = ImGui::GetMousePos().y;
					dragging = true;
				}
				if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
					dragging = false;
				}
				if (dragging) {
					this->req = Request::Move_Window;
				}
			}
		}

		for (auto viewDock : this->viewCollection) {
			if (viewDock.Type == DockType::Dock_Float) {
				viewDock.View->OnRender();
			}
		}

		// Window close/maximize/minimize button
		this->drawWindowCtrlButton();
		
		// Window resize frame WIP
		{
			/*
			ImDrawList* draw_list_fg = ImGui::GetForegroundDrawList();
			auto workSize = ImGui::GetMainViewport()->Size;

			draw_list_fg->AddRect(
				ImVec2(0, 0),
				ImVec2(5, workSize.y),
				ImColor(ImVec4(1.0f, 1.0f, 1.0f, 0.0f)));

			draw_list_fg->AddRect(
				ImVec2(0, 0),
				ImVec2(workSize.x, 1),
				ImColor(ImVec4(1.0f, 1.0f, 1.0f, 0.0f)));

			draw_list_fg->AddRect(
				ImVec2(workSize.x, 0),
				ImVec2(workSize.x, 0),
				ImColor(ImVec4(1.0f, 1.0f, 1.0f, 0.0f)));

			draw_list_fg->AddRect(
				ImVec2(0, 0),
				ImVec2(1, workSize.y),
				ImColor(ImVec4(1.0f, 1.0f, 1.0f, 0.0f)));
				*/
		}

		// ---------------------------

		int tabID = 1;

		// ---------------------------

		/*
		+-----------> x
		|
		|
		|
		|
		v
		y
		*/

		float leftViewPosY = ImGui::GetMainViewport()->WorkPos.y + 1;
		float leftViewSizeY = ImGui::GetMainViewport()->WorkSize.y;
		static float leftViewSizeX = 300.0f;

		static ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoSavedSettings | 
			ImGuiWindowFlags_NoResize | 
			ImGuiWindowFlags_NoDecoration;

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


			// Resize between left and top/central panel
			{
				auto mp = ImGui::GetMousePos();

				ImVec2 tl = ImVec2(leftViewSizeX, leftViewPosY);
				ImVec2 br = ImVec2(leftViewSizeX + 1, leftViewSizeY);

				static bool bool_switch = false;

				if (tl.x <= mp.x && br.x >= mp.x &&
					tl.y <= mp.y && br.y >= mp.y &&
					ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
					bool_switch = true;
				}

				if (bool_switch && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
					leftViewSizeX = ImGui::GetMousePos().x;
				} else {
					bool_switch = false;
				}
			}


		}

		// ---------------------------

		static float rightViewSizeX = 300 - 1;
		float rightViewPosX  = ImGui::GetMainViewport()->Size.x - rightViewSizeX + 1;
		float rightViewPosY  = ImGui::GetMainViewport()->WorkPos.y + 1;
		float rightViewSizeY = ImGui::GetMainViewport()->WorkSize.y;

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

		// Resize between right and top/central panel
		{
			auto mp = ImGui::GetMousePos();

			ImVec2 tl = ImVec2(rightViewPosX - 1, rightViewPosY);
			ImVec2 br = ImVec2(rightViewPosX, rightViewSizeY);

			static bool bool_switch = false;

			if (tl.x <= mp.x && br.x >= mp.x &&
				tl.y <= mp.y && br.y >= mp.y &&
				ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
				bool_switch = true;
			}

			if (bool_switch && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
				rightViewSizeX = ImGui::GetMainViewport()->Size.x - ImGui::GetMousePos().x;
			}
			else {
				bool_switch = false;
			}
		}

		// ---------------------------

		static ImGuiWindowFlags flagsTopCentralView =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoScrollWithMouse | 
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoDecoration;

		float topCentralPosX  = leftViewSizeX + 1;
		float topCentralPosY  = ImGui::GetMainViewport()->WorkPos.y + 1;
		float topCentralSizeX = ImGui::GetMainViewport()->WorkSize.x - rightViewSizeX - leftViewSizeX - 1;
		static float topCentralSizeY = ImGui::GetMainViewport()->WorkSize.y - 500;

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
			ImGuiWindowFlags_NoResize |
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


		// Resize between top and central panel
		{
			auto mp = ImGui::GetMousePos();

			ImVec2 tl = ImVec2(bottomCentralPosX, bottomCentralPosY - 1);
			ImVec2 br = ImVec2(bottomCentralSizeX, bottomCentralPosY);

			static bool bool_switch = false;

			if (tl.x <= mp.x && br.x >= mp.x &&
				tl.y <= mp.y && br.y >= mp.y &&
				ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
				bool_switch = true;
			}

			if (bool_switch && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
				topCentralSizeY = ImGui::GetMousePos().y - 21;
			}
			else {
				bool_switch = false;
			}
		}


	}



	void ViewManager::loadImGuiTheme() {

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowBorderSize = 0.0f;
		style.WindowPadding = ImVec2(1, 0);
		style.FrameRounding = 4.0f;
		style.ScrollbarSize = 15.0f;
		style.GrabMinSize = 15.0f;

		ImVec4* colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(69.0f / 255.0f, 69.0f / 255.0f, 69.0f / 255.0f, 1.00f);
		colors[ImGuiCol_Border] = ImVec4(69.0f / 255.0f, 69.0f / 255.0f, 69.0f / 255.0f, 1.00f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(97.0f / 255.0f, 97.0f / 255.0f, 97.0f / 255.0f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(97.0f / 255.0f, 97.0f / 255.0f, 97.0f / 255.0f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(97.0f / 255.0f, 97.0f / 255.0f, 97.0f / 255.0f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.38f, 0.38f, 0.38f, 0.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 1.00f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);



	}

	void ViewManager::drawWindowCtrlButton() {

		auto mp = ImGui::GetMousePos();

		ImDrawList* draw_list_fg = ImGui::GetForegroundDrawList();

		auto workSize = ImGui::GetMainViewport()->WorkSize;

		draw_list_fg->AddCircleFilled(ImVec2(workSize.x - 55, 10), 5.0f, ImColor(ImVec4(1.0f, 1.0f, 0.0f, 1.0f)));
		draw_list_fg->AddCircleFilled(ImVec2(workSize.x - 35, 10), 5.0f, ImColor(ImVec4(0.0f, 1.0f, 0.0f, 1.0f)));
		draw_list_fg->AddCircleFilled(ImVec2(workSize.x - 15, 10), 5.0f, ImColor(ImVec4(1.0f, 0.0f, 0.0f, 1.0f)));


		if (mp.x >= workSize.x - 60 &&
			mp.y >= 0 &&
			mp.x <= workSize.x &&
			mp.y <= 20
			) {
			draw_list_fg->AddCircleFilled(ImVec2(workSize.x - 55, 10), 2.0f, ImColor(ImVec4(0.75f, 0.75f, 0.0f, 1.0f)));
			draw_list_fg->AddCircleFilled(ImVec2(workSize.x - 35, 10), 2.0f, ImColor(ImVec4(0.0f, 0.75f, 0.0f, 1.0f)));
			draw_list_fg->AddCircleFilled(ImVec2(workSize.x - 15, 10), 2.0f, ImColor(ImVec4(0.75f, 0.0f, 0.0f, 1.0f)));
		}

		if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {

			// Minimize
			if (mp.x >= workSize.x - 60 &&
				mp.y >= 0 &&
				mp.x <= workSize.x - 46 &&
				mp.y <= 20) {

				this->req = Request::Minimize_Window;
			}

			// Maximize
			if (mp.x >= workSize.x - 45 &&
				mp.y >= 0 &&
				mp.x <= workSize.x - 26 &&
				mp.y <= 20) {

				this->req = Request::Maximize_Window;
			}

			// Close
			if (mp.x >= workSize.x - 25 &&
				mp.y >= 0 &&
				mp.x <= workSize.x &&
				mp.y <= 20) {

				this->req = Request::Close_Window;
			}
		}
	}


}