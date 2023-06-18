#include "ThresholdSegmentation.h"
int ThresholdSegmentation::iterativeThreshold(float* index, int rows, int cols) {
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