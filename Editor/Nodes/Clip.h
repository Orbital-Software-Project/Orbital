#pragma once

#include "INode.hpp"

#include <string>
#include <GL/glew.h>
#include <opencv2/core.hpp>



class Clip : public INode {

public:
	Clip();

	~Clip();

	void OnRender();

	bool OnRenderProperties();

	bool ComputeFrame(int frameIdx);

	cv::Mat GetMat() { return this->image; }

	int GetFrameCount() { return this->frame_count; }

public:
	bool AddToSequencer = false;

private:
	std::shared_ptr<NodeSocket> video_input_id;
	cv::Mat image;
	cv::Mat outChessboard;
	int frame_count = 0;

};