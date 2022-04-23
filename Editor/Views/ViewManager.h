#pragma once

#include "IView.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <vector>
#include <memory>

namespace Orb {



	//int PanelSeparator::separatorStackID = 0;

	// Hardcoded default layout of the Views
	class ViewManager {

	public:
		enum class Request {
			None,
			Move_Window,
			Close_Window,
			Minimize_Window,
			Maximize_Window
		};

		enum class DockType {
			Dock_Float,
			Dock_Left,
			Dock_Right,
			Dock_Central_Top,
			Dock_Central_Bottom,
		};

		struct ViewDockTypePair {
			DockType Type;

			std::shared_ptr<IView> View;
		};

	public:
		ViewManager();

		~ViewManager();

	public:
		void AddView(std::shared_ptr<IView> view, DockType dockType);

		void OnRender();

		bool HasRequest(Request& r);

	public:
		float Dx = 0;

		float Dy = 0;

	private:
		void loadImGuiTheme();

		void drawWindowCtrlButton();

		void handlePanelResizing();

		void drawFloatingPanel();

		void drawPanel(std::string name, ImVec2 pos, ImVec2 size, DockType dock_type);

		void drawRectCurrWND();

		void separatorLogic();

		void handleWindowMovement();

		void handleDockSpace();

	private:
		std::vector<ViewDockTypePair> viewCollection;
		Request req = Request::None;

	private:
		int tabID = 1;
		ImGuiWindowFlags viewFlags;

		ImRect leftPanel;
		ImRect rightPanel;
		ImRect topPanel;
		ImRect bottomPanel;

		ImVec2 separator_left_panel = ImVec2(300.0f, 500.0f);

		ImVec2 separator_right_panel = ImVec2(1920.0f - 300.0f, 500.0f);



	};
}