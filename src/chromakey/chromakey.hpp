#pragma once

#include <opencv2/core/core.hpp>

/**
 * @brief Compute the chroma mask for the given image.
 * @param[in] bgrImg The image to process.
 * @param[out] outImg The original image with a reduce green spill.
 * @param[out] out_mask The output mask as a grayscale image (not binary for better blending).
 */
void computeChromaMask(const cv::Mat &bgrImg, cv::Mat& outImg, cv::Mat &out_mask);

/**
 * @brief Given an input image and a mask image it return a 4-channels image, with the forth channel
 * being the mask (alpha).
 * @param[in] imageIn Input image.
 * @param[in] imageMask Input mask.
 * @return A 4-channels image composed of the input image and having as 4th channel the provided mask.
 */
cv::Mat applyTransparencyMask(const cv::Mat& imageIn, const cv::Mat& imageMask);

/**
 * @brief Given a input image, a background image and an alpha mask, it blends the two images according to the
 * alpha mask.
 * @param foreground The 3-channels input image.
 * @param background The 3-channels image to blend.
 * @param alpha The alpha mask used to blend.
 * @return A 3-channels image that blends together the two input images.
 */
cv::Mat alphaBlending(const cv::Mat& foreground, const cv::Mat& background, const cv::Mat& alpha);
