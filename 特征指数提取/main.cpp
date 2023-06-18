#include <iostream>

#include "VegetationIndex.h"
#include "WaterIndex.h"
#include "BuildingIndex.h"
#include "ThresholdSegmentation.h"

#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>


int main() {
	/* ��ȡtifͼƬ */

	//���Ⲩ��
	cv::Mat B = cv::imread("../image/tm1.tif", -1);
	//�̹Ⲩ��
	cv::Mat G = cv::imread("../image/tm2.tif", -1);
	//��Ⲩ��
	cv::Mat R = cv::imread("../image/tm3.tif", -1);
	//�����Ⲩ��
	cv::Mat NIR = cv::imread("../image/tm4.tif", -1);
	//�к��Ⲩ��
	cv::Mat SWIR1 = cv::imread("../image/tm5.tif", -1);
	//�Ⱥ��Ⲩ��
	cv::Mat TIR = cv::imread("../image/tm6.tif", -1);
	//Զ���Ⲩ��
	cv::Mat SWIR2 = cv::imread("../image/tm7.tif", -1);

	/* ����ͼ���С */
	int rows = B.rows;
	int cols = B.cols;

	/* �˳�ָ�� */
	bool run = true;

	while (run) {

		/* ����ѡ��λ */
		int outFlag = -1;
		int innerFlag = -1;

		/* ���н������� */
		float* result = new float[rows * cols];

		system("cls");
		std::cout << "��ѡ�����ָ������(ֱ�����밢��������)����ֲ�� ��ˮ�� �۽��� ���˳�����" << std::endl << ">> " << std::flush;
		std::cin >> outFlag;
		system("cls");

		if (outFlag == 1) {
			std::cout << "��ѡ��Ҫ�����ֲ��ָ�����ٱ�ֲֵ��ָ��RVI �ڹ�һ��ֲ��ָ��NDVI ����������ֲ��ָ��SAVI" << std::endl << ">> " << std::flush;
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
				std::cout << "����������Ч����ֱ�����밢��������" << std::endl;
				break;
			}
		}
		else if (outFlag == 2)
		{
			std::cout << "��ѡ��Ҫ�����ˮ��ָ�����ٹ�һ��ˮ��ָ��NDWI �ڹ�һ������ˮ��ָ��MNDWI ���Զ���ˮ����ȡָ��AWEInsh" << std::endl << ">> " << std::flush;
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
				std::cout << "����������Ч����ֱ�����밢��������" << std::endl;
				break;
			}
		}
		else if (outFlag == 3) {
			std::cout << "��ѡ��Ҫ����Ľ����õ�ָ�����ٲ�ֵ��������ָ��DBI �ڹ�һ����ֵ�����õ�ָ��NDBI �۹�һ����ֵ����뽨���õ�ָ��NDBBI" << std::endl << ">> " << std::flush;
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
				std::cout << "����������Ч����ֱ�����밢��������" << std::endl;
				break;
			}
		}
		else if (outFlag == 4) { run = false; exit(0); }

		/* �Ҷ����� */
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

		/* ��ֵ�ָ� */
		int temp = 0;
		ThresholdSegmentation thresholder;
		temp = thresholder.iterativeThreshold(result, rows, cols);
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				mat.at<uchar>(i, j) = int(result[i * cols + j]);
			}
		}

		delete[] result;

		/* ͼ��չʾ */
		cv::namedWindow("ָ��Ӱ��", CV_WINDOW_AUTOSIZE);
		imshow("ָ��Ӱ��", mat);


		/* ���ڵ���������ֵ���г����ָ� */
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (mat.at<uchar>(i, j) > temp) { mat.at <uchar>(i, j) = 255; }
				else { mat.at<uchar>(i, j) = 0; }
			}
		}


		std::cout << "��ָ���ָ���ֵΪ��" << temp << std::endl;

		cv::namedWindow("��ֵ�ָ���ֵ��Ӱ��", CV_WINDOW_AUTOSIZE);
		imshow("��ֵ�ָ���ֵ��Ӱ��", mat);
		cv::waitKey();
		exit;
	}

	return 1;
}

