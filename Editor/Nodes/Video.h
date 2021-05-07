#pragma once

#include "INode.hpp"

#include <string>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

class Video : public INode {

public:
	Video();

	~Video();

	void OnRender();

	bool OnRenderProperties();

	bool ComputeFrame(int frameIdx);

private:
	std::shared_ptr<NodeSocket> video_output;
	cv::VideoCapture video;
	cv::Mat curr_frame;

	std::string filePath = "";
	int frameCount = 0;
	int currFrame;
	int width = 0;
	int height = 0;


};