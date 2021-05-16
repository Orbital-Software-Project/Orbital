#pragma once

#include "IView.hpp"

#include <vector>
#include <memory>

namespace Orb {

	// Hardcoded default layout of the Views
	class LayoutManager {

	public:
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

	private:
		std::vector<ViewDockTypePair> viewCollection;

	};
}