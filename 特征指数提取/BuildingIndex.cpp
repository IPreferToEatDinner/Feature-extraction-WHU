#include "BuildingIndex.h"

float* BuildingIndex::DBI(cv::Mat NIR, cv::Mat SWIR2, int rows, int cols)
{
	float* dbi = new float[rows * cols];
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			dbi[i * cols + j] = SWIR2.at<uchar>(i, j) - NIR.at<uchar>(i, j);
		}
	}
	return dbi;
}

float* BuildingIndex::NDBI(cv::Mat SWIR1, cv::Mat NIR, int rows, int cols)
{
	float* ndbi = new float[rows * cols];
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			ndbi[i * cols + j] = (SWIR1.at<uchar>(i, j) - NIR.at<uchar>(i, j)) / (SWIR1.at<uchar>(i, j) + NIR.at<uchar>(i, j) + 0.00000001);
		}
	}
	return ndbi;
}

float* BuildingIndex::NDBBI(cv::Mat G, cv::Mat NIR, cv::Mat SWIR2, int rows, int cols)
{
	float* ndbbi = new float[rows * cols];
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			ndbbi[i * cols + j] = (1.5 * SWIR2.at<uchar>(i, j) - 0.5 * (NIR.at<uchar>(i, j) + G.at<uchar>(i, j))) / (1.5 * SWIR2.at<uchar>(i, j) + 0.5 * (NIR.at<uchar>(i, j) + G.at<uchar>(i, j)) + 0.0000001);
		}
	}
	return ndbbi;
}
