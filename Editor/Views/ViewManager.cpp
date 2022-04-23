#include "ViewManager.h"

#include <string>
#include <iostream>

namespace Orb {

	ViewManager::ViewManager() {
		this->loadImGuiTheme();

		this->viewFlags =
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoScrollWithMouse |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus;

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

		//this->handleDockSpace();

#ifndef NDEBUG
		//ImGui::ShowDemoWindow();
#endif
		
		bool leftPanelOpen = true;

		auto main_dock_id = ImGui::DockSpaceOverViewport();


		for (auto view : this->viewCollection) {

			if (view.View->Type == IView::ViewType::ViewType_Window) {
				if (ImGui::Begin(view.View->Name.c_str(), &leftPanelOpen, viewFlags)) {
					view.View->OnRender();

				}
				ImGui::End();
			}

			if (view.View->Type == IView::ViewType::ViewType_Toolbar) {
				view.View->OnRender();
			}
		}

		static ImGuiID id_split_1, id_split_2, id_split_3, id_split_4, id_split_5, id_split_6;

		static bool init_layout = true;
		if (init_layout) {
			init_layout = false;

			
			auto dockspace_id = ImGui::DockBuilderAddNode(main_dock_id);


			ImGui::DockBuilderSplitNode(main_dock_id, ImGuiDir_Right, 0.5, &id_split_1, &id_split_2);
			ImGui::DockBuilderSplitNode(id_split_2, ImGuiDir_Right, 0.5, &id_split_3, &id_split_4);
			ImGui::DockBuilderSplitNode(id_split_3, ImGuiDir_Down, 0.5, &id_split_5, &id_split_6);


			
			ImGui::DockBuilderDockWindow("Outliner", id_split_4);

			ImGui::DockBuilderDockWindow("Video preview", id_split_6);
			ImGui::DockBuilderDockWindow("Map viewer", id_split_6);
			ImGui::DockBuilderDockWindow("Sequencer", id_split_5);
			ImGui::DockBuilderDockWindow("Property editor", id_split_1);
			ImGui::DockBuilderDockWindow("Task panel", id_split_1);


		}

		ImGui::DockBuilderFinish(main_dock_id);


		// Window dragging and double click behavior
		this->handleWindowMovement();

		this->drawWindowCtrlButton();

		

		return;


		
		
		// Window close/maximize/minimize button
		
		
		this->tabID = 1;

		// Separator and resizing logic for the panels
		this->handlePanelResizing();
		
		this->drawFloatingPanel();


		this->drawPanel("LeftPanel", this->leftPanel.Min, this->leftPanel.GetSize(), DockType::Dock_Left);
		this->drawPanel("RightPanel", this->rightPanel.Min, this->rightPanel.GetSize(), DockType::Dock_Right);
		this->drawPanel("CentralTopPanel", this->topPanel.Min, this->topPanel.GetSize(), DockType::Dock_Central_Top);
		this->drawPanel("CentralBottomPanel", this->bottomPanel.Min, this->bottomPanel.GetSize(), DockType::Dock_Central_Bottom);




	}

	void ViewManager::handlePanelResizing() {

		ImVec2 host_wnd_size = ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);

		this->separatorLogic();

		this->leftPanel = ImRect(
			0.0f,
			ImGui::GetMainViewport()->WorkPos.y + 1, // Because of the toolbar (21px offset)
			this->separator_left_panel.x,
			host_wnd_size.y
		);

		this->rightPanel = ImRect(
			this->separator_right_panel.x,
			ImGui::GetMainViewport()->WorkPos.y + 1, 
			host_wnd_size.x,                         
			host_wnd_size.y                          
		);

		this->topPanel = ImRect(
			this->leftPanel.GetTR().x + 1.0f,
			this->leftPanel.GetTR().y,
			this->rightPanel.GetTL().x - 1.0f,
			this->separator_left_panel.y                                  
		);

