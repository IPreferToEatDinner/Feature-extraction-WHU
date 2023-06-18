#include <iostream>

#include "VegetationIndex.h"
#include "WaterIndex.h"
#include "BuildingIndex.h"
#include "ThresholdSegmentation.h"

#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>


int main() {
	/* 读取tif图片 */

	//蓝光波段
	cv::Mat B = cv::imread("../image/tm1.tif", -1);
	//绿光波段
	cv::Mat G = cv::imread("../image/tm2.tif", -1);
	//红光波段
	cv::Mat R = cv::imread("../image/tm3.tif", -1);
	//近红外波段
	cv::Mat NIR = cv::imread("../image/tm4.tif", -1);
	//中红外波段
	cv::Mat SWIR1 = cv::imread("../image/tm5.tif", -1);
	//热红外波段
	cv::Mat TIR = cv::imread("../image/tm6.tif", -1);
	//远红外波段
	cv::Mat SWIR2 = cv::imread("../image/tm7.tif", -1);

	/* 保存图像大小 */
	int rows = B.rows;
	int cols = B.cols;

	/* 退出指标 */
	bool run = true;

	while (run) {

		/* 设置选项位 */
		int outFlag = -1;
		int innerFlag = -1;

		/* 运行结果保存点 */
		float* result = new float[rows * cols];

		system("cls");
		std::cout << "请选择求解指数类型(直接输入阿拉伯数字)：①植被 ②水体 ③建筑 ④退出程序" << std::endl << ">> " << std::flush;
		std::cin >> outFlag;
		system("cls");

		if (outFlag == 1) {
			std::cout << "请选择要计算的植被指数：①比值植被指数RVI ②归一化植被指数NDVI ③土壤调节植被指数SAVI" << std::endl << ">> " << std::flush;
			std::cin >> innerFlag;
			VegetationIndex index;

			switch (innerFlag)
			{
			case 1:
				result = index.RVI(NIR, R, rows, cols);
				break;
			case 2:
				result = index.NDVI(NIR, R, rows, cols);
				break;
			case 3:
				result = index.SAVI(NIR, R, rows, cols);
				break;
			default:
				std::cout << "输入的序号无效，请直接输入阿拉伯数字" << std::endl;
				break;
			}
		}
		else if (outFlag == 2)
		{
			std::cout << "请选择要计算的水质指数：①归一化水体指数NDWI ②归一化差异水体指数MNDWI ③自动化水体提取指数AWEInsh" << std::endl << ">> " << std::flush;
			std::cin >> innerFlag;
			WaterIndex index;
			switch (innerFlag)
			{
			case 1:
				result = index.NDWI(G, NIR, rows, cols);
				break;
			case 2:
				result = index.MNDWI(G, SWIR1, rows, cols);
				break;
			case 3:
				result = index.AWEInsh(G, NIR, SWIR1, SWIR2, rows, cols);
				break;
			default:
				std::cout << "输入的序号无效，请直接输入阿拉伯数字" << std::endl;
				break;
			}
		}
		else if (outFlag == 3) {
			std::cout << "请选择要计算的建筑用地指数：①差值建筑覆盖指数DBI ②归一化差值建筑用地指数NDBI ③归一化差值裸地与建筑用地指数NDBBI" << std::endl << ">> " << std::flush;
			std::cin >> innerFlag;
			BuildingIndex index;
			switch (innerFlag)
			{
			case 1:
				result = index.DBI(NIR, SWIR2, rows, cols);
				break;
			case 2:
				result = index.NDBI(SWIR1, NIR, rows, cols);
				break;
			case 3:
				result = index.NDBBI(G, NIR, SWIR2, rows, cols);
				break;
			default:
				std::cout << "输入的序号无效，请直接输入阿拉伯数字" << std::endl;
				break;
			}
		}
		else if (outFlag == 4) { run = false; exit(0); }

		/* 灰度拉伸 */
		float max = -1000;
		float min = 1000;
		cv::Mat mat = B.clone();
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (result[i * cols + j] > max) { max = result[i * cols + j]; }
				if (result[i * cols + j] < min) { min = result[i * cols + j]; }
			}
		}

		for (int i = 0; i < cols * rows; i++) { result[i] = (result[i] - min) / (max - min) * 255; }

		/* 阈值分割 */
		int temp = 0;
		ThresholdSegmentation thresholder;
		temp = thresholder.iterativeThreshold(result, rows, cols);
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				mat.at<uchar>(i, j) = int(result[i * cols + j]);
			}
		}

		delete[] result;

		/* 图像展示 */
		cv::namedWindow("指数影像", CV_WINDOW_AUTOSIZE);
		imshow("指数影像", mat);


		/* 基于迭代求解的阈值进行初步分割 */
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (mat.at<uchar>(i, j) > temp) { mat.at <uchar>(i, j) = 255; }
				else { mat.at<uchar>(i, j) = 0; }
			}
		}


		std::cout << "该指数分割阈值为：" << temp << std::endl;

		cv::namedWindow("阈值分割后二值化影像", CV_WINDOW_AUTOSIZE);
		imshow("阈值分割后二值化影像", mat);
		cv::waitKey();
		exit;
	}

	return 1;
}

