#pragma once

#include "IView.hpp"
#include "Engine/Shader.h"

#include <vector>
#include <memory>

namespace Orb {

	// Hardcoded default layout of the Views
	class LayoutManager {

	public:
		enum class DockType {
			Dock_Left,
			Dock_Right,
			Dock_Central_Top,
			Dock_Central_Bottom,
		};

	public:
		LayoutManager(std::shared_ptr<Shader> shader);

		~LayoutManager();

	public:
		void AddView(std::shared_ptr<IView> view);

		void OnRender();

	private:
		std::vector<std::shared_ptr<IView>> viewCollection;

	};
}