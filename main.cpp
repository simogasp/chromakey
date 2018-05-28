#include "chromakey.hpp"
#include <opencv2/core.hpp>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv/highgui.h>
#include <opencv/cv.hpp>

void usage(const char* argv)
{
    std::cout << "Usage:\n " << argv[0] << " <filename> " << "\n";
}

int main(int argc, char* argv[] )
{
    if(argc != 2)
    {
        usage(argv[0]);
    }

    // load image
    cv::Mat rgbImg = cv::imread(std::string(argv[1]), cv::IMREAD_COLOR);

    cv::Mat outMask;
    cv::Mat outImage;

    computeChromaMask(rgbImg, outImage, outMask);

    outImage = applyTransparencyMask(outImage, outMask);

    const std::string ORIGINAL_IMG = "original image";
    const std::string OUTPUT_IMG = "mask image";
    const std::string FINAL_IMG = "final image";

    cvNamedWindow(ORIGINAL_IMG.c_str());
    cv::imshow(ORIGINAL_IMG, rgbImg);
    cv::waitKey(-1);

    cvNamedWindow(OUTPUT_IMG.c_str());
    cv::imshow(OUTPUT_IMG, outMask);
    cv::waitKey(-1);

    cvNamedWindow(FINAL_IMG.c_str());
    cv::imshow(FINAL_IMG, outImage);
    cv::waitKey(-1);

    cv::imwrite("result.png", outImage);

    return EXIT_SUCCESS;
}