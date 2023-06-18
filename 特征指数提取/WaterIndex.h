#pragma once

#ifndef WATERINDEX_H
#define WATERINDEX_H

#include <opencv2/core/core.hpp>

class WaterIndex {
public:
	static float* NDWI(cv::Mat G, cv::Mat NIR, int rows, int cols);
	static float* MNDWI(cv::Mat G, cv::Mat SWIR1, int rows, int cols);
	static float* AWEInsh(cv::Mat G, cv::Mat NIR, cv::Mat SWIR1, cv::Mat SWIR2, int rows, int cols);
};

#endif  // WATERINDEX_H