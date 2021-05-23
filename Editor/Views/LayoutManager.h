#pragma once

#include "IView.hpp"

#include <vector>
#include <memory>

namespace Orb {

	// TODO: rename class to ViewManager
	// Hardcoded default layout of the Views
	class LayoutManager {

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
		LayoutManager();

		~LayoutManager();

	public:
		void AddView(std::shared_ptr<IView> view, DockType dockType);

		void OnRender();

		bool HasRequest(Request& r);

	public:
		
		int Dx = 0;

		int Dy = 0;

	private:
		void loadImGuiTheme();

	private:
		std::vector<ViewDockTypePair> viewCollection;
		Request req = Request::None;
	};
}