		this->bottomPanel = ImRect(
			this->topPanel.GetBL().x,
			this->topPanel.GetBL().y + 1.0f,
			this->topPanel.GetBR().x,
			this->rightPanel.GetBL().y
		);
	}

	void ViewManager::drawPanel(std::string name, ImVec2 pos, ImVec2 size, DockType dock_type) {

		ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
		ImGui::SetNextWindowSize(size, ImGuiCond_Always);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		bool leftPanelOpen = true;
		if (ImGui::Begin(name.c_str(), &leftPanelOpen, viewFlags)) {
			//this->drawRectCurrWND();
			if (ImGui::BeginTabBar(("TabBar_" + name).c_str(), ImGuiTabBarFlags_None)) {
				for (auto viewDock : this->viewCollection) {
					if (viewDock.Type == dock_type) {
						if (ImGui::BeginTabItem((viewDock.View->Name + "##" + std::to_string(this->tabID)).c_str())) {
							viewDock.View->OnRender();
							ImGui::EndTabItem();
						}
						this->tabID++;
					}
				}
				ImGui::EndTabBar();
			}
			ImGui::End();
		}

		ImGui::PopStyleVar();
	}

	void ViewManager::drawFloatingPanel() {
		for (auto viewDock : this->viewCollection) {
			if (viewDock.Type == DockType::Dock_Float) {
				viewDock.View->OnRender();
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

	void ViewManager::drawRectCurrWND() {
		//return;

		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();

		vMin.x += ImGui::GetWindowPos().x;
		vMin.y += ImGui::GetWindowPos().y;
		vMax.x += ImGui::GetWindowPos().x;
		vMax.y += ImGui::GetWindowPos().y;

		ImGui::GetWindowDrawList()->AddRect(vMin, vMax, IM_COL32(255, 0, 0, 255));
	}

	void ViewManager::separatorLogic() {

		ImGui::GetForegroundDrawList()->AddCircleFilled(this->separator_left_panel, 3.0f, IM_COL32(0, 0, 0, 255));
		ImGui::GetForegroundDrawList()->AddCircleFilled(this->separator_right_panel,3.0f, IM_COL32(0, 0, 0, 255));

		auto mpos = ImGui::GetMousePos();

		static bool drag_switch = false;
		static bool is_left = false;
		static bool is_right = false;
		
		if (!drag_switch) {

			if (mpos.x >= this->separator_left_panel.x - 3 && mpos.x <= this->separator_left_panel.x + 3) {
				if (mpos.y >= this->separator_left_panel.y - 3 && mpos.y <= this->separator_left_panel.y + 3) {

					ImGui::GetForegroundDrawList()->AddCircleFilled(this->separator_left_panel, 3.0f, IM_COL32(255, 255, 255, 255));

					if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
						drag_switch = true;
						is_left = true;
					}
				}
			}

			if (mpos.x >= this->separator_right_panel.x - 3 && mpos.x <= this->separator_right_panel.x + 3) {
				if (mpos.y >= this->separator_right_panel.y - 3 && mpos.y <= this->separator_right_panel.y + 3) {

					ImGui::GetForegroundDrawList()->AddCircleFilled(this->separator_right_panel, 3.0f, IM_COL32(255, 255, 255, 255));

					if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
						drag_switch = true; 
						is_right = true;
					}
				}
			}
		}
		else 
		{
			drag_switch = ImGui::IsMouseDown(ImGuiMouseButton_Left);

			if (!drag_switch) {
				is_left = false;
				is_right = false;
			}
			
		}

		if (drag_switch) {
			if (is_left) {
				this->separator_left_panel = mpos;
				this->separator_right_panel.y = mpos.y;
			}
			if (is_right) {
				this->separator_right_panel = mpos;
				this->separator_left_panel.y = mpos.y;
			}

			

		}




	}

	void ViewManager::handleWindowMovement() {

		auto mp = ImGui::GetMousePos();
		static bool dragging = false;
		if (dragging || (mp.x < ImGui::GetMainViewport()->WorkSize.x - 60 && mp.y <= 20)) {
			// Mouse cursor inside main menu bar
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				this->req = Request::Maximize_Window;
				return;
			}
			if (!dragging && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
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

	void ViewManager::handleDockSpace() {

		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		static bool open = true;

		ImGui::Begin("DockSpace", &open, window_flags);

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("DockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		
		ImGui::End();


	}
}