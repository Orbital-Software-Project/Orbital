#pragma once

#include "IView.hpp"
#include "Engine/Shader.h"

#include <vector>
#include <memory>

namespace Orb {

	class LayoutManager {

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