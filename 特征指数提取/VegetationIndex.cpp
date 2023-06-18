#include "VegetationIndex.h"

float* VegetationIndex::RVI(cv::Mat NIR, cv::Mat R, int rows, int cols)
{
	float* rvi = new float[rows * cols];
	float max = -1000; float min = 1000;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			rvi[i * cols + j] = NIR.at<uchar>(i, j) / (R.at<uchar>(i, j) + 0.0000001);
		}
	}
	return rvi;
}

float* VegetationIndex::NDVI(cv::Mat NIR, cv::Mat R, int rows, int cols)
{
	float* ndvi = new float[rows * cols];
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			ndvi[i * cols + j] = (NIR.at<uchar>(i, j) - R.at<uchar>(i, j)) / (NIR.at<uchar>(i, j) + R.at<uchar>(i, j) + 0.0000001);
		}
	}
	return ndvi;
}

float* VegetationIndex::SAVI(cv::Mat NIR, cv::Mat R, int rows, int cols)
{
	float* savi = new float[rows * cols];
	float L = 0.5;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			savi[i * cols + j] = (NIR.at<uchar>(i, j) - R.at<uchar>(i, j)) / (NIR.at<uchar>(i, j) + R.at<uchar>(i, j) + L + 0.00000001) * (1 + L);
		}
	}
	return savi;
}
