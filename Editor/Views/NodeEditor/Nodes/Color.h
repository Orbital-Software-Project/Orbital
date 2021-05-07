#pragma once

#include "INode.hpp"

#include <string>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

class Color : public INode {

public:
	Color();

	~Color();

	void OnRender();

	bool OnRenderProperties();

	bool ComputeFrame(int frameIdx);



private:
	std::shared_ptr<NodeSocket> ns_input;
	std::shared_ptr<NodeSocket> ns_output;

	int slider_R = 255;
	int slider_G = 255;
	int slider_B = 255;



};