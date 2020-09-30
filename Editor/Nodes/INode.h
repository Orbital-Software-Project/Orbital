#pragma once

namespace Orb {

	class INode {

	public:
		INode() {};

		virtual ~INode() {}

		virtual void OnRender(int &id) = 0;
	};
}
