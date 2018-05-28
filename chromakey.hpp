#pragma once

#include <opencv2/core/core.hpp>

void computeChromaMask(const cv::Mat &bgrImg, cv::Mat& outImg, cv::Mat &out_mask);

cv::Mat applyTransparencyMask(const cv::Mat& imageIn, const cv::Mat& imageMask);
