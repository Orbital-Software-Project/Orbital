#pragma once

#include "IView.hpp"




#include <vector>
#include <memory>

namespace Orb {
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

	private:
		std::vector<ViewDockTypePair> viewCollection;
		Request req = Request::None;

	};
}