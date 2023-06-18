#pragma once

#ifndef THRESHOLD_SEGMENTATION_H
#define THRESHOLD_SEGMENTATION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class ThresholdSegmentation {
public:
	ThresholdSegmentation() = default;
	int iterativeThreshold(float* index, int rows, int cols);
};

#endif  // THRESHOLD_SEGMENTATION_H
