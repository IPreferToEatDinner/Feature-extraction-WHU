#include <stdio.h>
#include <stdlib.h>
#include <iostream>   
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui_c.h>
using namespace std;
using namespace cv;

//Vegetation Index植被指数部分
//比值植被指数RVI
float* RVI(Mat NIR, Mat R, int rows, int cols) {
	float* rvi = new float[rows * cols];
	float max = -1000; float min = 1000;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			rvi[i * cols + j] = NIR.at<uchar>(i, j) / (R.at<uchar>(i, j) + 0.0000001);
		}
	}
	return rvi;
}

//归一化植被指数NDVI
float* NDVI(Mat NIR, Mat R, int rows, int cols) {
	float* ndvi = new float[rows * cols];
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			ndvi[i * cols + j] = (NIR.at<uchar>(i, j) - R.at<uchar>(i, j)) / (NIR.at<uchar>(i, j) + R.at<uchar>(i, j) + 0.0000001);
		}
	}
	return ndvi;
}

//土壤调节植被指数SAVI
float* SAVI(Mat NIR, Mat R, int rows, int cols) {
	float* savi = new float[rows * cols];
	float L = 0.5;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			savi[i * cols + j] = (NIR.at<uchar>(i, j) - R.at<uchar>(i, j)) / (NIR.at<uchar>(i, j) + R.at<uchar>(i, j) + L + 0.00000001) * (1 + L);
		}
	}
	return savi;
}


//Water Index水体指数部分
//归一化水体指数NDWI
float* NDWI(Mat G, Mat NIR, int rows, int cols) {
	float* ndwi = new float[cols * rows];
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			ndwi[i * cols + j] = (G.at<uchar>(i, j) - NIR.at<uchar>(i, j)) / (G.at<uchar>(i, j) + NIR.at<uchar>(i, j) + 0.0000001);
		}
	}
	return ndwi;
}

//归一化差异水体指数MNDWI
float* MNDWI(Mat G, Mat SWIR1, int rows, int cols) {
	float* mndwi = new float[rows * cols];
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			mndwi[i * cols + j] = (G.at<uchar>(i, j) - SWIR1.at<uchar>(i, j)) / (G.at<uchar>(i, j) + SWIR1.at<uchar>(i, j) + 0.0000001);
		}
	}
	return mndwi;
}

//自动化水体提取指数AWEInsh
float* AWEInsh(Mat G, Mat NIR, Mat SWIR1, Mat SWIR2, int rows, int cols) {
	float* awein = new float[rows * cols];
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			awein[i * cols + j] = 4 * (G.at<uchar>(i, j) - SWIR1.at<uchar>(i, j)) / (0.25 * NIR.at<uchar>(i, j) + 2.75 * SWIR2.at<uchar>(i, j) + 0.0000001);
		}
	}
	return awein;
}


//Building Index建筑裸地指数部分
//差值建筑覆盖指数DBI
float* DBI(Mat NIR, Mat SWIR2, int rows, int cols) {
	float* dbi = new float[rows * cols];
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			dbi[i * cols + j] = SWIR2.at<uchar>(i, j) - NIR.at<uchar>(i, j);
		}
	}
	return dbi;
}

//归一化差值建筑用地指数NDBI
float* NDBI(Mat SWIR1, Mat NIR, int rows, int cols) {
	float* ndbi = new float[rows * cols];
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			ndbi[i * cols + j] = (SWIR1.at<uchar>(i, j) - NIR.at<uchar>(i, j)) / (SWIR1.at<uchar>(i, j) + NIR.at<uchar>(i, j) + 0.00000001);
		}
	}
	return ndbi;
}

//归一化差值裸地与建筑用地指数NDBBI
float* NDBBI(Mat G, Mat NIR, Mat SWIR2, int rows, int cols) {
	float* ndbbi = new float[rows * cols];
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			ndbbi[i * cols + j] = (1.5 * SWIR2.at<uchar>(i, j) - 0.5 * (NIR.at<uchar>(i, j) + G.at<uchar>(i, j))) / (1.5 * SWIR2.at<uchar>(i, j) + 0.5 * (NIR.at<uchar>(i, j) + G.at<uchar>(i, j)) + 0.0000001);
		}
	}
	return ndbbi;
}


//迭代法求取阈值
int Get_Threshold(float* index, int rows, int cols) {//迭代求取阈值
	int Thre = 0;
	int lowNum = 0;
	int highNum = 0;
	int lowSum = 0;
	int highSum = 0;
	int new_Thre = 128;//阈值
	int iter = 0;
	while (Thre != new_Thre && iter < 1000) {//阈值进行迭代改变或者迭代未达到截止次数
		iter++;
		lowNum = 0;
		highNum = 0;
		lowSum = 0;
		highSum = 0;
		Thre = new_Thre;
		for (int i = 0; i < rows * cols; i++) {
			if (index[i] > Thre) {
				highSum += index[i];
				highNum++;
			}
			else {
				lowSum += index[i];
				lowNum++;
			}
		}
		new_Thre = (highSum / highNum + lowSum / lowNum) / 2;
	}
	return new_Thre;
}

