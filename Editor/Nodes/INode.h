#pragma once

namespace Orb {

	// Node system
	// Multioutput is supported but not multiinput

	class INode {

	public:
		INode() {};

		virtual ~INode() {}

		virtual void OnRender(int &id) = 0;

		//virtual void OnRenderProperties() = 0;

	};
}
