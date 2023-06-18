#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class BuildingIndex {
public:
	static float* DBI(cv::Mat NIR, cv::Mat SWIR2, int rows, int cols);
	static float* NDBI(cv::Mat SWIR1, cv::Mat NIR, int rows, int cols);
	static float* NDBBI(cv::Mat G, cv::Mat NIR, cv::Mat SWIR2, int rows, int cols);
};