void man()
{
	//读取1 - 7波段灰度图

	Mat B = imread("../image/tm1.tif", -1);//蓝光波段
	Mat G = imread("../image/tm2.tif", -1);//绿光波段
	Mat R = imread("../image/tm3.tif", -1);//红光波段
	Mat NIR = imread("../image/tm4.tif", -1);//近红外波段
	Mat SWIR1 = imread("../image/tm5.tif", -1);//中红外波段
	Mat TIR = imread("../image/tm6.tif", -1);//热红外波段
	Mat SWIR2 = imread("../image/tm7.tif", -1);//远红外波段
	int rows = B.rows;
	int cols = B.cols;
	float* index = new float[rows * cols];
	printf("请选择您所需要的植被或水体或建筑用地指数进行求解\n");
	printf("(注：使用输入序号的方法进行指数选取，下面为指数对应序号)\n\n");

	printf("Vegetation Index植被指数部分：\n");
	printf("1:比值植被指数RVI  2:归一化植被指数NDVI  3:土壤调节植被指数SAVI\n\n");

	printf("Water Index水质指数部分：\n");
	printf("4:归一化水体指数NDWI 5:归一化差异水体指数MNDWI 6:自动化水体提取指数AWEInsh\n\n");

	printf("Building Index建筑用地指数部分：\n");
	printf("7:差值建筑覆盖指数DBI 8:归一化差值建筑用地指数NDBI 9:归一化差值裸地与建筑用地指数NDBBI\n\n");

	printf("请输入所需的指数对应的序号（直接输入数字即可）：");
	int choose = 0;
	scanf_s("%d", &choose);
	switch (choose)
	{//选择所需的指数（对应序号）
	case 1:index = RVI(NIR, R, rows, cols); break;
	case 2:index = NDVI(NIR, R, rows, cols); break;
	case 3:index = SAVI(NIR, R, rows, cols); break;
	case 4:index = NDWI(G, NIR, rows, cols); break;
	case 5:index = MNDWI(G, SWIR1, rows, cols); break;
	case 6:index = AWEInsh(G, NIR, SWIR1, SWIR2, rows, cols); break;
	case 7:index = DBI(NIR, SWIR2, rows, cols); break;
	case 8:index = NDBI(SWIR1, NIR, rows, cols); break;
	case 9:index = NDBBI(G, NIR, SWIR2, rows, cols); break;
	}
	//灰度拉伸展示遥感指数产生的影像
	float max = -1000; float min = 1000;
	Mat mat = B.clone();
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (index[i * cols + j] > max)max = index[i * cols + j];
			if (index[i * cols + j] < min)min = index[i * cols + j];
		}
	}
	for (int i = 0; i < cols * rows; i++)index[i] = (index[i] - min) / (max - min) * 255;
	//阈值分割
	int temp = 0;
	temp = Get_Threshold(index, rows, cols);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			mat.at<uchar>(i, j) = int(index[i * cols + j]);
		}
	}
	namedWindow("选取的指数影像", CV_WINDOW_AUTOSIZE);
	imshow("选取的指数影像", mat);
	waitKey();
	//基于迭代求解的阈值进行初步分割
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (mat.at<uchar>(i, j) > temp)mat.at <uchar>(i, j) = 255;
			else {
				mat.at<uchar>(i, j) = 0;
			}
		}
	}
	printf("此指数分割阈值为：");
	printf("%d\n", temp);
	namedWindow("阈值分割影像", CV_WINDOW_AUTOSIZE);
	imshow("阈值分割影像", mat);
	waitKey();
	exit;
	//询问用户对阈值分割结果是否满意，若不满意可自己输入阈值进行分割
	printf("请问是否修改阈值，若修改则输入1，不修改则输入0\n");
	int n = 0;
	scanf_s("%d", &n);
	if (n == 0)exit;
	else if (n == 1) {
		int h = 0;
		printf("请输入您修改的阈值： ");
		scanf_s("%d", &h);
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				mat.at<uchar>(i, j) = int(index[i * cols + j]);
				if (mat.at<uchar>(i, j) > h)mat.at <uchar>(i, j) = 255;
				else {
					mat.at<uchar>(i, j) = 0;

				}
			}
		}
		namedWindow("修改后阈值分割影像", CV_WINDOW_AUTOSIZE);
		imshow("修改后阈值分割影像", mat);
		waitKey();
		exit;
	}
	system("pause");
}

