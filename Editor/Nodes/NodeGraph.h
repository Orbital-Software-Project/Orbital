#pragma once

#include "INode.hpp"
#include "clip.h"

#include <vector>

// TODO: implement topological sorting 
// https://www.geeksforgeeks.org/topological-sorting/

class NodeGraph {

public:
	NodeGraph();

	void OnRender();

	void AddNode(INode* node);

	void SetFrameIdx(int frameIdx);

	std::vector<INode*> GetBeginNodes();

	std::vector<INode*> GetParentNodes(INode* node);

	void BuildNodePyramid(std::vector<std::vector<INode*>>& nodes);

	INode* GetNodeBySocket(int socket_id);

	INode* GetNode(int node_id);

	void ComputeNodes();

	void ComputeClip(Clip* node, int frameIdx = -1);

	void ResetNodes();

	template<typename T>
	std::vector<T> GetNodesByType(INodeType type) {
		std::vector<T> res;
		for (INode* nd : this->nodes) {
			if (nd->Type == type) {
				res.push_back(static_cast<T>(nd));
			}
		}
		return res;
	}

private:
	std::vector<std::pair<int, int>> links;
	std::vector<INode*> nodes;
	int frameIdx = 0;
	bool doCompute = true;

	std::vector<std::vector<INode*>> pyramid;
	std::vector<INode*> nodes_for_inspector;

};