#pragma once

#include <opencv2/core/core.hpp>

class VegetationIndex {
public:
	static float* RVI(cv::Mat NIR, cv::Mat R, int rows, int cols);
	static float* NDVI(cv::Mat NIR, cv::Mat R, int rows, int cols);
	static float* SAVI(cv::Mat NIR, cv::Mat R, int rows, int cols);

	VegetationIndex() = default;
};