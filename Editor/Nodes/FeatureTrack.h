#pragma once

#include "INode.hpp"

#include <string>
#include <GL/glew.h>

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>



class FeatureTrack : public INode {

public:
	FeatureTrack();

	~FeatureTrack();

	void OnRender();

	bool ComputeFrame(int frameIdx);

	bool OnRenderProperties();

private:
	void doTrack();

private:
	std::shared_ptr<NodeSocket> video_input = nullptr;
	std::shared_ptr<SocketData> video_data = nullptr;

	std::shared_ptr<NodeSocket> track_data_output = nullptr;

	cv::Ptr<cv::AKAZE> akaze;
	cv::Ptr<cv::Feature2D> detector;
	cv::Ptr<cv::DescriptorMatcher> matcher;

	const double akaze_thresh        = 3e-4; // AKAZE detection threshold set to locate about 1000 keypoints
	const double ransac_thresh       = 2.5f; // RANSAC inlier threshold
	const double nn_match_ratio      = 0.8f; // Nearest-neighbour matching ratio
	const int    bb_min_inliers      = 100;  // Minimal number of inliers to draw bounding box
	const int    stats_update_period = 10;   // On-screen statistics are updated every 10 frames


};