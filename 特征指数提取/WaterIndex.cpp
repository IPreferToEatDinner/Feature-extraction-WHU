#include "WaterIndex.h"

float* WaterIndex::NDWI(cv::Mat G, cv::Mat NIR, int rows, int cols)
{
	float* ndwi = new float[cols * rows];
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			ndwi[i * cols + j] = (G.at<uchar>(i, j) - NIR.at<uchar>(i, j)) / (G.at<uchar>(i, j) + NIR.at<uchar>(i, j) + 0.0000001);
		}
	}
	return ndwi;
}

float* WaterIndex::MNDWI(cv::Mat G, cv::Mat SWIR1, int rows, int cols)
{
	float* mndwi = new float[rows * cols];
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			mndwi[i * cols + j] = (G.at<uchar>(i, j) - SWIR1.at<uchar>(i, j)) / (G.at<uchar>(i, j) + SWIR1.at<uchar>(i, j) + 0.0000001);
		}
	}
	return mndwi;
}

float* WaterIndex::AWEInsh(cv::Mat G, cv::Mat NIR, cv::Mat SWIR1, cv::Mat SWIR2, int rows, int cols)
{
	float* awein = new float[rows * cols];
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			awein[i * cols + j] = 4 * (G.at<uchar>(i, j) - SWIR1.at<uchar>(i, j)) / (0.25 * NIR.at<uchar>(i, j) + 2.75 * SWIR2.at<uchar>(i, j) + 0.0000001);
		}
	}
	return awein;
}


